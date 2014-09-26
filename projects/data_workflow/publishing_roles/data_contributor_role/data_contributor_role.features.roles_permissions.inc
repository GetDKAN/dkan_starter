<?php
/**
 * @file
 * data_contributor_role.features.roles_permissions.inc
 */

/**
 * Implements hook_default_roles_permissions().
 */
function data_contributor_role_default_roles_permissions() {
  $roles = array();

  // Exported role: data contributor
  $roles['data contributor'] = array(
    'name' => 'data contributor',
    'weight' => 4,
    'machine_name' => 'data_contributor',
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access content overview' => TRUE,
      'access unpublished data' => TRUE,
      'create dataset content' => TRUE,
      'create resource content' => TRUE,
      'delete own dataset content' => TRUE,
      'delete own resource content' => TRUE,
      'edit own dataset content' => TRUE,
      'edit own resource content' => TRUE,
      'manage datastore' => TRUE,
      'reference unpublished nodes' => TRUE,
      'view own unpublished content' => TRUE,
    ),
  );

  return $roles;
}
