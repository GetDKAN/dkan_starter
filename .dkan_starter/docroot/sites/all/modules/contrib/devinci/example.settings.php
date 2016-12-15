<?php
/**
 * Example settings.php
 *
 * To fully leverage environment switching with the devinci module, you should
 * copy this file into your settings.php file and edit settings as appropriate.
 * This file should be able to replace the one created by Acquia or pantheon.
 */

/**
 * Load a local settings.php file if one exists.
 *  - It should declare the ENVIRONMENT constant to be 'local' by using:
 *    define('ENVIRONMENT', 'local');
 *  - It should also set the $database settings for your local environment.
 *  - .gitignore should probably be set to ignore this file so it doesn't get
 *    comitted by accident.
 *  - You can also create an environment.settings.php file that devinci will use
 *    to configure more complex environment setups, or with custom hosting
 *    companies beside acquia and pantheon.
 */
$settings_local = DRUPAL_ROOT . '/' . conf_path() . '/settings.local.php';
if (file_exists($settings_local)) {
  include $settings_local;
}

/******************************************************
 * REQUIRED: Setup standard environments using devinci.
 ******************************************************/
require DRUPAL_ROOT . "/sites/all/modules/contrib/devinci/devinci.environments.inc";
// Note that you can define your own custom env mappings if you
// have additional/custom environments on acquia for instance, then
// pass the map array to devinci_set_env($env_map).
devinci_set_env();

/********************************************************
 * OPTIONAL: Setup default settings for ALL environments.
 ********************************************************/

// Don't show any errors.
$conf['error_level'] = ERROR_REPORTING_HIDE;
error_reporting(0);
ini_set("display_errors", 0);

//Ensure we don't send emails by default.
$conf['mail_system'] = array (
  'default-system' => 'MaillogMailSystem',
  'maillog' => 'MaillogMailSystem',
);
$conf['maillog_send'] = 0;

// Disable all caching
$conf['page_cache_maximum_age'] = 0;
$conf['cache'] = 0;
$conf['preprocess_js'] = 0;
$conf['preprocess_css'] = 0;
$conf['cache_lifetime'] = 0;

// Disable zip compression.
$conf['page_compression'] = 0;
$conf['css_gzip_compression'] = 0;
$conf['js_gzip_compression'] = 0;

// Enable Fast 404 settings (from default.settings.php).
$conf['404_fast_paths_exclude'] = '/\/(?:styles)\//';
$conf['404_fast_paths'] = '/\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
$conf['404_fast_html'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL "@path" was not found on this server.</p></body></html>';

// Disable drupal being able to update itself.
$conf['allow_authorize_operations'] = FALSE;
$update_free_access = FALSE;

// Garbage collector settings from default.settings.php
ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);
ini_set('session.gc_maxlifetime', 200000);
ini_set('session.cookie_lifetime', 2000000);

// Disable git support for the environment indicator by default.
$conf['environment_indicator_git_support'] = FALSE;

// Assume using a specific install profile (simplify installation).
//$conf['install_profile'] = 'minimal';

// This should be updated to the actual live site url if using stage_file_proxy.
//$conf['stage_file_proxy_origin'] = 'http://my-live-site.com';

/******************************************************
 * OPTIONAL: Override default settings per environment.
 ******************************************************/
switch(ENVIRONMENT) {

  /**
   * Local Environment
   */
  case 'local':
    // Features Master module supoorts temporarily enabling modules.
    // This will add modules in the local environment, but EXCLUDE them
    // from being exported using features_master.
    $conf['features_master_temp_enabled_modules'] = array(
      'dblog',
      'devel',
      'field_ui',
      'maillog',
      'stage_file_proxy',
      'views_ui',
    );
    // Features Master also supports temporarily disabling modules.
    // This will disable modules in the local environment, but INCLUDE them
    // when exporting using features_master.
    $conf['features_master_temp_disabled_modules'] = array(
      'syslog',
    );
    // Show ALL errors when working locally.
    $conf['error_level'] = ERROR_REPORTING_DISPLAY_ALL;
    ini_set("display_errors", 1);

    // Enable environment indicator to show current git branch.
    // Note: git must be available, which it's not on acquia.
    $conf['environment_indicator_git_support'] = TRUE;
    break;

  /**
   * Development Environment
   */
  case 'development':
    $conf['features_master_temp_enabled_modules'] = array(
      'dblog',
      'devel',
      'field_ui',
      'maillog',
      'stage_file_proxy',
      'views_ui',
    );
    // Enable git support for the environment indicator to show current branch.
    $conf['environment_indicator_git_support'] = TRUE;
    break;

  /**
   * Test Environment
   */
  case 'test':
    $conf['features_master_temp_enabled_modules'] = array(
      'maillog',
      'stage_file_proxy',
    );
    $conf['error_level'] = ERROR_REPORTING_HIDE;

    // Enable caching like in production.
    $conf['page_cache_maximum_age'] = 900;
    $conf['cache'] = 1;
    $conf['preprocess_js'] = 1;
    $conf['preprocess_css'] = 1;
    // Enable git support for the environment indicator to show current branch.
    $conf['environment_indicator_git_support'] = TRUE;
    break;

  /**
   * Production Environment
   */
  case 'production':
    // Enable the ability to send emails - via core mail in this case,
    // but it coulbe be update to use SMTP or mail API.
    $conf['mail_system'] = array (
      'default-system' => 'DefaultMailSystem',
    );
    // Enable caching for production.
    // 15 minutes max page cache time.
    $conf['page_cache_maximum_age'] = 900;
    $conf['cache'] = 1;
    $conf['preprocess_js'] = 1;
    $conf['preprocess_css'] = 1;
    break;

  default:
    // For safety, if no environment matches: exit with a warning message.
    print ("ENVIRONMENT set to " . ENVIRONMENT . ", but not mapped in settings.php");
    exit().
}

/*******************************************
 * OPTIONAL: Perform tasks when swtiching environments.
 *******************************************/
/* For environment switching to work, ensure environment.module is enabled and
 * use either hook_environment_switch() in a custom module, or simply define
 * devinci_custom_environment_switch() in settings.php as shown below.
 */
function devinci_custom_environment_switch($target_env, $current_env) {

  switch($target_env) {

    case 'local':
      // Example: Clear all caches. (drush cc all)
      drupal_flush_all_caches();
      // Example: Revert a features_master module, which is assumed to be called
      // 'custom_config'. Update to the name of your master module. This saves
      // the step of manually reverting when switching environments.
      features_master_features_revert('custom_config');
      break;

    case 'development':
      drupal_flush_all_caches();
      features_master_features_revert('custom_config');
      break;

    case 'test':
      drupal_flush_all_caches();
      features_master_features_revert('custom_config');
      break;

    case 'production':
      drupal_flush_all_caches();
      features_master_features_revert('custom_config');
      break;
  }
}
