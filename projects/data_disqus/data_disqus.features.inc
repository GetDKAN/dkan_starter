<?php
/**
 * @file
 * data_disqus.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function data_disqus_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
