<?php
/**
 * @file
 * visualization_entity.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function visualization_entity_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_uuid_resource'
  $field_bases['field_uuid_resource'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_uuid_resource',
    'foreign keys' => array(),
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
        'dataset' => 0,
        'group' => 0,
        'page' => 0,
        'resource' => 0,
      ),
      'target_type' => 'node',
    ),
    'target_bundles' => array(
      'dataset' => 0,
      'group' => 0,
      'page' => 0,
      'resource' => 0,
    ),
    'translatable' => 0,
    'type' => 'uuidreference',
  );

  return $field_bases;
}
