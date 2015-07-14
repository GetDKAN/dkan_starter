<?php
/**
 * @file
 * portal_administrator_role.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function portal_administrator_role_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
