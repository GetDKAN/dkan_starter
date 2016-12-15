<?php
/**
 * @file
 * visualization_entity_maps.features.inc
 */

/**
 * Implements hook_eck_bundle_info().
 */
function visualization_entity_maps_eck_bundle_info() {
  $items = array(
    'visualization_ve_map' => array(
      'machine_name' => 'visualization_ve_map',
      'entity_type' => 'visualization',
      'name' => 've_map',
      'label' => 'Map',
    ),
  );
  return $items;
}
