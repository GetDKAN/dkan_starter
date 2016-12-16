<?php

/**
 * @file
 * Self-test diagnostic test functions for _acquia_purge_get_diagnosis().
 */

/**
 * Test whether the current hosting environment is Acquia Cloud.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_acquiacloud($t) {
  $site = _acquia_purge_get_site_name();
  $test = array('title' => $t('Hosting environment'));
  if (_acquia_purge_are_we_on_acquiacloud()) {
    $test['value'] = $t('Acquia Cloud (@site)', array('@site' => $site));
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_OK;
  }
  else {
    $test['value'] = $t('Third-party');
    $test['description'] = $t('You are not running on Acquia Cloud, this is a'
      . ' mandatory requirement for this module. The module will not process'
      . ' scheduled purges, once deployed scheduled purges will be processed.');
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
  }
  return $test;
}

/**
 * Test the amount of load balancers configured.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_balancers($t) {
  $balancers = _acquia_purge_get_balancers();
  $balancers_c = count($balancers);
  $test = array(
    'value' => implode(', ', $balancers),
    'title' => $t('Load balancers'),
  );

  // Determine whether the amount of load balancers is healthy.
  if (!$balancers_c) {
    $test['value'] = $t('No load balancers detected.');
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('There are no load balancers detected which makes'
      . ' it impossible to purge your site. Please contact Acquia Support!');
  }
  elseif ($balancers_c < 2) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('You have only one load balancer, this means your'
      . ' site can not be failed over by us. Please contact Acquia Support.');
  }
  elseif ($balancers_c >= 6) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('Your website is running behind @no load'
      . ' balancers, which will put severe stress on your database. Please pay'
      . ' attention to the number of items in the queue table.',
      array('@no' => $balancers_c));
  }
  else {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_OK;
  }
  return $test;
}

/**
 * Test against known conflicting modules such as varnish, boost and purge.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_conflicts($t) {
  $test = array(
    'severity' => ACQUIA_PURGE_SEVLEVEL_OK,
    'value' => $t('None'),
    'title' => $t('Module conflicts'),
  );

  // Detect the existence of the Boost module which is incompatible.
  if (module_exists('boost')) {
    $test['value'] = 'boost';
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('Your site has the boost module enabled which is'
      . ' known to cause issues on Acquia Cloud. Because of its heavy'
      . ' interactions with the file system it will destabilize your site.');
  }

  // Detect the existence of the Purge module which is incompatible for now.
  if (module_exists('purge')) {
    $test['value'] = 'purge';
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('Your site has the Purge module enabled which'
      . ' is incompatible with this version of Acquia Purge. However, this'
      . ' module will depend on it in the future. Please, remove it for now!');
  }

  // Detect if the Varnish module is enabled, which isn't necessary.
  if (module_exists('varnish')) {
    $test['value'] = 'varnish';
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('Your site runs with the varnish module enabled,'
      . ' which is known to not work on Acquia Cloud. As Acquia Purge does its'
      . ' work already for you we strongly encourage you to remove it.');
  }

  return $test;
}

/**
 * Test whether the amount of domain names is healthy.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_domains($t) {
  $allriskmode = _acquia_purge_variable('acquia_purge_allriskmode');
  $domains_link = drupal_get_path('module', 'acquia_purge') . '/DOMAINS.txt';
  $domains_link = url($domains_link);
  $domains = _acquia_purge_get_domains();
  $domains_c = count($domains);
  $test = array(
    'value' => implode(', ', $domains),
    'title' => $t('Purged domains'),
    'description' => $t('The domains for which content gets cleared from your'
      . ' load balancers. Every domain name multiplies the purging work to be'
      . ' done, it is therefore important to <a href="!link" target="_blank">'
      . 'specify your domains</a> when the automatically detected list exceeds'
      . ' 4 domains or when it is incorrect.', array('!link' => $domains_link)),
  );

  // Asterisks in domain names doesn't work given the way purging is configured
  // on Acquia Cloud. To prevent people from silent failure, test for this.
  $wildcard_domains_found = FALSE;
  foreach ($domains as $domain) {
    if (strpos($domain, '*') !== FALSE) {
      $wildcard_domains_found = $domain;
      break;
    }
  }

  // Evaluate domain names configuration and kick off warnings or even a full
  // block when the situation asks for it.
  if (!$domains_c) {
    $test['value'] = $t('0 domains detected.');
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('No domains have been detected which implies that'
      . ' something is seriously wrong. Pending purges will not be processed.');
  }
  elseif ($wildcard_domains_found) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('The domain "@baddomain" has a asterisk in it,'
      . ' which implies that its subdomains require clearing. However, this is'
      . ' unfortunately not supported by Acquia Cloud and therefore will lead'
      . ' to purge failure. Please remove this domain from your configuration.',
      array('@baddomain' => $wildcard_domains_found));
  }
  elseif ((php_sapi_name() === 'cli') && in_array('default', $domains)) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('The domain name "default" has been found'
      . ' indicating that you are running via Drush. Either you will need to'
      . ' specify your domains or provide --uri for the right site.');
  }
  elseif ($domains_c <= 6) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_OK;
    unset($test['description']);
  }
  elseif (($domains_c > 6) && ($domains_c <= 8)) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('Because you have @no domain names there is a'
      . ' <b>high risk</b> that purging your site will put stress on your'
      . ' servers, it is <b>strongly recommended</b> to <a href="!link"'
      . ' target="_blank">specify your domains</a> to not exceed 6 domains.',
      array('!link' => $domains_link, '@no' => $domains_c));
  }
  elseif ($allriskmode) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('Because you have @no domain names there is a'
      . ' <b>high risk</b> that purging your site will put stress on your'
      . ' servers, it is <b>strongly recommended</b> to <a href="!link"'
      . ' target="_blank">specify your domains</a> to not exceed 6 domains.',
      array('!link' => $domains_link, '@no' => $domains_c));
  }
  else {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('Because you have @no domain names there is a'
      . ' <b>very high risk</b> that purging your site will put stress on your'
      . ' servers, it is <b>strongly recommended</b> to <a href="!link"'
      . ' target="_blank">specify your domains</a> to not exceed 6 domains. To'
      . ' prevent system failure, pending purges will not be processed!',
      array('!link' => $domains_link, '@no' => $domains_c));
  }

  return $test;
}

/**
 * Diagnose several of the various settings the expire module provides.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_expire($t) {
  $test = array('title' => $t('Cache expiration'));

  // If the module is disabled, Acquia Purge will still work as API, but warn!
  if (!module_exists('expire')) {
    $test['value'] = $t('disabled');
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('The expire module is disabled, which only makes'
      . ' sense if you want to use Acquia Purge as pure API (and have disabled'
      . ' its dependency in acquia_purge.info). If you want your content to be'
      . ' cleared when it changes, enable it back on again.');
    return $test;
  }

  // Check the 'expire_status' setting, which is dead essential for purging.
  $mode = variable_get('expire_status', EXPIRE_STATUS_DISABLED);
  $modes = array(
    EXPIRE_STATUS_DISABLED => t('disabled'),
    EXPIRE_STATUS_ENABLED_INTERNAL => t('internal'),
    EXPIRE_STATUS_ENABLED_EXTERNAL => t('delegated'),
  );

  // Test the configured mode and raise any issues.
  $test['value'] = isset($modes[$mode]) ? $modes[$mode] : 'n/a';
  if ((int) $mode === EXPIRE_STATUS_ENABLED_EXTERNAL) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_OK;
  }
  else {
    $cmd = 'php -r "print json_decode(' . EXPIRE_STATUS_ENABLED_EXTERNAL;
    $cmd .= ');"|drush vset --format=json expire_status -';
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('The expire module is operating in @mode detection'
      . ' mode, this means purges will not happen when your content changes.'
      . ' Unless you are using the Acquia Purge APIs, put it back with:'
      . " '!cmd'",
      array(
        '@int' => $mode,
        '@mode' => $modes[$mode],
        '!cmd' => $cmd,
      )
    );
    return $test;
  }

  // Check if 'expire_include_base_url' is set to TRUE, which is incompatible.
  if (variable_get('expire_include_base_url', EXPIRE_INCLUDE_BASE_URL)) {
    $test['value'] = $t('include base URL enabled');
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t("The variable 'expire_include_base_url' is set"
      . ' to TRUE and that is not compatible with Acquia Purge, please disable'
      . ' the "Include base URL in expires" setting!');
  }

  // Check if 'expire_debug' has any debugging enabled, which gets expensive.
  elseif (variable_get('expire_debug', EXPIRE_DEBUG_DISABLED)) {
    $test['value'] = $t('debugging enabled');
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t("The expire module's debugging mode is enabled"
      . ' which is highly discouraged! The logging efforts duplicates as'
      . ' Acquia Purge also logs heavily and this might cause instabilities');
  }

  return $test;
}

/**
 * Report on how our logging is configured.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_logging($t) {
  $test = array(
    'severity' => ACQUIA_PURGE_SEVLEVEL_OK,
    'value' => $t('successes and failures'),
    'title' => $t('Logging verbosity'),
  );
  if (_acquia_purge_variable('acquia_purge_log_success') !== TRUE) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_INFO;
    $test['value'] = $t('failures only');
  }
  return $test;
}

/**
 * Test if anonymous page caching is enabled.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_page_cache($t) {
  $cache = variable_get('cache', 0);
  $test = array(
    'severity' => ACQUIA_PURGE_SEVLEVEL_OK,
    'value' => $cache ? $t('Enabled') : $t('Disabled'),
    'title' => $t('Anonymous page cache'),
  );

  // See https://www.drupal.org/node/2330805
  if ((!$cache) && module_exists('authcache')) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_OK;
    $test['value'] = $t('Disabled because of Authcache');
  }
  elseif (!$cache) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('Your site has Drupal page caching disabled,'
      . ' which is of extreme importance to every website that gets traffic.'
      . ' Once enabled your load balancer will be instructed to start serving'
      . ' pages from its cache to offload traffic from your web servers. The'
      . ' more traffic served from your load balancer, the faster your'
      . ' site will be!');
  }
  return $test;
}

/**
 * Test if the page_cache_maximum_age setting is configured in a healthy way.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_page_cache_age($t) {
  $age = (int) variable_get('page_cache_maximum_age', 0);
  $test = array('title' => $t('Expiration of cached pages'));

  // Set the value label.
  $test['value'] = $t('Disabled');
  if ($age !== 0) {
    $test['value'] = array('@age' => round($age / 60 / 60, 2));
    $test['value'] = $t('@age hours', $test['value']);
  }

  // Determine what "find out more" link we can provide.
  $link = 'http://blog.merge.nl/20120118/how-does-caching-work-drupal';
  if (user_access('administer site configuration')) {
    $link = url('admin/config/development/performance');
  }

  // Users without a TTL configured won't put our module to much use.
  if ($age === 0) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('The "<a href="!link" target="_blank">expiration'
    . ' of cached pages</a>"-setting of your website is disabled, while we'
    . ' strongly recommend to put it to 6 hours or longer in combination with'
    . ' the Acquia Purge module. The setting determines how long external'
    . ' caches (like our load balancers) are instructed to keep a cached copy'
    . ' of content. With your current setting your load balancer is not being'
    . ' used and your web servers will likely be under constant stress,'
    . ' causing a slow site and  system resources being wasted.',
    array('!link' => $link));
  }

  // We have picked 6 hours as a safe zone to be in, everything above is better!
  elseif ($age < 21600) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('The "<a href="!link" target="_blank">expiration'
    . ' of cached pages</a>"-setting of your website is set to @age, while we'
    . ' strongly recommend to put it to 6 hours or longer in combination with'
    . ' the Acquia Purge module. The setting determines how long external'
    . ' caches (like our load balancers) are instructed to keep a cached copy'
    . ' of content. With your current configuration, your load balancer is not'
    . ' used effectively and your web servers will likely be under stress,'
    . ' causing a slow site and system resources being wasted.',
    array('!link' => $link, '@age' => $test['value']));
  }

  // Everything above 6 hours is good although longer is still better obviously.
  else {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_OK;
  }

  return $test;
}

/**
 * Report how processing of the queue occurs.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_processing_mode($t) {
  $test = array(
    'title' => $t('Processing mode(s)'),
    'value' => array($t('ajax')),
    'severity' => ACQUIA_PURGE_SEVLEVEL_OK,
  );

  // Add tests for lateruntime and cron processing modes, when enabled.
  if (_acquia_purge_variable('acquia_purge_lateruntime') === TRUE) {
    $test['value'][] = $t('lateruntime');
  }
  if (_acquia_purge_variable('acquia_purge_cron') === TRUE) {
    $test['value'][] = $t('cron');

    // Raise a warning when it was too long ago since cron ran.
    $cron_last = variable_get('cron_last');
    if (!is_numeric($cron_last)) {
      $cron_last = variable_get('install_time', 0);
    }
    if (REQUEST_TIME - $cron_last > 3600) {
      $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
      $test['description'] = $t('It has been longer than one hour since cron ran'
        . ' for the last time, which puts your site at risk. As Acquia Purge'
        . ' has cron processing enabled, it assumes processing of its queue.'
        . ' When this is not frequent enough, the queue can reach dangerous'
        . ' levels and eventually even lead to a safety shutdown.');
    }
  }

  $test['value'] = implode(', ', $test['value']);
  return $test;
}

/**
 * Report the processing capacity under the current configuration.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_processing_capacity($t) {
  $maxitems = _acquia_purge_get_capacity();
  $factor = _acquia_purge_get_capacity_factor();
  $test = array(
    'title' => $t('Processing capacity'),
    'severity' => ACQUIA_PURGE_SEVLEVEL_OK,
    'value' => $t('@items items per run, slowdown factor=@factorX',
      array('@items' => $maxitems, '@factor' => $factor)),
  );
  return $test;
}

/**
 * Test how http:// and https:// based purging are set up.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_protocol_schemes($t) {
  $schemes = _acquia_purge_get_protocol_schemes();
  $domains = _acquia_purge_get_domains();
  $test = array(
    'title' => $t('Purged schemes'),
    'value' => implode(', ', $schemes),
  );

  // Test if no protocol schemes are to be purged.
  if (in_array('ERROR', $schemes)) {
    $test['value'] = $t('None');
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('It seems as if you have put both '
    . ' <b>acquia_purge_http</b> and <b>acquia_purge_https</b> settings to'
    . ' <b>FALSE</b> and thus disabled, which leads to a situation'
    . ' that does not work. Enable either one of them for Acquia Purge'
    . ' to continue to operate.');
  }

  // Test for the best possible scenario, either http:// OR https:// enabled.
  elseif (count($schemes) === 1) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_OK;
  }

  // Test for the Molotov cocktail scenario, two schemes with >4 domains.
  elseif ((count($schemes) === 2) && (count($domains) > 4)) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('You are purging both <b>http://</b> as'
    . ' <b>https://</b> paths on your site with more than 4 domain names.'
    . ' Acquia Purge has self-shutdown to prevent worse from happening as'
    . ' two protocol schemes with this many domain names can likely bring'
    . ' down your website at some point. Its better to either only wipe'
    . ' https:// or by reducing unnecessary domain names, for instance'
    . ' bare domains that redirect. See README.txt and DOMAINS.txt for more'
    . ' information on tuning.');
  }

  // Test for the lesser dangerous cocktail, just two schemes.
  elseif (count($schemes) === 2) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('You are purging both <b>http://</b> as'
    . ' <b>https://</b> paths on your site, which can be dangerous. Especially'
    . ' if you have more than 1-2 domains as this will lead in the double'
    . ' amount of work that gets done. Keep a close eye on the queue and'
    . ' consider delivering all your traffic off https:// (by doing a full'
    . ' -site redirect) and to disable http:// purging, see README.txt.');
  }

  return $test;
}

/**
 * Report which queue backend is in use.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_queue_backend($t) {
  $test = array(
    'title' => $t('Queue backend'),
    'value' => t('Database'),
    'severity' => ACQUIA_PURGE_SEVLEVEL_OK,
  );
  if (_acquia_purge_variable('acquia_purge_smartqueue')) {
    $test['value'] = t('Smart queue (drops very old queue items)');
  }
  return $test;
}

/**
 * Test and report the current queue volume.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_queue_volume($t) {
  $allriskmode = _acquia_purge_variable('acquia_purge_allriskmode');
  $stats = _acquia_purge_service()->stats();
  $test = array('title' => $t('Queue volume'));

  // Define our warning and shutdown thresholds.
  $factor   = _acquia_purge_get_capacity_factor();
  $warning  = ceil(3000 / $factor);
  $shutdown = ceil(10000 / $factor);

  // Setup our $t() variables.
  $tvars = array(
    '@total' => $stats['remaining'],
    '@warning' => $warning,
    '@shutdown' => $shutdown,
  );
  $test['value'] = $t('@total (warning: @warning, shutdown: @shutdown)', $tvars);

  // Test the queue level for these thresholds and act appropriately.
  if ($stats['remaining'] < $warning) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_OK;
  }
  elseif (($stats['remaining'] >= $warning) && ($stats['remaining'] < $shutdown)) {
    $test['value'] = $stats['remaining'];
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('Your queue is at @total items which is'
      . ' regarded as dangerous, as it either means that your are purging too'
      . ' broadly (domains, http:// and https://) or that your installation is'
      . ' not able to keep up processing. If you are not using cron for'
      . ' processing, consider this now and if you already are, consider a'
      . ' shorter cron interval. If you have many domain names, consider'
      . ' reducing the domains. As safety measure, Acquia Purge will shut'
      . ' itself down once the queue reaches @shutdown items!', $tvars);
  }
  elseif ($allriskmode && ($stats['remaining'] >= $shutdown)) {
    $test['value'] = $stats['remaining'];
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_WARNING;
    $test['description'] = $t('Your queue contains @total items and crossed'
      . ' the safety limit of @shutdown items, Acquia Purge has not shut itself'
      . ' down because you enabled "all risk mode". Because of this, you are'
      . ' not entitled to Acquia support for any tickets related to queue'
      . ' processing. You are being highly encouraged to lower your slowdown'
      . ' factor by reducing domain names and/or increasing your cron interval'
      . ' or enable cron processing when you have not yet done this.', $tvars);
  }
  elseif ($stats['remaining'] >= $shutdown) {
    $test['value'] = $stats['remaining'];
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t('Your queue contains @total items and crossed'
      . ' the safety limit of @shutdown items, Acquia Purge has shut itself'
      . ' down to prevent your system from thrashing. What this means is that'
      . ' you are either purging too broadly (domains, http:// and https://)'
      . ' or that your installation is not able to keep up. If you are not'
      . ' using cron for processing, set this up now and if you already are,'
      . ' increase your cron interval! If you have many domain names, reduce'
      . ' the list. Clear Varnish manually and run drush ap-forget to clear'
      . ' your queue and unblock the safety shutdown.', $tvars);
  }

  return $test;
}

/**
 * Report where state data gets stored.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_statedata($t) {
  $test = array(
    'title' => $t('State data location'),
    'value' => ACQUIA_PURGE_STATE_FILE,
    'severity' => ACQUIA_PURGE_SEVLEVEL_OK,
  );
  if (_acquia_purge_are_we_using_memcached()) {
    $test['value'] = 'memcache';
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_INFO;
  }
  return $test;
}

/**
 * Report on the current status of the system.
 *
 * @param string $t
 *   Name of the t() function to call.
 *
 * @return array
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR
 */
