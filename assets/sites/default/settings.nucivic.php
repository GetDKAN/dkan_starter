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
    // Add this back when we split backup and monitoring functionality in 
    // $conf['features_master_temp_enabled_modules'] = array_merge(
    //   $conf['features_master_temp_enabled_modules'],
    //   array(
    //     'nucivic_data_devops',
    //   )
    // );
    break;
}

// Fake the 'derived_key' used to connect to Solr, if we can't find the
// Acquia-set "AH_PRODUCTION" environment variable.
// This will cause all requests to Acquia Search instances respond with 403.
if (!isset($_ENV["AH_PRODUCTION"])) {

  // EDIT THE NEXT LINE TO MATCH your Search API "server" machinename.
  $search_api_server_machine_name = 'dkan_acquia_solr';

  $conf['search_api_acquia_overrides'][$search_api_server_machine_name] = array(
      #'path' => '/solr/[core_ID]',
      #'host' => '[hostname].acquia-search.com',
      'derived_key' => 'FAKE',
  );
}
