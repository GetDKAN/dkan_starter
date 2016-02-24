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
