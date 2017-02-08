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
  $base_url_https = str_replace('http://', 'https://', $base_url);

  if ($conf['default']['https_securepages'] && !$conf['default']['https_everywhere']) {
    $conf['securepages_basepath'] = $base_url;
    $conf['securepages_basepath_ssl'] = $base_url_https;
    $conf['securepages_enable'] = 1;
    $conf['securepages_forms'] = "user_login\r\nuser_login_block";
    $conf['securepages_ignore'] = "";
    $conf['securepages_pages'] =  "node/add*\r\nnode/*/edit\r\nnode/*/delete\r\nuser\r\nuser/*\r\nadmin\r\nadmin/*\r\npanels\r\npanels*";
    $conf['securepages_secure'] = "1";
    $conf['securepages_switch'] = "0";
    $conf['securepages_debug'] = "0";
    // Enables for "Authenticate users" role which should always be "2".
    $conf['securepages_roles'] = array("2" => "2");
  }
  else {
    $conf['securepages_enable'] = "0";
  }

  if ($conf['default']['https_everywhere']) {
    $base_url = $base_url_https;
    $conf['acquia_purge_https'] = TRUE;
    $conf['acquia_purge_http'] = FALSE;

    // Disable securepages when https everywhere is enabled.
    $conf['features_master_temp_disabled_modules'][] = 'securepages';
  }

  if (isset($conf['acquia'][$env]['core_id'])) {
    $search_server_id = $conf['acquia']['search']['id'];
    $conf['search_api_acquia_overrides'][$search_server_id] = array(
      'path' => '/solr/' . $conf['acquia'][$env]['core_id'],
      'host' => $conf['acquia']['search']['host'],
      'derived_key' => $conf['acquia'][$env]['derived_key'],
    );
  }
  acquia_hosting_db_choose_active();
}
