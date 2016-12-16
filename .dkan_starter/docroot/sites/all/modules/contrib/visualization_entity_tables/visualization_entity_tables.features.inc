<?php
/**
 * @file
 * visualization_entity_tables.features.inc
 */

/**
 * Implements hook_eck_bundle_info().
 */
function visualization_entity_tables_eck_bundle_info() {
  $items = array(
    'visualization_ve_table' => array(
      'machine_name' => 'visualization_ve_table',
      'entity_type' => 'visualization',
      'name' => 've_table',
      'label' => 'Table',
    ),
  );
  return $items;
}
