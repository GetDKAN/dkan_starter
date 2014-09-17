<?php
/**
 * @file
 * portal_administrator_role.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function portal_administrator_role_user_default_roles() {
  $roles = array();

  // Exported role: portal administrator.
  $roles['portal administrator'] = array(
    'name' => 'portal administrator',
    'weight' => 5,
    'machine_name' => 'portal_administrator',
  );

  return $roles;
}
