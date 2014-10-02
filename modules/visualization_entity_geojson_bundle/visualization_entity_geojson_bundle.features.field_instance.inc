<?php
/**
 * @file
 * visualization_entity_geojson_bundle.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function visualization_entity_geojson_bundle_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'visualization-geojson_visualization-field_resource'
  $field_instances['visualization-geojson_visualization-field_resource'] = array(
    'bundle' => 'geojson_visualization',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'visualization',
    'field_name' => 'field_resource',
    'label' => 'resource',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'apply_chosen' => 0,
      ),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('resource');

  return $field_instances;
}
