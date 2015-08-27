<?php

$conf['install_profile'] = 'dkan';

// IMPORTANT. Local settings include comes first so we can fake acquia env variables on local.
// See settings.local.demo.php for how to setup your local environment.
if (!(function_exists('acquia_hosting_db_choose_active') || file_exists('/var/www/site-php'))) {
  $settings_local = DRUPAL_ROOT . '/' . conf_path() . '/settings.local.php';
  if (file_exists($settings_local)) {
    include $settings_local;
  }
}

// Setup standard environments using devinci
require DRUPAL_ROOT . "/sites/all/modules/contrib/devinci/devinci.environments.inc";
devinci_set_env();

/**
 * Setup standard settings for all environments.
 */
$conf['install_profile'] = 'dkan';


/**
 * Add Additional Custom Settings Below This Line.
 *
 * For environment switching, use environment.module and hook_environment_switch().
 */
switch(ENVIRONMENT) {

  case 'local':
    $conf['features_master_temp_enabled_modules'] = array(
      'maillog',
      'dblog',
      'devel',
      'views_ui',
      'acquia_purge',
      'acquia_solr'
    );
    $conf['features_master_temp_disabled_modules'] = array(
      'acquia_purge',
      'acquia_solr'
    );
    break;

  case 'dev':
    $conf['features_master_temp_enabled_modules'] = array(
      'maillog',
      'dblog',
      'devel',
    );
    break;

  case 'test':
    $conf['features_master_temp_enabled_modules'] = array(
      'maillog',
    );
    break;

  case 'live':
    break;

  default:
    print ("ENVIRONMENT set to " . ENVIRONMENT . ", but not mapped in settings.php");
    throw new Exception("ENVIRONMENT set to " . ENVIRONMENT . ", but not mapped in settings.php");
}

/**
 * Function called by devinci when the environment switches if defined.
 */
function devinci_custom_environment_switch($target_env, $current_env) {

  switch($target_env) {

    case 'local':
      break;

    case 'dev':
      break;

    case 'test':
      break;

    case 'live':
      break;
  }
}
