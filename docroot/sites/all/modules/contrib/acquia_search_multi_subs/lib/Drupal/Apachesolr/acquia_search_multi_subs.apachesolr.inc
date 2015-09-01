<?php

/**
 * @file
 * Contains code specific to the Apache Solr Search Integration module.
 */

/**
 * Initializes this module's Apache Solr Search Integration module support.
 *
 * Changes the Acquia Search environment on the fly based on the
 * AH_SITE_ENVIRONMENT and AH_SITE_NAME server variables.
 *
 * This will be called on every page request if the "acquia_search" module is
 * installed.
 *
 * @see acquia_search_multi_subs_init()
 */
function acquia_search_multi_subs_apachesolr_init() {
  // Get information from environment.
  $acquia_identifier = acquia_agent_settings('acquia_identifier');
  $ah_site_environment = isset($_ENV['AH_SITE_ENVIRONMENT']) ? $_ENV['AH_SITE_ENVIRONMENT'] : '';
  $ah_site_name = isset($_ENV['AH_SITE_NAME']) ? $_ENV['AH_SITE_NAME'] : '';
  $ah_region = isset($_ENV['AH_CURRENT_REGION']) ? $_ENV['AH_CURRENT_REGION'] : '';
  $sites_foldername = substr(conf_path(), strrpos(conf_path(), '/') + 1);
  $subscription_expected_search_cores = acquia_search_multi_subs_get_expected_search_cores($acquia_identifier, $ah_site_environment, $ah_site_name, $sites_foldername);
  // Get the search cores available in the subscription.
  $available_search_cores = acquia_search_multi_subs_get_search_cores();

  // Load the Apache Solr Search Integration environments to check.
  $environments = apachesolr_load_all_environments();
  foreach ($environments as $environment) {
    // Skip non-Acquia Search environments.
    if ($environment['service_class'] != 'AcquiaSearchService') {
      continue;
    }
    $env_id = $environment['env_id'];

    // Get some variables
    $environment_core_name = apachesolr_environment_variable_get($env_id, 'acquia_override_subscription_corename');
    $environment_core_url = $environment['url'];
    $expected_core_host = acquia_search_multi_subs_get_hostname($environment_core_name);
    $acquia_override_auto_switch = apachesolr_environment_variable_get($env_id, 'acquia_override_auto_switch', TRUE);
    $acquia_override_failover = apachesolr_environment_variable_get($env_id, 'acquia_override_failover', FALSE);
    $acquia_override_selector = apachesolr_environment_variable_get($env_id, 'acquia_override_selector');

    // If auto switch is enabled, search through all the subscription's search cores
    // to find best match.
    if ($acquia_override_auto_switch) {
      $expected_core_names = $subscription_expected_search_cores;
    }
    else {
      // If auto switch is disabled, only try to match the specified core.
      if ($acquia_override_selector == 'other') {
        $expected_core_names = array($environment_core_name);
      }
      elseif ($acquia_override_selector == 'default') {
        $expected_core_names = array(acquia_agent_settings('acquia_identifier'));
      }
      else {
        $expected_core_names = array($acquia_override_selector);
      }
    }

    // Remove available cores from other regions if multiregion is enabled.
    if ($acquia_override_failover) {
      foreach ($available_search_cores as $core_id => $available_search_core) {
        $expected_search_core_region = str_replace('-', '', $ah_region);
        $matches = array();
        preg_match("/^([^-]*)/", $available_search_core['balancer'], $matches);
        $search_core_region = reset($matches);
        if ($search_core_region != $expected_search_core_region) {
          unset($available_search_cores[$core_id]);
        }
      }
    }
    else {
      // if not enabled, remove all failover indexes from being a possible candidate
      foreach ($subscription_expected_search_cores as $search_core_id => $search_core) {
        if (strstr($search_core, 'failover')) {
          unset($subscription_expected_search_cores[$search_core_id]);
        }
      }
    }

    $match_found = false;
    foreach ($expected_core_names as $expected_core_name) {
      // This allows us to break from the 2-level deep foreach.
      // @see the foreach loop below.
      if ($match_found) {
        break;
      }

      // Note it's OK to use http:// on the following since it will be replaced
      // later with https:// if needed.
      $expected_environment_core_url = 'http://' . $expected_core_host . '/solr/' . $expected_core_name;

      // Loop over all the available search cores.
      foreach ($available_search_cores as $available_search_core) {

        // Checking if we need to switch to another core.
        if ($available_search_core['core_id'] == $expected_core_name) {
          // If the core is already the same, don't do anything.
          if ($expected_environment_core_url != $environment_core_url) {
            acquia_search_multi_subs_apachesolr_set_default_core($env_id, $available_search_core);
            watchdog('acquia_search_multi_subs', 'Switched Apache Solr environment "@env_id" from core @old_core_id to @new_core_id', array('@env_id' => $env_id, '@old_core_id' => $environment_core_url, '@new_core_id' => $expected_environment_core_url), WATCHDOG_NOTICE);
          }
          $match_found = true;
          break;
        }
      }
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for apachesolr_environment_edit_form().
 *
 * Adds the configuration for switching servers.
 *
 * @see acquia_search_multi_subs_get_settings_form()
 * @see acquia_search_multi_subs_environment_edit_form_validate()
 * @see acquia_search_multi_subs_environment_edit_form_submit()
 */
function acquia_search_multi_subs_form_apachesolr_environment_edit_form_alter(array &$form, array &$form_state) {
  if ($form['service_class']['#value'] == 'AcquiaSearchService') {

    // Gets environment from form, gets connection status to Acquia Search.
    $env_id = isset($form['env_id']['#default_value']) ? $form['env_id']['#default_value'] : '';
    $environment = ($env_id) ? apachesolr_environment_load($env_id) : FALSE;
    $configuration = $environment ? $environment['conf'] : array();

    // Get our common settings form.
    acquia_search_multi_subs_get_settings_form($form, $form_state, $configuration);

    // Add our submit handler to the form.
    array_unshift($form['actions']['save']['#submit'], 'acquia_search_multi_subs_environment_edit_form_submit');
    array_unshift($form['actions']['save_edit']['#submit'], 'acquia_search_multi_subs_environment_edit_form_submit');
  }
}

/**
 * Form submission handler for apachesolr_environment_edit_form().
 *
 * Adds the values to the environment variable array so Acquia Search will work.
 *
 * @see acquia_search_multi_subs_get_settings_form()
 * @see acquia_search_multi_subs_environment_edit_form_validate()
 */
function acquia_search_multi_subs_environment_edit_form_submit(array $form, array &$form_state) {
  $fv = &$form_state['values'];

  $fv['conf'] = array_merge($fv['conf'], $fv['acquia_override_subscription']);

  // Only activate if the checkbox has been enabled and there is valid content.
  // Add our overrides to the configuration.
  if(!empty($fv['conf']['acquia_override_selector']) && empty($fv['conf']['acquia_override_auto_switch'])) {
    // Set the identifier and key.
    $identifier = $fv['conf']['acquia_override_subscription_id'];
    $key = $fv['conf']['acquia_override_subscription_key'];
    $corename = $fv['conf']['acquia_override_subscription_corename'];

    // Set the derived key for this environment.
    $subscription = acquia_agent_get_subscription(array(), $identifier, $key);
    $derived_key_salt = $subscription['derived_key_salt'];
    $derived_key = _acquia_search_multi_subs_create_derived_key($derived_key_salt, $corename, $key);
    $fv['conf']['acquia_search_key'] = $derived_key;

    $override_values = array();
    $override_values['env_id'] = $form['env_id']['#default_value'];
    $override_values['name'] = $fv['name'];
    $override_values['acquia_subscription_id'] = $identifier;
    $override_values['acquia_subscription_key'] = $key;
    $override_values['acquia_subscription_corename'] = $corename;

    // Merge with our defaults.
    $fv = array_merge($fv, acquia_search_get_environment($override_values));

    $search_host = acquia_search_multi_subs_get_hostname($corename);
    $fv['url'] = 'http://' . $search_host . variable_get('acquia_search_path', '/solr/'. $corename);

    if (empty($fv['conf']['acquia_override_auto_switch'])) {
      // Unset our apachesolr_environments variable. This overrides certain
      // configurations and makes sure that we do not accidentially override the
      // server configs if auto switch is set to disabled.
      variable_del('apachesolr_environments');
    }
  }
  else {
    // Remove any manual configuration so we can auto-set the environment.
    unset($fv['conf']['acquia_search_key']);
    unset($fv['conf']['acquia_subscription_id']);
    unset($fv['conf']['acquia_subscription_key']);
    apachesolr_environment_variable_del($fv['name'], 'acquia_search_key');
    apachesolr_environment_variable_del($fv['name'], 'acquia_override_subscription_id');
    apachesolr_environment_variable_del($fv['name'], 'acquia_override_subscription_key');
    apachesolr_environment_variable_del($fv['name'], 'acquia_override_subscription_corename');
    $fv = array_merge($fv, acquia_search_get_environment());
  }
}

/**
 * Implements hook_acquia_search_enable_alter().
 *
 * Alters the environment before it gets saved.
 *
 * @param array $environment
 *   The environment that is being enabled by the Acquia Search module.
 */
function acquia_search_multi_subs_acquia_search_enable_alter(array &$environment) {
  $env_id = $environment['env_id'];
  $corename = apachesolr_environment_variable_get($env_id, 'acquia_override_subscription_corename');

  // Override the URL
  $search_host = acquia_search_multi_subs_get_hostname($corename);
  $url = 'http://' . $search_host . variable_get('acquia_search_path', '/solr/'. $corename);
  $environment['url'] = $url;
}

/**
 * Sets our default search core for the acquia index.
 *
 * @param string $env_id
 *   Environment identifier
 * @param array $search_core
 *   The search core data array from the connector subscription data
 */
function acquia_search_multi_subs_apachesolr_set_default_core($env_id, array $search_core) {
  // Get the derived key salt from the subscription.
  $key = acquia_agent_settings('acquia_key');
  $subscription = acquia_agent_settings('acquia_subscription_data');
  $derived_key_salt = $subscription['derived_key_salt'];
  // Create our derived key.
  $derived_key = _acquia_search_multi_subs_create_derived_key($derived_key_salt, $search_core['core_id'], $key);

  // Set our variables in the subscription array.
  apachesolr_environment_variable_set($env_id, 'acquia_override_subscription_corename', $search_core['core_id']);
  apachesolr_environment_variable_set($env_id, 'acquia_search_key', $derived_key);
  apachesolr_environment_variable_set($env_id, 'acquia_override_selector', $search_core['core_id']);

  // Generate the url where this core should connect to.
  $url = 'http://' . $search_core['balancer'] . variable_get('acquia_search_path', '/solr/'. $search_core['core_id']);
  $apachesolr_environments = variable_get('apachesolr_environments', array());
  $apachesolr_environments[$env_id]['url'] = $url;
  // Set our variable, needs two page loads to make sure it is in the system.
  // Setting the global conf variable is not valid as some calls to
  // apachesolr_load_all_environments happen too early in the process. If we set
  // the variable, we are sure it is there in the next page load. Also, setting
  // this variable only happens when the environment changes and another
  // environment which fits is found.
  variable_set('apachesolr_environments', $apachesolr_environments);
  drupal_static_reset('apachesolr_load_all_environments');
}
