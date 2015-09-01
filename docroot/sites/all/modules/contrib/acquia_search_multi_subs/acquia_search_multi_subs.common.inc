<?php

/**
 * @file
 * Helper functions used for the integration with either Solr module.
 */

/**
 * Calculates eligible search core names based on environment information,
 * in order of most likely (or preferred!) core names first.
 *
 * The generated list of expected core names is done according to Acquia Search
 * conventions, prioritized in this order:
 * WXYZ-12345.[env].[sitefolder]
 * WXYZ-12345.[env].default
 * WXYZ-12345_[sitename][env]
 * WXYZ-12345.dev.[sitefolder] (only if $ah_site_environment isn't 'prod')
 * WXYZ-12345_[sitename]dev    (only if $ah_site_environment isn't 'prod')
 * WXYZ-12345                  (only if $ah_site_environment is 'prod')
 *
 * NOTE that [sitefolder] is a stripped-down version of the sites/* folder,
 * such that it is only alphanumeric and max. 16 chars in length.
 * E.g. for sites/www.example.com, the expected corename for a dev environment
 * could be WXYZ-12345.dev.wwwexamplecom
 *
 * @param string $acquia_identifier
 *   Subscription ID. E.g. WXYZ-12345
 * @param string $ah_site_environment
 *   String with the environment, from $_ENV[AH_SITE_ENVIRONMENT].
 *   E.g. 'dev', 'test', 'prod'.
 * @param string $ah_site_name
 *   From $_ENV[AH_SITE_NAME]
 * @param string $sites_foldername
 *   Optional. The current site folder within [docroot]/sites/*.
 *   @see conf_path()
 * @return array
 *   The eligible core_ids sorted by best match first.
 */
function acquia_search_multi_subs_get_expected_search_cores($acquia_identifier, $ah_site_environment, $ah_site_name, $sites_foldername = 'default') {
  // Build eligible environments array.
  $ah_environments = array();
  // If we have the proper environment, add it as the first option.
  if ($ah_site_environment) {
    $ah_environments[$ah_site_environment] = $ah_site_name;
  }
  // Add fallback options. For sites that lack the AH_* variables or are non-prod
  // we will try to match .dev.[sitefolder] cores.
  if ($ah_site_environment != 'prod') {
    $ah_environments['dev'] = $ah_site_name;
  }

  $expected_core_names = array();
  foreach ($ah_environments as $site_environment => $site_name) {
    // The possible core name suffixes are [current site folder name] and 'default'.
    $core_suffixes = array_unique(array($sites_foldername, 'default'));
    foreach ($core_suffixes as $core_suffix) {
      // Fix the $core_suffix: alphanumeric only, and max length == 16
      $core_suffix = preg_replace('@[^a-zA-Z0-9]+@', '', $core_suffix);
      $core_suffix = substr($core_suffix, 0, 16);
      // Add WXYZ-12345.[env].[sitefolder] option.
      $expected_core_names[] = $acquia_identifier . '.' . $site_environment . '.' . $core_suffix;
      // Add WXYZ-12345_[sitename][env] option.
      if (!empty($site_name) && $sites_foldername == 'default') {
        // Replace any weird characters that might appear in the sitegroup name or
        // identifier.
        $site_name = preg_replace('@[^a-zA-Z0-9_-]+@', '_', $site_name);
        $expected_core_names[] = $acquia_identifier . '_' . $site_name;
      }
    }
    // Add our failover options
    $expected_core_names[] = $acquia_identifier . '.' . $site_environment . '.failover';
  }
  // Add suffix-less core if we're on prod now. If the sitename is empty,
  // it means we are not on Acquia Hosting or something is wrong. Do not
  // allow the prod index to be one of the available cores.
  if ($ah_site_environment == 'prod' && $ah_site_name != '') {
    $expected_core_names[] = $acquia_identifier;
  }

  return array_unique($expected_core_names);
}

/**
 * Retrieves all the available search cores as set in the subscription.
 *
 * @return array
 *   The search cores that are available according to the information in the
 *   rpc backend.
 */
function acquia_search_multi_subs_get_search_cores() {
  $search_cores = drupal_static(__FUNCTION__, array());

  // See if we can return it from static cache.
  if (!empty($search_cores)) {
    return $search_cores;
  }

  $subscription = acquia_agent_settings('acquia_subscription_data');
  // Get our search cores if they exist.
  if (isset($subscription['heartbeat_data']['search_cores'])) {
    $search_cores = $subscription['heartbeat_data']['search_cores'];
  }
  return $search_cores;
}

/**
 * Form constructor for configuring the Solr server switch.
 *
 * Used by SearchApiAcquiaSearchMultiService::configurationForm() and
 * acquia_search_multi_subs_form_apachesolr_environment_edit_form_alter() to add
 * these options in the appropriate place for the different modules.
 *
 * @param array $form
 *   The existing form's array, passed as a reference to be appended to.
 * @param array $form_state
 *   The form's current state.
 * @param array $configuration
 *   The existing configuration for this environment/server.
 */
