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
  $sitegroup = getenv('AH_SITE_GROUP');

  $base_url = $conf['acquia'][$env]['base_url'];
  $conf['securepages_basepath'] = $base_url;
  $conf['securepages_basepath_ssl'] = str_replace('http://', 'https://', $base_url);

  if (!$conf['https_everywhere']) {
    $conf['https'] = FALSE;
    $conf['securepages_enable'] = FALSE;
  }

  if (isset($conf['acquia'][$env]['core_id'])) {
    $conf['search_api_acquia_overrides']['acquia_search'] = array(
      'path' => '/solr/' . $conf['acquia'][$env]['core_id'],
      'host' => $conf['acquia']['search']['host'],
      'derived_key' => $conf['acquia'][$env]['derived_key']
    );
  }
  // New relic settings per enviroment.
  if (extension_loaded('newrelic')) {
    switch ($env) {
      case 'dev':
      case 'test':
      case 'prod':
        newrelic_set_appname("$sitegroup.$env", '', 'true');
        break;
    }
  }
}
