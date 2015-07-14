<?php
/**
 * @file
 * content_editor_role.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function content_editor_role_user_default_roles() {
  $roles = array();

  // Exported role: content editor.
  $roles['content editor'] = array(
    'name' => 'content editor',
    'weight' => 4,
    'machine_name' => 'content_editor',
  );

  return $roles;
}