function acquia_search_multi_subs_get_settings_form(array &$form, array &$form_state, array $configuration) {
  // Fill in defaults.
  $configuration += acquia_search_multi_subs_default_configuration();

  // Define the override form.
  $form['acquia_override_subscription'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure Acquia Search'),
    '#description' => t('This is usually not necessary unless you really want this search environment to connect to a different Acquia search subscription. By default it uses your subscription that was configured in the <a href="@url">Acquia Agent</a>.', array('@url' => url('admin/config/system/acquia-agent'))),
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#weight' => 11,
    '#element_validate' => array('acquia_search_multi_subs_form_validate'),
  );

  // Add a checkbox to auto switch per environment.
  $form['acquia_override_subscription']['acquia_override_auto_switch'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically switch when an Acquia Environment is detected'),
    '#description' => t('Based on the detection of the AH_SITE_NAME and AH_SITE_ENVIRONMENT header we can detect which environment you are currently using and switch the Acquia Search Core automatically if there is a corresponding core.'),
    '#default_value' => $configuration['acquia_override_auto_switch'],
  );

  // Add a form element to make it easier to choose from multiple cores.
  $options = array('default' => t('Default'), 'other' => t('Other'));
  $search_cores = acquia_search_multi_subs_get_search_cores();
  $failover_exists = NULL;
  $failover_region = NULL;
  if (is_array($search_cores)) {
    foreach ($search_cores as $search_core) {
      $options[$search_core['core_id']] = $search_core['core_id'];
      if (strstr($search_core['core_id'], '.failover')) {
        $failover_exists = TRUE;
        $matches = array();
        preg_match("/^([^-]*)/", $search_core['balancer'], $matches);
        $failover_region = reset($matches);
      }
    }
  }
  $form['acquia_override_subscription']['acquia_override_selector'] = array(
    '#type' => 'select',
    '#title' => t('Acquia Search Core'),
    '#options' => $options,
    '#default_value' => $configuration['acquia_override_selector'],
    '#description' => t('Choose any of the available search cores or manually define one by choosing "other". To use the default settings, select default.'),
    '#states' => array(
      'visible' => array(
        ':input[name*="acquia_override_auto_switch"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Show a warning if there are not enough cores available to make the auto
  // switch possible.
  if (count($options) <= 2) {
    $t_args = array('!refresh' => l(t('refresh'), 'admin/config/system/acquia-search-multi-subs/refresh-status', array('query' => array( 'destination' => current_path()))));
    drupal_set_message(t('It seems you only have 1 Acquia Search index. To find out if you are eligible for a search core per environment it is recommended you open a support ticket with Acquia. Once you have that settled, !refresh your subscription so it pulls in the latest information to connect to your indexes.', $t_args), 'warning', FALSE);
  }

  // Generate the custom form.
  $form['acquia_override_subscription']['acquia_override_subscription_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your Acquia Subscription Identifier'),
    '#description' => t('Prefilled with the identifier of the Acquia Connector. You can find your details in Acquia Insight.'),
    '#default_value' => $configuration['acquia_override_subscription_id'],
    '#states' => array(
      'visible' => array(
        ':input[name*="acquia_override_selector"]' => array('value' => 'other'),
        ':input[name*="acquia_override_auto_switch"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['acquia_override_subscription']['acquia_override_subscription_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your Acquia Subscription key'),
    '#description' => t('Prefilled with the key of the Acquia Connector. You can find your details in Acquia Insight.'),
    '#default_value' => $configuration['acquia_override_subscription_key'],
    '#states' => array(
      'visible' => array(
        ':input[name*="acquia_override_selector"]' => array('value' => 'other'),
        ':input[name*="acquia_override_auto_switch"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['acquia_override_subscription']['acquia_override_subscription_corename'] = array(
    '#type' => 'textfield',
    '#description' => t('Please enter the name of the Acquia Search core you want to connect to that belongs to the above identifier and key. In most cases you would want to use the dropdown list to get the correct value.'),
    '#title' => t('Enter your Acquia Search Core Name'),
    '#default_value' => $configuration['acquia_override_subscription_corename'],
    '#states' => array(
      'visible' => array(
        ':input[name*="acquia_override_selector"]' => array('value' => 'other'),
        ':input[name*="acquia_override_auto_switch"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Check if we have MR cores.
  if ($failover_exists) {
    // Add a checkbox to auto switch to the failover variant in a defined region.
    $t_args = array(
      '%region' => $failover_region,
      '@import' => url('https://github.com/acquia/acquia-search-service-import'),
      '@export' => url('https://github.com/acquia/acquia-search-service-import'),
    );
    $form['acquia_override_subscription']['acquia_override_failover'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable multi-region failover support.'),
      '#description' => t('If the Drupal site is hosted in %region it will fail over to the fail-over index. You still need to use the Acquia Search Service <a href="@export">Export</a> and <a href="@import">Import</a> tools to sync your Acquia Search index between the two.', $t_args),
      '#default_value' => $configuration['acquia_override_failover'],
      '#states' => array(
        'visible' => array(
          ':input[name*="acquia_override_auto_switch"]' => array('checked' => TRUE),
        ),
      ),
    );
  }
}

/**
 * Returns default configuration for the override functionality.
 *
 * @return array
 *   The default configuration.
 */
function acquia_search_multi_subs_default_configuration() {
  return array(
    'acquia_override_auto_switch' => TRUE,
    'acquia_override_selector' => '',
    'acquia_override_failover' => FALSE,
    'acquia_override_subscription_id' => acquia_agent_settings('acquia_identifier'),
    'acquia_override_subscription_key' => acquia_agent_settings('acquia_key'),
    'acquia_override_subscription_corename' => '',
    'acquia_search_multi_subs_get_settings_form' => FALSE,
  );
}

/**
 * Form element validation handler for the subscription override sub-form.
 *
 * @see acquia_search_multi_subs_get_settings_form()
 */
function acquia_search_multi_subs_form_validate(array $element, &$form_state) {
  // Get the correct values array.
  $form_values = $form_state['values'];
  foreach ($element['#parents'] as $key) {
    $form_values = $form_values[$key];
  }

  // Don't check the settings further if we are set to "auto" mode.
  if (!empty($form_values['acquia_override_auto_switch'])) {
    // Already set the correct ID and key, though, since those won't change.
    // Only the core name is adapted automatically.
    $form_state['values']['acquia_override_subscription_id'] = acquia_agent_settings('acquia_identifier');
    $form_state['values']['acquia_override_subscription_key'] = acquia_agent_settings('acquia_key');
    return;
  }
  else {
    // If we do not auto switch, we also do not auto fail-over to other regions.
    unset($form_state['values']['acquia_override_failover']);
    $form_state['values']['options']['form']['acquia_override_subscription']['acquia_override_failover'] = 0;
  }

  // If the selector is set to a core (not "other"), insert the proper settings.
  if ($form_values['acquia_override_selector'] == 'other') {
    if ($form_values['acquia_override_subscription_id'] == '') {
      form_error($element['acquia_override_subscription_id'], t('You must at least fill in a valid Acquia Subscription Identifier.'));
    }
    if ($form_values['acquia_override_subscription_key'] == '') {
      form_error($element['acquia_override_subscription_key'], t('You must at least fill in a valid Acquia Subscription key.'));
    }
    if ($form_values['acquia_override_subscription_corename'] == '') {
      $form_state['values']['acquia_override_subscription_corename'] = $form_values['acquia_override_subscription_id'];
    }
  }
  elseif ($form_values['acquia_override_selector'] == 'default') {
    // Delete all possible index variables from the Acquia Multi Subs module, since
    // we revert to default for this Search Environment
    $form_state['values']['acquia_override_subscription_id'] = acquia_agent_settings('acquia_identifier');
    $form_state['values']['acquia_override_subscription_key'] = acquia_agent_settings('acquia_key');
    $form_state['values']['acquia_override_subscription_corename'] = acquia_agent_settings('acquia_identifier');
  }
  else {
    $form_state['values']['acquia_override_subscription_id'] = acquia_agent_settings('acquia_identifier');
    $form_state['values']['acquia_override_subscription_key'] = acquia_agent_settings('acquia_key');
    $form_state['values']['acquia_override_subscription_corename'] = $form_values['acquia_override_selector'];
  }

  // Set the identifier and key.
  $identifier = $form_values['acquia_override_subscription_id'];
  $key = $form_values['acquia_override_subscription_key'];

  $subscription = acquia_agent_get_subscription(array(), $identifier, $key);
  if (!is_array($subscription)) {
    form_error($element['acquia_override_subscription_key'], t('This combination of ID and key is not valid.'));
  }
}

/**
 * Retrieves the host name to use for a certain Acquia Search core.
 *
 * @param string $corename
 *   The core_id of the Acquia Search core.
 *
 * @return string
 *   The domain to use for this core.
 */
function acquia_search_multi_subs_get_hostname($corename) {
  // Override the URL.
  $search_host = variable_get('acquia_search_host', 'search.acquia.com');
  $search_cores = acquia_search_multi_subs_get_search_cores();
  foreach ($search_cores as $search_core) {
    if ($corename == $search_core['core_id']) {
      $search_host = $search_core['balancer'];
    }
  }
  return $search_host;
}

/**
 * Derives a key for the solr hmac using a salt, id and key.
 *
 * Identical copy of _acquia_search_create_derived_key(), to avoid a dependency
 * on the acquia_search module.
 */
function _acquia_search_multi_subs_create_derived_key($salt, $id, $key) {
  $derivation_string = $id . 'solr' . $salt;
  return hash_hmac('sha1', str_pad($derivation_string, 80, $derivation_string), $key);
}
