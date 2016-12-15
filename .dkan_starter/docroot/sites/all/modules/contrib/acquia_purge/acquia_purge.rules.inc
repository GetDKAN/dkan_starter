<?php

/**
 * @file
 * Rules integration that provides a purge action as rule action.
 */

/**
 * Implements hook_rules_action_info().
 */
function acquia_purge_rules_action_info() {

  // Original rule action that was only able to purge paths using Acquia Purge
  // but which now maps to the same callback as "Clear pages on Acquia Cloud.".
  return array(
    '_acquia_purge_action_purge' => array(
      'label' => t('Purge a path from Varnish on Acquia Cloud (OBSOLETE!)'),
      'group' => t('Expire'),
      'parameter' => array(
        'path' => array(
          'type' => 'text',
          'label' => t('Path'),
          'description' => t('This rule gets removed in future versions as it is obsolete, please use the new and better "Clear pages on Acquia Cloud" rule action.'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info_alter().
 */
function acquia_purge_rules_action_info_alter(&$actions) {
  $action = 'expire_rules_action_flush_url';
  if (!isset($actions[$action])) {
    return;
  }

  // Rename and overload Expire's "Clear URL(s) from the page cache." rule
  // action into "Clear pages on Acquia Cloud.". With this reimplementation
  // the middle man gets cut out whilst retaining all functionality and it
  // improves the administrators experience drastically.
  $actions[$action]['label'] = t('Clear pages on Acquia Cloud.');
  $actions[$action]['base'] = '_acquia_purge_action_flush_url';
  $actions[$action]['module'] = 'acquia_purge';
  $actions[$action]['parameter']['urls']['description'] = t('A Drupal path or full absolute URL that refers to a page on your website. Paths need to be relative from the document root of your site, for instance "node/1" or "news" but not "news/*" as wildcards are not supported. When providing tokens that generate absolute URLs it is important to understand that the domain names in URLs will be stripped out and not respected as Acquia Purge will use its own list of domains, which it uses to clear every path or URL it processes.');

  // Remove the original validation callback, which we will also take care of.
  unset($actions[$action]['callbacks']);
}

/**
 * Action callback to the "Purge a path from Varnish on Acquia Cloud" rule.
 *
 * Please note that this is Acquia Purge's original rule action, which got
 * deprecated in release 7.x-1.0. It still works as it will just redirect its
 * behavior to the new 'Clear pages on Acquia Cloud.' rule action, but we are
 * actively warning people not to use it.
 *
 * @param string $urls_and_paths
 *   User input string with absolute URL(s) or Drupal paths.
 *
 * @deprecated
 */
function _acquia_purge_action_purge($urls_and_paths) {

  // Rule actions can run very often, and if we would write out a log warning
  // about the deprecation of this rule action every time, we can congest the
  // site. Instead, only put a warning in when the queue's absolutely empty.
  if (_acquia_purge_service()->stats('total') === 0) {
    watchdog(
      'acquia_purge',
      'Please switch to using the rule action "Clear pages on Acquia Cloud."'
        . ' as soon as possible, this rule will disappear in future updates!',
      array(),
      WATCHDOG_ERROR
    );
  }

  // Relay execution to the new rule action, old rules will certainly still
  // work, if not run better with the added URLs support.
  _acquia_purge_action_flush_url($urls_and_paths);
}

/**
 * Purge URL(s) and Path(s), directly using Acquia Purge's APIs.
 *
 * @param string $urls_and_paths
 *   User input string with absolute URL(s) or Drupal paths.
 */
function _acquia_purge_action_flush_url($urls_and_paths) {
  $paths = array();

  // Propagate the $paths array based on anything we found that made sense.
  foreach (explode("\n", $urls_and_paths) as $path) {

    // Trim both the start and end, as else parse_url() will add underscores.
    $path = trim($path);

    // Attempt to parse absolute URLs, and to extract paths when possible.
    if ($url = parse_url($path)) {
      $path = $url['path'];
      if (isset($url['query'])) {
        $path .= '?' . $url['query'];
      }
    }

    // Validate the path after it was cleaned. Duplicates and empty paths will
    // still be added as AcquiaPurgeService::addPaths() deduplicates for us and
    // because empty paths usually represent frontpage wipes.
    $path = _acquia_purge_input_clean($path);
    if ($msg = _acquia_purge_input_validate($path)) {
      switch ($msg) {
        case _acquia_purge_input_validate_msgs('empty'):
        case _acquia_purge_input_validate_msgs('double'):
          $paths[] = $path;
          break;

        // Anything still triggering validation certainly needs attention.
        default:
          watchdog(
            'acquia_purge',
            "Rule action failure on path '%path': !msg",
            array('%path' => $path, '!msg' => $msg),
            WATCHDOG_ERROR
          );
          break;
      }
    }
    else {
      $paths[] = $path;
    }
  }

  _acquia_purge_service()->addPaths($paths);
}
