<?php
/**
 * @file
 * dkan_dash.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dkan_dash_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__react_dashboard';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'path' => array(
          'weight' => '30',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__react_dashboard'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_react_dashboard';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_react_dashboard'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_react_dashboard';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_react_dashboard'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_react_dashboard';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision',
  );
  $export['node_options_react_dashboard'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_react_dashboard';
  $strongarm->value = '0';
  $export['node_preview_react_dashboard'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_react_dashboard';
  $strongarm->value = 0;
  $export['node_submitted_react_dashboard'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_react_dashboard_pattern';
  $strongarm->value = 'dashboard/[node:title]';
  $export['pathauto_node_react_dashboard_pattern'] = $strongarm;
  return $export;
}
