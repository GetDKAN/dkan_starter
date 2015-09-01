<?php

/**
 * @file
 * dkan_harvest.drush.inc.
 */

/**
 * Implements hook_drush_command().
 */
function dkan_harvest_drush_command() {
  // Cache the remote ckan endpoint.
  $items['dkan-cache-harvested-data'] = array(
    'aliases' => array('dkan-chd'),
    'description' => 'Caches remote data.json endpoints locally',
    'callback' => 'dkan_harvest_cache_data',
  );
  // Runs the migration.
  $items['dkan-migrate-cached-data'] = array(
    'aliases' => array('dkan-mcd'),
    'description' => 'Migrates cached data.json endpoints',
    'callback' => 'dkan_harvest_migrate_data',
  );
  // Cache + Migration run.
  $items['dkan-harvest-run'] = array(
    'aliases' => array('dkan-hr'),
    'description' => 'Caches data and runs the migration for data.json endpoints',
    'callback' => 'dkan_harvest_run',
  );
  return $items;
}
