<?php

/**
 * @file
 * Drush integration providing common maintenance tasks.
 */

/**
 * Implements hook_drush_help().
 */
function acquia_purge_drush_help($section) {
  switch ($section) {
    case 'meta:acquia_purge:title':
      return dt('Acquia Purge commands');

    case 'meta:acquia_purge:summary':
      return dt('Manage and create scheduled purges.');

  }
}

/**
 * Implements hook_drush_command().
 */
function acquia_purge_drush_command() {
  return array(

    // Define the 'ap-diagnosis' command.
    'ap-diagnosis' => array(
      'description' => 'Perform a series of diagnostic self-tests.',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
      'aliases' => array('apd'),
      'examples' => array('drush apd'),
    ),

    // Define the 'ap-domains' command.
    'ap-domains' => array(
      'description' => 'List all domains Acquia Purge will purge against.',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
      'aliases' => array('apdo'),
      'examples' => array('drush apdo'),
    ),

    // Define the 'ap-purge' command.
    'ap-purge' => array(
      'description' => 'Purge a specified path/paths from your balancers.',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
      'aliases' => array('app'),
      'arguments' => array(
        'paths' => 'The Drupal path to be purged, e.g.: "news" or "<front>", multiple separated paths are also accepted: "news contact node/1".',
      ),
      'examples' => array('drush app "news"'),
    ),

    // Define the 'ap-forget' command.
    'ap-forget' => array(
      'description' => 'Forget all scheduled purges and empty the queue.',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
      'aliases' => array('apf'),
      'examples' => array('drush apf'),
    ),

    // Define the 'ap-list' command.
    'ap-list' => array(
      'description' => 'List all the items that are in the queue.',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
      'aliases' => array('apl'),
      'examples' => array('drush apl'),
    ),

    // Define the 'ap-process' command.
    'ap-process' => array(
      'description' => 'Purge all queued items from the command line.',
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
      'aliases' => array('appr'),
      'examples' => array('drush appr'),
    ),
  );
}

/**
 * Perform a series of diagnostic self-tests.
 *
 * @param int $v
 *   Optional, the level of diagnostics presented. Test results that match or
 *   are higher than the given level are returned.
 */
function drush_acquia_purge_ap_diagnosis($v = ACQUIA_PURGE_SEVLEVEL_INFO) {
  $oldverbosity = drush_get_context('DRUSH_VERBOSE');
  $tests = _acquia_purge_get_diagnosis($v);

  // Define a mapping between install.inc's requirement states and drush_log().
  $severitymap = array(
    ACQUIA_PURGE_SEVLEVEL_INFO => 'info',
    ACQUIA_PURGE_SEVLEVEL_OK   => 'ok',
    ACQUIA_PURGE_SEVLEVEL_WARNING => 'warning',
    ACQUIA_PURGE_SEVLEVEL_ERROR   => 'error',
  );

  // Tell Drush to be verbose.
  drush_set_context('DRUSH_VERBOSE', TRUE);
  drush_set_context('DRUSH_COLUMNS', 120);

  // Determine the longest test name and set the drush_type field.
  $longest = 25;
  foreach ($tests as $id => $test) {
    $tests[$id]['drush_type'] = $severitymap[$test['severity']];
    $len = drupal_strlen($test['name']);
    if ($len > $longest) {
      $longest = $len + 2;
    }
  }

  // Iterate the tests and call drush_log with the appropriate message type.
  foreach ($tests as $test) {
    _drush_acquia_purge_ap_diagnosis_log($test, $longest);
  }

  // Set back the old verbosity state.
  drush_set_context('DRUSH_VERBOSE', $oldverbosity);
}

/**
 * Log the given test.
 *
 * @param array $test
 *   Associative array with the following elements:
 *   - title: The name of the requirement.
 *   - value: The current value (e.g., version, time, level, etc).
 *   - description: The description of the requirement/status.
 *   - severity: ACQUIA_PURGE_SEVLEVEL_INFO, _OK, _WARNING, _ERROR.
 * @param int $longest
 *   The longest test name, used to calculate formatting padding.
 */
function _drush_acquia_purge_ap_diagnosis_log(array $test, $longest) {

  // When running in backend mode, log messages aren't printed and forwarded.
  if (drush_get_context('DRUSH_BACKEND')) {
    $message = sprintf('%s: %s', $test['name'], $test['value_plain']);
    return drush_log($message, $test['drush_type']);
  }

  // When not running in backend mode, prepare for fancier output.
  $type = sprintf(' [%s]', $test['drush_type']);
  $pad_maxlength = 11;
  $colors = array(
    'info' => '%s',
    'error'  => '%s',
    'warning' => '%s',
    'ok' => '%s',
  );

  // In color mode, setup ASCII color codes borrowed from _drush_print_log().
  if (!drush_get_context('DRUSH_NOCOLOR')) {
    $colors['error'] = "\033[31;40m\033[1m%s\033[0m";
    $colors['warning'] = "\033[1;33;40m\033[1m%s\033[0m";
    $colors['ok'] = "\033[1;32;40m\033[1m%s\033[0m";
  }

  // Print the test name and value.
  printf(
    sprintf($colors[$test['drush_type']], "%s%s%s%s%s\n"),
    $type,
    str_repeat(' ', $pad_maxlength - strlen($type)),
    drupal_strtoupper($test['name']),
    str_repeat(' ', $longest - strlen($test['name'])),
    drupal_strtolower($test['value_plain'])
  );

  // Print descriptions for warnings and errors.
  if ($test['severity'] >= ACQUIA_PURGE_SEVLEVEL_WARNING) {
    $pad = str_repeat(' ', $pad_maxlength);
    printf(
      sprintf($colors[$test['drush_type']], "\n%s%s\n\n"),
      $pad,
      wordwrap($test['description_plain'], 2 * $longest, "\n$pad")
    );
  }
}

