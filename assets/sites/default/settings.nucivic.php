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
  case 'production':
    $conf['features_master_temp_enabled_modules'] = array_merge(
      $conf['features_master_temp_enabled_modules'],
      array(
        'nucivic_data_devops',
      )
    );
    // Remove this bit after you are sure you have a solr instance per env.
    $conf['features_master_temp_disabled_modules'] = array_merge(
      $conf['features_master_temp_disabled_modules'],
      array(
        'dkan_acquia_search_solr',
      ),
    );
    break;
}