function _acquia_purge_get_diagnosis_status($t) {
  $passivemode = _acquia_purge_variable('acquia_purge_passivemode');
  $errorlimit = _acquia_purge_variable('acquia_purge_errorlimit');
  $stats = _acquia_purge_service()->stats();
  $test = array(
    'title' => $t('Status'),
    'value' => $passivemode ? $t('idle (passive mode)') : t('idle'),
    'severity' => ACQUIA_PURGE_SEVLEVEL_INFO,
  );

  // When the user did not set a static integer as error limit, we calculate
  // it dynamically with limit = factor^3, with 20 as lowest limit. The higher
  // the factor is, the more tolerant our limit is. Limits for factors 1-14:
  // 20, 20, 27, 64, 125, 216, 343, 512, 729, 1000, 1331, 1728, 2197, 2744
  if (!is_int($errorlimit)) {
    $factor = _acquia_purge_get_capacity_factor();
    $errorlimit = ($factor <= 2) ? 20 : ceil(pow($factor, 3));
  }
  if ($stats['bad'] > $errorlimit) {
    $test['severity'] = ACQUIA_PURGE_SEVLEVEL_ERROR;
    $test['description'] = $t(
        'Acquia Purge failed over @limit times (@bad), since the last time the'
      . ' queue was processed. To avoid crashing your site with endless queue'
      . ' processing, cache invalidation has been disabled. We recommend you to'
      . " ask your site maintainer to check Drupal's watchdog logs for errors,"
      . ' before lifting the blockade with "drush ap-forget".',
      array('@limit' => $errorlimit, '@bad' => $stats['bad']));
    return $test;
  }

  // Report status if the system is running.
  if ($stats['running']) {
    $test['description'] = theme('acquia_purge_status_bar_widget', $stats);
    if ($stats['locked']) {
      $test['value'] = $test['value_plain'] = $t('Purging');
    }
    else {
      $test['value'] = $test['value_plain'] = $t('Items queued');
    }
  }

  return $test;
}