/**
 * List all detected domain names that Acquia Purge will purge.
 */
function drush_acquia_purge_ap_domains() {

  // Stop invocation if we are not detecting Acquia Cloud heuristics.
  if (!_acquia_purge_are_we_on_acquiacloud()) {
    return drush_set_error("You must be on Acquia Cloud to use Acquia Purge.");
  }

  foreach (_acquia_purge_get_domains() as $domain) {
    printf("%s\n", $domain);
  }
}

/**
 * Purge a specified path from your balancers.
 *
 * @param string $paths
 *   The Drupal path to be purged, e.g.: "news" or "<front>",
 *   multiple separated paths are also accepted: "news contact node/1".
 */
function drush_acquia_purge_ap_purge($paths = NULL) {

  // Stop invocation if we are not detecting Acquia Cloud heuristics.
  if (!_acquia_purge_are_we_on_acquiacloud()) {
    return drush_set_error("You must be on Acquia Cloud to use Acquia Purge.");
  }

  // Stop invocation if serious error conditions have been found.
  if (!_acquia_purge_are_we_allowed_to_purge()) {
    return drush_acquia_purge_ap_diagnosis(ACQUIA_PURGE_SEVLEVEL_ERROR);
  }

  // The input may be space separated, split out and validate each path.
  $paths = explode(' ', $paths);
  foreach ($paths as $path) {
    if ($msg = _acquia_purge_input_validate($path)) {
      drush_print($path);
      return drush_set_error($msg);
    }
  }

  // Improve the administrative experience by automatically adding variations
  // on the given paths, which includes trailing slash versions and pagination.
  if (_acquia_purge_variable('acquia_purge_variations')) {
    _acquia_purge_input_path_variations($paths);
  }

  // Add all paths and dispatch processing to ap-process.
  _acquia_purge_service()->addPaths($paths);
  drush_acquia_purge_ap_process();
}

/**
 * Forget all scheduled purges and empty the queue.
 */
function drush_acquia_purge_ap_forget() {

  // Stop invocation if we are not detecting Acquia Cloud heuristics.
  if (!_acquia_purge_are_we_on_acquiacloud()) {
    return drush_set_error("You must be on Acquia Cloud to use Acquia Purge.");
  }

  // Retrieve statistics, clear the queue and log to Drush.
  $remaining = _acquia_purge_service()->stats('remaining');
  _acquia_purge_service()->clear();
  drush_log(dt("Removed @remaining items from the queue.",
    array('@remaining' => $remaining)), 'ok');
}

/**
 * List all the items that are in the queue.
 */
function drush_acquia_purge_ap_list() {

  // Stop invocation if we are not detecting Acquia Cloud heuristics.
  if (!_acquia_purge_are_we_on_acquiacloud()) {
    return drush_set_error("You must be on Acquia Cloud to use Acquia Purge.");
  }

  // Directly query the queue table and print all records.
  $items = db_select('queue', 'q')
    ->fields("q", array("data"))
    ->condition('name', 'acquia_purge')
    ->execute();
  while ($item = $items->fetchAssoc()) {
    if ($item = unserialize(current($item))) {
      printf(" - '%s'\n", current($item));
    }
  }
}

/**
 * Purge all queued items from the command line.
 */
function drush_acquia_purge_ap_process(&$context = NULL) {
  if (!_acquia_purge_are_we_allowed_to_purge()) {
    drush_set_error(dt('Diagnostic error conditions were found, aborting.'));
    return drush_acquia_purge_ap_diagnosis(ACQUIA_PURGE_SEVLEVEL_ERROR);
  }

  //
  // MAIN DRUSH PROCESS.
  //
  if (is_null($context)) {

    // Retrieve the statistics and determine if processing is needed.
    $stats = _acquia_purge_service()->stats();
    if (!$stats['running']) {
      return drush_print("The purge queue is empty, done!");
    }

    // Define the batch operation we're going to conduct.
    $steps = (int) ceil($stats['remaining'] / _acquia_purge_get_capacity());
    $batch = array(
      'title' => dt('Purging'),
      'operations' => array(),
      'init_message' => t('Initializing'),
      'error_message' => t('An error occurred'),
    );
    for ($i = 1; $i <= $steps; $i++) {
      $batch['operations'][] = array(__FUNCTION__, array());
    }

    // Store state data as it else won't be available to the sub processes.
    _acquia_purge_service()->state()->commit();

    // Kick off the batch process, which will start forking processes.
    batch_set($batch);
    $batch =& batch_get();
    drush_backend_batch_process();
  }

  // SUB PROCESS.
  else {

    if (!_acquia_purge_service()->lockAcquire()) {
      return drush_print("Unable to acquire lock, ensure that all users "
        . "close their browser tabs as a different purge seems to be active!");
    }

    // Process the maximum amount of paths we can process during runtime.
    printf("\n# %d items left...\n", _acquia_purge_service()->stats('remaining'));
    if (_acquia_purge_service()->process()) {
      $printed_urls = array();
      foreach (_acquia_purge_service()->history() as $url) {
        if (!(in_array($url, $printed_urls))) {
          drush_log(dt("Purged: @url", array('@url' => $url)), 'ok');
          $printed_urls[] = $url;
        }
      }
    }

    // Inform the Batch API when the process is finished.
    if (!_acquia_purge_service()->stats('running')) {
      $context['finished'] = 1;
      $context['message'] = dt("Queue processed successfully!");
    }
    _acquia_purge_service()->lockRelease();
  }
}
