<?php
/**
 * @file
 * visualization_entity_tables.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function visualization_entity_tables_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_show_title'
  $field_bases['field_show_title'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_show_title',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => '',
        1 => '',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_ve_table_description'
  $field_bases['field_ve_table_description'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_table_description',
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

  // Exported field_base: 'field_ve_table_numrecords'
  $field_bases['field_ve_table_numrecords'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_table_numrecords',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  // Exported field_base: 'field_ve_table_pager'
  $field_bases['field_ve_table_pager'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_table_pager',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => '',
        1 => '',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_ve_table_resize'
  $field_bases['field_ve_table_resize'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_table_resize',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => '',
        1 => '',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_ve_table_uuid_resource'
  $field_bases['field_ve_table_uuid_resource'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_table_uuid_resource',
    'indexes' => array(
      'target_entity' => array(
        0 => 'target_type',
        1 => 'target_uuid',
      ),
    ),
    'locked' => 0,
    'module' => 'uuidreference',
    'settings' => array(
      'target_bundles' => array(
        'data_dashboard' => 0,
        'dataset' => 0,
        'dkan_data_story' => 0,
        'group' => 0,
        'page' => 0,
        'resource' => 'resource',
      ),
      'target_type' => 'node',
    ),
    'target_bundles' => array(
      'data_dashboard' => 0,
      'dataset' => 0,
      'dkan_data_story' => 0,
      'group' => 0,
      'page' => 0,
      'resource' => 'resource',
    ),
    'translatable' => 0,
    'type' => 'uuidreference',
  );

  return $field_bases;
}
