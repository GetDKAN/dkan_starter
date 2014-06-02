<?php
/**
 * This settings.php file was created by the Acquia Cloud ah-site-archive-import
 * Drush command. The imported archive did not contain a settings.php file, so
 * the import process created this file by default. You can replace this file
 * with the standard default settings.php file for your version of Drupal.
 * However, be sure to keep the last line that loads the "Acquia Cloud settings
 * include file", which provides the correct database credentials for your site.
 */
$update_free_access = FALSE;
$drupal_hash_salt = '';
ini_set('arg_separator.output',     '&amp;');
ini_set('magic_quotes_runtime',     0);
ini_set('magic_quotes_sybase',      0);
ini_set('session.cache_expire',     200000);
ini_set('session.cache_limiter',    'none');
ini_set('session.cookie_lifetime',  2000000);
ini_set('session.gc_divisor',       100);
ini_set('session.gc_maxlifetime',   200000);
ini_set('session.gc_probability',   1);
ini_set('session.save_handler',     'user');
ini_set('session.use_cookies',      1);
ini_set('session.use_only_cookies', 1);
ini_set('session.use_trans_sid',    0);
ini_set('url_rewriter.tags',        '');

$conf['shield_pass'] = 'dkanusaid';
$conf['shield_user'] = 'admin';
$conf['shield_print'] = '';

// On Acquia Cloud, this include file configures Drupal to use the correct
// database in each site environment (Dev, Stage, or Prod). To use this
// settings.php for development on your local workstation, set $db_url
// (Drupal 5 or 6) or $databases (Drupal 7) as described in comments above.
if (file_exists('/var/www/site-php')) {
  require('/var/www/site-php/usaiddemo/usaiddemo-settings.inc');
}

if (isset($_ENV['AH_SITE_ENVIRONMENT'])) {
  $search_api_server_name = 'acquia';
  switch ($_ENV['AH_SITE_ENVIRONMENT']) {
    case 'dev':
      $conf['search_api_acquia_overrides'][$search_api_server_name] = array(
        'path' => '/solr/DIJX-51605',
        'host' => 'useast1-c6.acquia-search.com',
      );
      break;

    case 'test':
      $conf['search_api_acquia_overrides'][$search_api_server_name] = array(
        'path' => '/solr/DIJX-51605',
        'host' => 'useast1-c6.acquia-search.com',
      );
      break;

    case 'prod':
      $conf['search_api_acquia_overrides'][$search_api_server_name] = array(
        'path' => '/solr/DIJX-51605',
        'host' => 'useast1-c6.acquia-search.com',
      );
      break;
  }
}
elseif (!(function_exists('acquia_hosting_db_choose_active') || file_exists('/var/www/site-php'))) {
  $settings_local = DRUPAL_ROOT . '/' . conf_path() . '/settings.local.php';
  if (file_exists($settings_local)) {
    include $settings_local;
  }
}
