<?php
/**
 * @file
 * og_permissions_settings.features.og_features_role.inc
 */

/**
 * Implements hook_og_features_default_roles().
 */
function og_permissions_settings_og_features_default_roles() {
  $roles = array();

  // Exported OG Role: 'node:group:content editor'.
  $roles['node:group:content editor'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'group',
    'name' => 'content editor',
  );

  return $roles;
}
