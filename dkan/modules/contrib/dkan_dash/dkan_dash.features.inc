<?php
/**
 * @file
 * dkan_dash.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dkan_dash_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function dkan_dash_node_info() {
  $items = array(
    'react_dashboard' => array(
      'name' => t('Dashboard'),
      'base' => 'node_content',
      'description' => t('Provides the functionality to create react dashboards'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
