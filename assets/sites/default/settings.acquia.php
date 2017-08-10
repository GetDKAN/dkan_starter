<?php

/**
 * @file
 * Acquia Settings.
 */

if (isset($_ENV['AH_SITE_ENVIRONMENT'])) {
  if (isset($conf['memcache_servers'])) {
    $conf['cache_backends'][] = './sites/all/modules/contrib/memcache/memcache.inc';
    $conf['cache_default_class'] = 'MemCacheDrupal';
    $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
  }

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
    $conf['securepages_pages'] = "node/add*\r\nnode/*/edit\r\nnode/*/delete\r\nuser\r\nuser/*\r\nadmin\r\nadmin/*\r\npanels\r\npanels*";
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

  // Conditionally manage memory.
  $high_memory_paths = array();
  // Things that break in ODFE.
  if (isset($conf['default']['odfe']) && $conf['default']['odfe']['enabled']) {
    $high_memory_paths += array(
      'admin',
      'node/add',
      'node/%node/edit',
      'node/%node/moderation',
      'file/ajax',
      'api/action/datastore/search.json',
    );
  }
  
  // ODSM edit forms.
  $high_memory_paths[] = 'admin/config/services/odsm/edit';

  // Standarize node edit paths for validation.
  $current_path = preg_replace("/\/\d+/", '/%node', $_GET['q']);
  foreach ($high_memory_paths as $high_memory_path) {
    if ((strpos($current_path, $high_memory_path) === 0)) {
      ini_set('memory_limit', '512M');
    }
  }

  acquia_hosting_db_choose_active();
}
else {
  // Fake the 'derived_key' used to connect to Solr, if we can't find the
  // Acquia-set "AH_PRODUCTION" environment variable.
  // This will cause all requests to Acquia Search instances respond with 403.
  if (
    isset($conf['acquia']) &&
    isset($conf['acquia']['search']) &&
    isset($conf['acquia']['search']['id'])
  ) {

    $search_api_server_machine_name = $conf['acquia']['search']['id'];
  }
  else {
    $search_api_server_machine_name = 'dkan_acquia_solr';
  }

  $conf['search_api_acquia_overrides'][$search_api_server_machine_name] = array(
    // 'path' => '/solr/[core_ID]',
    // 'host' => '[hostname].acquia-search.com',.
    'derived_key' => 'FAKE',
  );
}
