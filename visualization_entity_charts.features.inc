<?php
/**
 * @file
 * visualization_entity_charts.features.inc
 */

/**
 * Implements hook_eck_bundle_info().
 */
function visualization_entity_charts_eck_bundle_info() {
  $items = array(
    'visualization_ve_chart' => array(
      'machine_name' => 'visualization_ve_chart',
      'entity_type' => 'visualization',
      'name' => 've_chart',
      'label' => 'Chart',
    ),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function visualization_entity_charts_eck_entity_type_info() {
  $items = array(
    'visualization' => array(
      'name' => 'visualization',
      'label' => 'Visualization',
      'properties' => array(
        'title' => array(
          'label' => 'Title',
          'type' => 'text',
          'behavior' => 'title',
        ),
        'created' => array(
          'label' => 'Created',
          'type' => 'integer',
          'behavior' => 'created',
        ),
        'changed' => array(
          'label' => 'Changed',
          'type' => 'integer',
          'behavior' => 'changed',
        ),
      ),
    ),
  );
  return $items;
}
