<?php
/**
 * @file
 * publishing_workflow.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function publishing_workflow_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workbench_moderation_default_state_dataset';
  $strongarm->value = 'draft';
  $export['workbench_moderation_default_state_dataset'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workbench_moderation_default_state_resource';
  $strongarm->value = 'draft';
  $export['workbench_moderation_default_state_resource'] = $strongarm;

  return $export;
}
