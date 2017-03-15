<?php
/**
 * @file
 * dkan_dash.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function dkan_dash_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create react_dashboard content'.
  $permissions['create react_dashboard content'] = array(
    'name' => 'create react_dashboard content',
    'roles' => array(
      'administrator' => 'administrator',
      'content creator' => 'content creator',
      'editor' => 'editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any react_dashboard content'.
  $permissions['delete any react_dashboard content'] = array(
    'name' => 'delete any react_dashboard content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own react_dashboard content'.
  $permissions['delete own react_dashboard content'] = array(
    'name' => 'delete own react_dashboard content',
    'roles' => array(
      'administrator' => 'administrator',
      'content creator' => 'content creator',
      'editor' => 'editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any react_dashboard content'.
  $permissions['edit any react_dashboard content'] = array(
    'name' => 'edit any react_dashboard content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own react_dashboard content'.
  $permissions['edit own react_dashboard content'] = array(
    'name' => 'edit own react_dashboard content',
    'roles' => array(
      'administrator' => 'administrator',
      'content creator' => 'content creator',
      'editor' => 'editor',
      'site manager' => 'site manager',
    ),
    'module' => 'node',
  );

  return $permissions;
}
