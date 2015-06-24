<?php
/**
 * @file
 * custom_config.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function custom_config_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "fieldable_panels_panes" && $api == "fieldable_panels_pane_type") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function custom_config_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_open_data_schema_apis_defaults().
 */
function custom_config_open_data_schema_apis_defaults() {
  $open_data_schema_apis = array();

  // Exported Open Data API Map: CKAN Package Revision List
  $open_data_schema_apis['ckan_package_revision_list'] = array(
    'name' => 'CKAN Package Revision List',
    'enabled' => TRUE,
    'api_schema' => 'ckan_dataset',
    'entity' => '',
    'bundle' => '',
    'arguments' => array(
      1 => array(
        'field' => 'id',
        'required' => 1,
      ),
    ),
    'machine_name' => 'ckan_package_revision_list',
    'outputformat' => 'json',
    'endpoint' => 'api/3/action/package_revision_list',
    'callback' => 'open_data_schema_ckan_package_revision_list_callback',
  );
  // Exported Open Data API Map: CKAN Revision List
  $open_data_schema_apis['ckan_revision_list'] = array(
    'name' => 'CKAN Revision List',
    'enabled' => TRUE,
    'api_schema' => 'ckan_dataset',
    'entity' => '',
    'bundle' => '',
    'arguments' => array(),
    'machine_name' => 'ckan_revision_list',
    'outputformat' => 'json',
    'endpoint' => 'api/3/action/revision_list',
    'callback' => 'open_data_schema_ckan_revision_list_callback',
  );
  // Exported Open Data API Map: CKAN Site Read
  $open_data_schema_apis['ckan_site_read'] = array(
    'name' => 'CKAN Site Read',
    'enabled' => TRUE,
    'api_schema' => 'ckan_no_schema',
    'entity' => '',
    'bundle' => '',
    'arguments' => array(),
    'machine_name' => 'ckan_site_read',
    'outputformat' => 'json',
    'endpoint' => 'api/3/action/site_read',
    'callback' => 'open_data_schema_ckan_site_read_callback',
  );
  return $open_data_schema_apis;
}
