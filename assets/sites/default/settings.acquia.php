<?php
/**
 * Acquia Settings.
 */
 
if (isset($conf['memcache_servers'])) {
  $conf['cache_backends'][] = './sites/all/modules/contrib/memcache/memcache.inc';
  $conf['cache_default_class'] = 'MemCacheDrupal';
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
}

if (isset($_ENV['AH_SITE_ENVIRONMENT'])) {
  // Set 2 session cookies, one secure and one not.
  $conf['https'] = FALSE;
  // Enable https redirection.
  $conf['securepages_enable'] = FALSE;
  $sitegroup = getenv('AH_SITE_GROUP');
  $env = getenv(['AH_SITE_ENVIRONMENT']);
  switch ($env) {
    case 'dev':
    case 'test':
      $env = $env == 'test' ? 'stg': $env;
      $base_url="http://$sitegroup.$env.prod.acquia-sites.com";
      $conf['securepages_basepath'] = 'http://$sitegroup.$env.prod.acquia-sites.com';
      $conf['securepages_basepath_ssl'] = 'https://$sitegroup.$env.prod.acquia-sites.com';
      break;
    case 'prod':
      $base_url="http://$sitegroup.$env.prod.acquia-sites.com";
      $conf['securepages_basepath'] = 'http://$sitegroup.prod.acquia-sites.com';
      $conf['securepages_basepath_ssl'] = 'https://$sitegroup.prod.acquia-sites.com';
      break;
  }

  // New relic settings per enviroment.
  if (extension_loaded('newrelic')) {
    switch ($env) {
      case 'dev':
      case 'test':
      case 'prod':
        $app_name = '$sitegroup.$env';
        newrelic_set_appname($app_name, '', 'true');
        break;
    }
  }
}
