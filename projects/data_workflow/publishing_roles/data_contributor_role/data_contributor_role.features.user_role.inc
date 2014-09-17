<?php
/**
 * @file
 * data_contributor_role.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function data_contributor_role_user_default_roles() {
  $roles = array();

  // Exported role: data contributor.
  $roles['data contributor'] = array(
    'name' => 'data contributor',
    'weight' => 4,
    'machine_name' => 'data_contributor',
  );

  return $roles;
}
