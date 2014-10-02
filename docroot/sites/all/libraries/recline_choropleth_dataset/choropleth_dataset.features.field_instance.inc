<?php
/**
 * @file
 * choropleth_dataset.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function choropleth_dataset_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-dataset-field_choropleth_label'
  $field_instances['node-dataset-field_choropleth_label'] = array(
    'bundle' => 'dataset',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Input the label for the point based map in the choropleth map view.',
    'display' => array(
      'choropleth' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 21,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_choropleth_label',
    'label' => 'Choropleth Point-based Map Label',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Choropleth Point-based Map Label');
  t('Input the label for the point based map in the choropleth map view.');

  return $field_instances;
}
