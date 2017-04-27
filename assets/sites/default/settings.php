<?php

/**
 * @file
 * Main settings.php.
 */

/**
 * Load a local settings.php file if one exists.
 *
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
elseif (file_exists($settings_docker)) {
  include $settings_docker;
}

/**
 * Include config file from config folder.
 */
$config_file = DRUPAL_ROOT . '/../config/config.php';
if (file_exists($config_file)) {
  include $config_file;
}

/**
 * Validation function to check if variable is available.
 */
function _data_starter_validates($variable = '') {
  global $conf;
  $validates = isset($conf['default'][$variable]) && $conf['default'][$variable] != 'changeme';
  return $validates;
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
define("CI", getenv('CI'));
devinci_set_env($env_map);

/********************************************************
 * OPTIONAL: Setup default settings for ALL environments.
 ********************************************************/
// Init 'features_master_temp_enabled_modules' and
// 'features_master_temp_disabled_modules'.
$conf['features_master_temp_enabled_modules'] = array();
$conf['features_master_temp_disabled_modules'] = array();

// Use the executable scan method for ClamAV by default (Daemon mode can cause
// some problems)
$conf['clamav_mode'] = 1;

// Adds support for fast file if enabled in config.yml.
if (isset($conf['default']['fast_file']) && $conf['default']['fast_file']['enable']) {
  if (!CI) {
    $conf['dkan_datastore_fast_import_selection'] = 2;
    $conf['dkan_datastore_fast_import_selection_threshold'] = $conf['default']['fast_file']['limit'];
    $conf['dkan_datastore_load_data_type'] = 'load_data_local_infile';
    $conf['queue_filesize_threshold'] = $conf['default']['fast_file']['queue'];
    $conf['dkan_datastore_class'] = 'DkanDatastoreFastImport';
  }

  $databases['default']['default']['pdo'] = array(
    PDO::MYSQL_ATTR_LOCAL_INFILE => 1,
    PDO::MYSQL_ATTR_USE_BUFFERED_QUERY => 1,
  );
}
else {
  if (!CI) {
    $conf['dkan_datastore_fast_import_selection'] = 0;
  }
  else {
    // Set PDO values for CI environment and avoid setting
    // up the fast import selection option.
    $databases['default']['default']['pdo'] = array(
      PDO::MYSQL_ATTR_LOCAL_INFILE => 1,
      PDO::MYSQL_ATTR_USE_BUFFERED_QUERY => 1,
    );
  }
}

// Don't show any errors.
$conf['error_level'] = ERROR_REPORTING_HIDE;
ini_set('error_reporting', E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED);
ini_set("display_errors", 0);

// Ensure we don't send emails by default.
$conf['mail_system'] = array(
  'default-system' => 'MaillogMailSystem',
  'maillog' => 'MaillogMailSystem',
);
$conf['maillog_send'] = 0;

// Disable all caching.
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

// Garbage collector settings from default.settings.php.
ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 100);
ini_set('session.gc_maxlifetime', 200000);
ini_set('session.cookie_lifetime', 2000000);

// Disable cron. We run this from Jenkins.
// Except for CircleCI or test purpose.
if (CI) {
  $conf['cron_safe_threshold'] = 0;
}

// Disable git support for the environment indicator by default.
$conf['environment_indicator_git_support'] = FALSE;

// Assume using a specific install profile (simplify installation).
$conf['install_profile'] = 'dkan';

// This should be updated to the actual live site url if using stage_file_proxy.
if (_data_starter_validates('stage_file_proxy_origin')) {
  $conf['stage_file_proxy_origin'] = $conf['default']['stage_file_proxy_origin'];
}

// KEY for dkan health status.
$conf['dkan_health_status_health_api_access_key'] = 'DKAN_HEALTH';

// Never disallow cli access via shield config.
$conf['shield_allow_cli'] = 1;

/******************************************************
 * OPTIONAL: Override default settings per environment.
 ******************************************************/

/**
 * Settings that are shared between environments.
 */
