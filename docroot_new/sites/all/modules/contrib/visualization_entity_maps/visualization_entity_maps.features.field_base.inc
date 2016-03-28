<?php
/**
 * @file
 * visualization_entity_maps.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function visualization_entity_maps_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_ve_map_description'
  $field_bases['field_ve_map_description'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_map_description',
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

  // Exported field_base: 'field_ve_map_file'
  $field_bases['field_ve_map_file'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_map_file',
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'file',
    'settings' => array(
      'display_default' => 0,
      'display_field' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'file',
  );

  // Exported field_base: 'field_ve_map_settings'
  $field_bases['field_ve_map_settings'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_map_settings',
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

  // Exported field_base: 'field_ve_map_uuid_resource'
  $field_bases['field_ve_map_uuid_resource'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_ve_map_uuid_resource',
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
        'group' => 0,
        'page' => 0,
        'resource' => 'resource',
      ),
      'target_type' => 'node',
    ),
    'target_bundles' => array(
      'data_dashboard' => 0,
      'dataset' => 0,
      'group' => 0,
      'page' => 0,
      'resource' => 'resource',
    ),
    'translatable' => 0,
    'type' => 'uuidreference',
  );

  return $field_bases;
}
