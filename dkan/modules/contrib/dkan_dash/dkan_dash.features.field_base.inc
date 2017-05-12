<?php
/**
 * @file
 * dkan_dash.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function dkan_dash_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_dash_settings'.
  $field_bases['field_dash_settings'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_dash_settings',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  return $field_bases;
}
