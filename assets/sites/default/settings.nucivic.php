<?php
/**
 * NuCivic Settings.
 */

/**
 * NuCivic specific config per environment
 */
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
