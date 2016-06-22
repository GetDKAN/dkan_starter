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

  if ($conf['acquia']['subscription'] == 'enterprise') {
    switch ($env) {
    case 'dev':
    case 'test':
      $env = $env == 'test' ? 'stg': $env;
      $base_url="http://$sitegroup$env.prod.acquia-sites.com";
      $conf['securepages_basepath'] = 'http://$sitegroup$env.prod.acquia-sites.com';
      $conf['securepages_basepath_ssl'] = 'https://$sitegroup$env.prod.acquia-sites.com';
      break;
    case 'prod':
      break;
    }
  }

  if (!$conf['https_everywhere']) {
    $conf['https'] = FALSE;
    $conf['securepages_enable'] = FALSE;
    $conf['securepages_basepath'] = $base_url;
    $conf['securepages_basepath_ssl'] = str_replace('http://', 'https://', $base_url);
  }

  switch ($_ENV['AH_SITE_ENVIRONMENT']) {
    case 'prod':
      $core_id = $conf['acquia']['prod']['core_id'];
      $derived_key = $conf['acquia']['prod']['derived_key'];
      break;
    case 'test':
      $core_id = $conf['acquia']['test']['core_id'];
      $derived_key = $conf['acquia']['test']['derived_key'];
      break;
    case 'ra':
      $core_id = $conf['acquia']['test']['core_id'];
      $derived_key = $conf['acquia']['test']['derived_key'];
      break;
    case 'dev':
    default:
      $core_id = $conf['acquia']['dev']['core_id'];
      $derived_key = $conf['acquia']['dev']['derived_key'];
      break;
  }
  $host = $conf['acquia']['search']['host'];

  $conf['search_api_acquia_overrides']['test'] = array(
    'path' => '/solr/' . $core_id,
    'host' => $host,
    'derived_key' => $derived_key
  );
  $conf['search_api_acquia_overrides']['dkan_acquia_solr'] = array(
    'path' => '/solr/' . $core_id,
    'host' => $host,
    'derived_key' => $derived_key
  );
  
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
