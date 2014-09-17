<?php
/**
 * @file
 * portal_administrator_role.features.roles_permissions.inc
 */

/**
 * Implements hook_default_roles_permissions().
 */
function portal_administrator_role_default_roles_permissions() {
  $roles = array();

  // Exported role: portal administrator
  $roles['portal administrator'] = array(
    'name' => 'portal administrator',
    'weight' => 5,
    'machine_name' => 'portal_administrator',
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access content overview' => TRUE,
      'access unpublished data' => TRUE,
      'access user profiles' => TRUE,
      'administer menu' => TRUE,
      'administer nodes' => TRUE,
      'administer taxonomy' => TRUE,
      'administer users' => TRUE,
      'assign content editor role' => TRUE,
      'assign data contributor role' => TRUE,
      'assign group organizer role' => TRUE,
      'assign portal administrator role' => TRUE,
      'create page content' => TRUE,
      'delete any dataset content' => TRUE,
      'delete any page content' => TRUE,
      'delete any resource content' => TRUE,
      'delete own page content' => TRUE,
      'delete revisions' => TRUE,
      'edit any dataset content' => TRUE,
      'edit any page content' => TRUE,
      'edit any resource content' => TRUE,
      'edit own page content' => TRUE,
      'manage datastore' => TRUE,
      'reference unpublished nodes' => TRUE,
      'revert revisions' => TRUE,
      'view any unpublished content' => TRUE,
      'view any unpublished dataset content' => TRUE,
      'view any unpublished page content' => TRUE,
      'view any unpublished resource content' => TRUE,
      'view revisions' => TRUE,
    ),
  );

  return $roles;
}
