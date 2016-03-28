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
    'callback' => 'dkan_harvest_drush_chd',
    'options' => array(
      'sources' => 'A list of comma delimited source keys as provided in hook_harvest_sources.',
    ),
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
  $items['orphaned-resources-count'] = array(
    'description' => "Count resources that are not linked to datasets.",
    'drupal dependencies' => array('dkan_dataset'),
    'aliases' => array('dkan-or-count', 'orphaned-resources'),
  );
  $items['orphaned-resources-delete'] = array(
    'description' => "Count resources that are not linked to datasets.",
    'drupal dependencies' => array('dkan_dataset'),
    'aliases' => array('dkan-or-delete', 'orphaned-resources-delete'),
  );
  return $items;
}

/**
 * Get a count of orphaned resources.
 */
function drush_dkan_harvest_orphaned_resources_count() {
  $result = db_query("SELECT count(node.nid) AS nid FROM
  {node} node
  LEFT JOIN {field_data_field_dataset_ref} field_data_field_dataset_ref ON node.nid = field_data_field_dataset_ref.entity_id AND (field_data_field_dataset_ref.entity_type = 'node' AND field_data_field_dataset_ref.deleted = '0')
  WHERE (( (node.status = '1') AND (node.type IN  ('resource')) AND (field_data_field_dataset_ref.field_dataset_ref_target_id IS NULL ) ))
  ")->fetchAll();
  drush_print(dt('There are !count orphaned resources', array('!count' => $result[0]->nid)));
  return TRUE;
}

/**
 * Delete orphaned resources.
 */
function drush_dkan_harvest_orphaned_resources_delete() {
  $results = db_query("SELECT node.nid AS nid FROM
  {node} node
  LEFT JOIN {field_data_field_dataset_ref} field_data_field_dataset_ref ON node.nid = field_data_field_dataset_ref.entity_id AND (field_data_field_dataset_ref.entity_type = 'node' AND field_data_field_dataset_ref.deleted = '0')
  WHERE (( (node.status = '1') AND (node.type IN  ('resource')) AND (field_data_field_dataset_ref.field_dataset_ref_target_id IS NULL ) ))
  ")->fetchAll();
  foreach ($results as $result) {
    drush_print(dt('Deleting nid !nid', array('!nid' => $result->nid)));
    node_delete($result->nid);
  }
  return TRUE;
}

/**
 * Callback function for dkan-chd command.
 */
function dkan_harvest_drush_chd() {
  $options = array();
  $options['sources'] = drush_get_option('sources', FALSE);
  $options['sources'] = $options['sources'] ? explode(',', $options['sources']) : array();
  return dkan_harvest_cache_data($options);
}
