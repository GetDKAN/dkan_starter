<?php

/**
 * @file
 * Rules integration for APE module.
 */

/**
 * Implements hook_rules_action_info().
 */
function ape_rules_action_info() {
  return array(
    'set_cache_header' => array(
      'label' => t('Set page cache expiration'),
      'group' => t('System'),
      'parameter' => array(
        'max_age' => array(
          'type' => 'integer',
          'label' => t('Expiration of cached page'),
          'description' => t('The maximum time an external cache can use an old version of a page, in seconds.'),
          'restriction' => 'input',
          'default value' => 1800,
        ),
      ),
      'base' => 'ape_cache_set',
      'access callback' => 'ape_rules_access',
    ),
  );
}
