<?php
/**
 * @file
 * data_story_storyteller_role.features.roles_permissions.inc
 */

/**
 * Implements hook_default_roles_permissions().
 */
function data_story_storyteller_role_default_roles_permissions() {
  $roles = array();

  // Exported role: storyteller
  $roles['storyteller'] = array(
    'name' => 'storyteller',
    'weight' => 6,
    'machine_name' => 'storyteller',
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access content' => TRUE,
      'access content overview' => TRUE,
      'create blog content' => TRUE,
      'delete own blog content' => TRUE,
      'edit own blog content' => TRUE,
      'use text format filtered_html' => TRUE,
      'use text format full_html' => TRUE,
      'use text format html' => TRUE,
      'use text format php_code' => TRUE,
      'view own unpublished content' => TRUE,
    ),
  );

  return $roles;
}
