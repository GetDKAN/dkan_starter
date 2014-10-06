<?php
/**
 * @file
 * nucivic_data_demo_front.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function nucivic_data_demo_front_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function nucivic_data_demo_front_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
