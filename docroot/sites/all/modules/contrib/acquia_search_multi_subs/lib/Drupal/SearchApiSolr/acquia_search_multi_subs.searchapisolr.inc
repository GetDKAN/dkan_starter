<?php

/**
 * @file
 * Contains code specific to the Search API Solr Search module.
 */

/**
 * Initializes this module's Apache Solr Search Integration module support.
 *
 * Changes the Acquia Search environment on the fly based on the
 * AH_SITE_ENVIRONMENT and AH_SITE_NAME server variables.
 *
 * This will be called on every page request if the "search_api_acquia" module
 * is installed.
 *
 * @see acquia_search_multi_subs_init()
 */
function acquia_search_multi_subs_searchapi_init() {
  // Get information from environment.
  $acquia_identifier = acquia_agent_settings('acquia_identifier');
  $ah_site_environment = isset($_ENV['AH_SITE_ENVIRONMENT']) ? $_ENV['AH_SITE_ENVIRONMENT'] : '';
  $ah_site_name = isset($_ENV['AH_SITE_NAME']) ? $_ENV['AH_SITE_NAME'] : '';
  $ah_region = isset($_ENV['AH_CURRENT_REGION']) ? $_ENV['AH_CURRENT_REGION'] : '';
  $sites_foldername = substr(conf_path(), strrpos(conf_path(), '/') + 1);
  $subscription_expected_search_cores = acquia_search_multi_subs_get_expected_search_cores($acquia_identifier, $ah_site_environment, $ah_site_name, $sites_foldername);
  // Get the search cores available in the subscription.
  $available_search_cores = acquia_search_multi_subs_get_search_cores();

  // Load the Search API servers to check. Only load servers using the
  // acquia_search_service class.
  $servers = search_api_server_load_multiple(FALSE, array('class' => 'acquia_search_service'));
  foreach ($servers as $server) {
    // Load defaults.
    $options = $server->options;
    $options += array('acquia_override_subscription' => array());
    $options['acquia_override_subscription'] += acquia_search_multi_subs_default_configuration();

    // Get some variables
    $environment_core_name = $options['acquia_override_subscription']['acquia_override_subscription_corename'];
    $acquia_override_auto_switch = $options['acquia_override_subscription']['acquia_override_auto_switch'];
    $acquia_override_selector = $options['acquia_override_subscription']['acquia_override_selector'];
    $acquia_override_failover = $options['acquia_override_subscription']['acquia_override_failover'];

    // If auto switch is enabled, search thru all the subscription's search cores
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
      if ($match_found) {
        break;
      }
      // Loop over all the available search cores.
      foreach ($available_search_cores as $available_search_core) {
        if ($available_search_core['core_id'] == $expected_core_name) {
          // If the core is already the same, don't do anything.
          if ($available_search_core['core_id'] != $environment_core_name) {
            acquia_search_multi_subs_searchapi_set_default_core($server, $available_search_core);
            watchdog('acquia_search_multi_subs', 'Switched Search API solr server "@server_name" from core @old_core_id to @new_core_id', array('@server_name' => $server->name, '@old_core_id' => $environment_core_name, '@new_core_id' => $available_search_core['core_id']), WATCHDOG_NOTICE);
          }
          $match_found = true;
          break;
        }
      }
    }
  }
}

/**
 * Sets our default search core for the acquia index.
 *
 * @param SearchApiServer $server
 *   Search API Server object we are modifying.
 * @param $search_core
 *   The search core data array from the connector subscription data.
 */
function acquia_search_multi_subs_searchapi_set_default_core(SearchApiServer &$server, $search_core) {
  $server->options['acquia_override_subscription']['acquia_override_subscription_corename'] = $search_core['core_id'];
  // Get the derived key salt from the subscription.
  $subscription = acquia_agent_settings('acquia_subscription_data');
  $derived_key_salt = $subscription['derived_key_salt'];
  // Get the derived key salt from the subscription.
  $key = acquia_agent_settings('acquia_key');
  // Create our derived key.
  $derived_key = _acquia_search_multi_subs_create_derived_key($derived_key_salt, $search_core['core_id'], $key);
  $server->options['derived_key'] = $derived_key;
  $server->save();
}