switch (ENVIRONMENT) {
  case 'local':
    if (_data_starter_validates('stage_file_proxy_origin')) {
      if ($conf['default']['stage_file_proxy']) {
        $conf['features_master_temp_enabled_modules'] = array_merge(
          $conf['features_master_temp_enabled_modules'],
          array(
            'stage_file_proxy',
          ));
      }
    }

    // Features Master also supports temporarily disabling modules.
    // This will disable modules in the local environment, but INCLUDE them
    // when exporting using features_master.
    $conf['features_master_temp_disabled_modules'] = array_merge(
      $conf['features_master_temp_disabled_modules'],
      array(
        'acquia_agent',
        'acquia_purge',
        'expire_panels',
        'dkan_acquia_expire',
        'dkan_acquia_search_solr',
        'expire',
        'search_api_solr',
        'search_api_acquia',
        'securepages',
        'syslog',
        'shield',
      ));

    $conf['error_level'] = ERROR_REPORTING_DISPLAY_ALL;
    ini_set("display_errors", 1);

  case 'development':
    $conf['features_master_temp_enabled_modules'] = array_merge(
        $conf['features_master_temp_enabled_modules'],
        array(
          'dblog',
          'devel',
          'field_ui',
          'views_ui',
        ));

  case 'test':
    $conf['features_master_temp_enabled_modules'] = array_merge(
        $conf['features_master_temp_enabled_modules'],
        array(
          'maillog',
        ));

    $conf['features_master_temp_disabled_modules'] = array_merge(
        $conf['features_master_temp_disabled_modules'],
        array(
          // Only send Acquia Insite scores from prod.
          'acquia_spi',
          'googleanalytics',
          'google_tag',
        ));

    // Enable git support for the environment indicator to show current branch.
    $conf['environment_indicator_git_support'] = TRUE;
    // Make sure the default timezone is set to UTC for testing.
    $conf['date_default_timezone'] = 'UTC';

  case 'production':
    if (ENVIRONMENT == "test" || ENVIRONMENT == "production") {
      // Enable caching for test and production.
      // 6 hours cache time.
      $conf['page_cache_maximum_age'] = 21600;
      $conf['page_compression'] = 1;
      $conf['cache'] = 1;
      $conf['preprocess_js'] = 1;
      $conf['preprocess_css'] = 1;
    }

    $conf['ape_alternative_lifetime'] = 300;
    $conf['ape_alternatives'] = <<<EOT
search
search*
EOT;
    $conf['pathauto_update_action'] = 1;
    break;

  default:
    // For safety, if no environment matches: exit with a warning message.
    print ("ENVIRONMENT set to " . ENVIRONMENT . ", but not mapped in settings.php");
    exit();
}

/**
 * Settings that are individual to environments.
 */
if (ENVIRONMENT == "production") {
  // Enable the ability to send emails - via core mail in this case,
  // but it coulbe be update to use SMTP or mail API.
  $conf['mail_system'] = array(
    'default-system' => 'DefaultMailSystem',
  );

  // Add tracking codes for Google Analytics.
  if (isset($conf['gaClientTrackingCode']) && $conf['gaClientTrackingCode'] != 'UA-XXXXX-Y') {
    $conf['googleanalytics_account'] = $conf['gaClientTrackingCode'];
  }
  elseif (isset($conf['gaNuCivicTrackingCode']) && $conf['gaNuCivicTrackingCode'] != 'UA-XXXXX-Z') {
    $conf['googleanalytics_account'] = $conf['gaNuCivicTrackingCode'];
  }

  if (isset($conf['gaNuCivicTrackingCode']) &&
    $conf['googleanalytics_account'] != $conf['gaNuCivicTrackingCode'] &&
    $conf['gaNuCivicTrackingCode'] != 'UA-XXXXX-Z') {
    $conf['googleanalytics_codesnippet_after'] = "ga('create', '" . $conf['gaNuCivicTrackingCode'] . "', 'auto', 'nucivicTracker');ga('nucivicTracker.send', 'pageview');";
  }
}
// Disable dkan_worflow modules so that dkan tests pass
// See: https://jira.govdelivery.com/browse/CIVIC-5128
if (getenv('CI') == "true") {
  // TODO: change clamav feature tests so that they enable clamav.
  $conf['features_master_temp_enabled_modules'][] = 'clamav';

  $conf['features_master_temp_disabled_modules'][] = 'dkan_workflow';
  $conf['features_master_temp_disabled_modules'][] = 'dkan_workflow_permissions';
  $conf['features_master_temp_disabled_modules'][] = 'link_badges';
  $conf['features_master_temp_disabled_modules'][] = 'menu_badges';
  $conf['features_master_temp_disabled_modules'][] = 'views_dkan_workflow_tree';
  $conf['features_master_temp_disabled_modules'][] = 'workbench';
  $conf['features_master_temp_disabled_modules'][] = 'workbench_email';
  $conf['features_master_temp_disabled_modules'][] = 'workbench_moderation';
  $conf['features_master_temp_disabled_modules'][] = 'drafty';
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
