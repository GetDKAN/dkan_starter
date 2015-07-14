<?php

/**
 * @file
 * Hooks provided by features_banish.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the banished feature items provided by features_banish_get_banished().
 *
 * Items are loaded from:
 * - module.info files (feature modules only): To use this, change the info item
 *   from features[something].. to features_banish[something]..
 * - features_banish_items variable: You can set the same way you can set all
 *   system variables: drush vset, variable_set(), or $conf['settings'].
 *
 * @param $banished_items
 *   An array whose keys are feature component names and values are an array of
 *   items to bansish. See a features export as an example.
 */
function hook_features_banish_alter(&$banished_items) {
  // Banish the cron_last variable when using strongarm. It should never
  // be exported because it's a timestamp when cron ran last and changes.
  $banished_items['stongarm'][] = 'cron_last';
}
