<?php
/**
 * @file
 * dkan_dash.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function dkan_dash_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-react_dashboard-field_dash_settings'.
  $field_instances['node-react_dashboard-field_dash_settings'] = array(
    'bueditor_profile' => 0,
    'bundle' => 'react_dashboard',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_dash_settings',
    'label' => 'Settings',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 13,
      ),
      'type' => 'text_textarea',
      'weight' => 32,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Settings');

  return $field_instances;
}
