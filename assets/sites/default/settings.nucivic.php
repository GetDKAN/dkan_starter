<?php
/**
 * NuCivic Settings.
 */

/**
 * NuCivic specific config per environment
 */
$conf['features_master_temp_enabled_modules'][] = 'dkan_default_content';

switch(ENVIRONMENT) {
  case 'development':
  case 'test':
    // Remove this if you are certain you have multiple cores on an acquia site.
    if (!isset($conf['features_master_temp_disabled_modules'])) {
      $conf['features_master_temp_disabled_modules'] = array();
    }
    $conf['features_master_temp_disabled_modules'] = array_merge(
      $conf['features_master_temp_disabled_modules'],
      array(
        'search_api_solr',
        'search_api_acquia',
        'dkan_acquia_search_solr',
      )
    );
    break;
  case 'production':
    // Add this back when we split backup and monitoring functionality in 
    // $conf['features_master_temp_enabled_modules'] = array_merge(
    //   $conf['features_master_temp_enabled_modules'],
    //   array(
    //     'nucivic_data_devops',
    //   )
    // );
    break;
}

/******************************************************
 * OPTIONAL: Perform tasks when switching environments.
 *****************************************************/
/* For environment switching to work, ensure environment.module is enabled and
 * use either hook_environment_switch() in a custom module, or simply define
 * devinci_custom_environment_switch() in settings.php as shown below.
 */
function devinci_custom_environment_switch($target_env, $current_env) {
  switch($target_env) {
    case 'local':
      // Set the search server to use the local solr server instead of acquia's
      db_query("UPDATE search_api_index set server = 'local_solr_server' where server = 'dkan_acquia_solr'");
      //db_query("DELETE FROM search_api_index where server IS NULL");
      db_query("UPDATE search_api_server set enabled = 0 WHERE machine_name <> 'local_solr_server'");

    case 'development':
    case 'test':
    case 'production':
      drupal_flush_all_caches();
      features_master_features_revert('custom_config');
      features_master_features_revert('custom_config');
      features_revert_module('visualization_entity_charts_dkan');
      features_revert_module('dkan_dataset_content_types');
      features_revert_module('dkan_sitewide_search_db');
      features_revert_module('dkan_data_story');
      features_revert_module('dkan_dataset_groups');
      features_revert_module('dkan_permissions');
      break;
  }
}
