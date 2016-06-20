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
  $env = getenv('AH_SITE_ENVIRONMENT');
  // There's no way to predict the urls anymore. Replace with actual urls.
  switch ($env) {
    case 'dev':
      $base_url = $conf['acquia']['dev']['base_url'];
      break;
    case 'test':
      $base_url = $conf['acquia']['test']['base_url'];
      break;
    case 'prod':
      $base_url = $conf['acquia']['prod']['base_url'];
      break;
  }

  $conf['https'] = FALSE;
  $conf['securepages_enable'] = FALSE;
  $conf['securepages_basepath'] = $base_url;
  $conf['securepages_basepath_ssl'] = str_replace('http://', 'https://', $base_url);
  
  $sitegroup = getenv('AH_SITE_GROUP');
  
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
