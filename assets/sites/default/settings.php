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
$settings_docker = DRUPAL_ROOT . '/' . conf_path() . '/settings.docker.php';
if (file_exists($settings_local)) {
  include $settings_local;
}
else if (file_exists($settings_docker)) {
  include $settings_docker;
}

/**
 * Include config file from config folder.
 */
$config_file = DRUPAL_ROOT . '/../config/config.php';
if (file_exists($config_file)) {
  include $config_file;
}

// Needs to happen before Acquia connects.
// TODO: Move to our version of devinci.
if (file_exists('/var/www/site-php')) {
  $conf['acquia_hosting_settings_autoconnect'] = FALSE;
}

/******************************************************
 * REQUIRED: Setup standard environments using devinci.
 ******************************************************/
require DRUPAL_ROOT . "/sites/all/modules/contrib/devinci/devinci.environments.inc";
// Note that you can define your own custom env mappings if you
// have additional/custom environments on acquia for instance, then
// pass the map array to devinci_set_env($env_map).
$env_map = array(
  'local' => 'local',
  'dev' => 'development',
  'test' => 'test',
  'live' => 'production',
  'prod' => 'production',
  'ra' => 'production',
);
devinci_set_env($env_map);

/********************************************************
 * OPTIONAL: Setup default settings for ALL environments.
 ********************************************************/

// Adds support for fast file if enabled in config.yml.
if (isset($conf['default']['fast_file']) && $conf['default']['fast_file']['enable']) {
  $conf['dkan_datastore_fast_import_selection'] = 2;
  $conf['dkan_datastore_fast_import_selection_threshold'] = $conf['default']['fast_file']['limit'];
  $conf['dkan_datastore_load_data_type'] = 'load_data_local_infile';
  $conf['queue_filesize_threshold'] = $conf['default']['fast_file']['queue'];

  $databases['default']['default']['pdo'] = array(
    PDO::MYSQL_ATTR_LOCAL_INFILE => 1,
    PDO::MYSQL_ATTR_USE_BUFFERED_QUERY => 1,
  );
}
else {
  $conf['dkan_datastore_fast_import_selection'] = 0;
}

// Don't show any errors.
$conf['error_level'] = ERROR_REPORTING_HIDE;
ini_set('error_reporting', E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED);
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
$conf['install_profile'] = 'dkan';

// This should be updated to the actual live site url if using stage_file_proxy.
//$conf['stage_file_proxy_origin'] = 'http://my-live-site.com';

// KEY for dkan health status
$conf['dkan_health_status_health_api_access_key'] = 'DKAN_HEALTH';

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
      // This is temporary, there's a recline dependency on field_ui that
      // needs to be removed. See https://github.com/NuCivic/recline/pull/34
      // When the above is merged, unconment the following line and update
      // the custom_config feature master list.
      // 'field_ui',
      'maillog',
      // 'stage_file_proxy',
      'views_ui',
    );
    // Features Master also supports temporarily disabling modules.
    // This will disable modules in the local environment, but INCLUDE them
    // when exporting using features_master.
     $conf['features_master_temp_disabled_modules'] = array(
      'acquia_agent',
      'acquia_spi',
      'acquia_purge',
      'dkan_acquia_expire',
      'dkan_acquia_search_solr',
      'expire',
      'search_api_solr',
      'search_api_acquia',
      'securepages',
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
      // 'stage_file_proxy',
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
      // 'stage_file_proxy',
    );
    $conf['error_level'] = ERROR_REPORTING_HIDE;

    // Enable caching like in production.
    $conf['page_cache_maximum_age'] = 21600;
    $conf['ape_alternative_lifetime'] = 300;
    $conf['ape_alternatives'] = "search
    search*";
    $conf['page_compression'] = 1;
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
    // 6 hours cache time.
    $conf['page_cache_maximum_age'] = 21600;
    $conf['ape_alternative_lifetime'] = 300;
    $conf['ape_alternatives'] = "search
search*";
    $conf['page_compression'] = 1;
    $conf['cache'] = 1;
    $conf['preprocess_js'] = 1;
    $conf['preprocess_css'] = 1;
    $conf['pathauto_update_action'] = 1;
    break;

  default:
    // For safety, if no environment matches: exit with a warning message.
    print ("ENVIRONMENT set to " . ENVIRONMENT . ", but not mapped in settings.php");
    exit();
}

// Fake the 'derived_key' used to connect to Solr, if we can't find the
// Acquia-set "AH_PRODUCTION" environment variable.
// This will cause all requests to Acquia Search instances respond with 403.
if (!isset($_ENV["AH_SITE_ENVIRONMENT"])) {

  // EDIT THE NEXT LINE TO MATCH your Search API "server" machinename.
  $search_api_server_machine_name = 'dkan_acquia_solr';

  $conf['search_api_acquia_overrides'][$search_api_server_machine_name] = array(
      #'path' => '/solr/[core_ID]',
      #'host' => '[hostname].acquia-search.com',
      'derived_key' => 'FAKE',
  );
}

/****************************
 * OPTIONAL: Acquia Settings.
 ***************************/
/* This are acquia specific settings 
 */
include "settings.acquia.php";

/*****************************
 * OPTIONAL: Custom Settings.
 ****************************/
/* This are custom site settings 
 */
include "settings.custom.php";
