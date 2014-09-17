<?php
/**
 * @file
 * group_organizer_role.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function group_organizer_role_user_default_roles() {
  $roles = array();

  // Exported role: group organizer.
  $roles['group organizer'] = array(
    'name' => 'group organizer',
    'weight' => 3,
    'machine_name' => 'group_organizer',
  );

  return $roles;
}
