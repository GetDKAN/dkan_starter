<?php
/**
 * @file
 * group_organizer_role.features.roles_permissions.inc
 */

/**
 * Implements hook_default_roles_permissions().
 */
function group_organizer_role_default_roles_permissions() {
  $roles = array();

  // Exported role: group organizer
  $roles['group organizer'] = array(
    'name' => 'group organizer',
    'weight' => 3,
    'machine_name' => 'group_organizer',
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access content overview' => TRUE,
      'access unpublished data' => TRUE,
      'administer group' => TRUE,
      'assign group organizer role' => TRUE,
      'create group content' => TRUE,
      'delete any group content' => TRUE,
      'delete own group content' => TRUE,
      'edit any group content' => TRUE,
      'edit own group content' => TRUE,
      'view any unpublished group content' => TRUE,
    ),
  );

  return $roles;
}
