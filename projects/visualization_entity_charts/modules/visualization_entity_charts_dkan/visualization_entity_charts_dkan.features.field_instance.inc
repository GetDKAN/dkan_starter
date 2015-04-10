<?php
/**
 * @file
 * visualization_entity_charts_dkan.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function visualization_entity_charts_dkan_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'visualization-ve_chart-field_vecd_esource'
  $field_instances['visualization-ve_chart-field_vecd_esource'] = array(
    'bundle' => 've_chart',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Choose DKAN Resource as data source.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 11,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'visualization',
    'field_name' => 'field_vecd_esource',
    'label' => 'Resource',
    'required' => 0,
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
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Choose DKAN Resource as data source.');
  t('Resource');

  return $field_instances;
}
