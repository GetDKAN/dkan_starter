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
      'access stale drafts page' => TRUE,
      'access stale reviews page' => TRUE,
      'access unpublished data' => TRUE,
      'access user profiles' => TRUE,
      'access workbench' => TRUE,
      'administer main-menu menu items' => TRUE,
      'administer nodes' => TRUE,
      'administer taxonomy' => TRUE,
      'administer users' => TRUE,
      'assign content editor role' => TRUE,
      'assign data contributor role' => TRUE,
      'assign group organizer role' => TRUE,
      'assign portal administrator role' => TRUE,
      'bypass workbench moderation' => TRUE,
      'clear dkan_file feeds' => TRUE,
      'clear dkan_link feeds' => TRUE,
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
      'import dkan_file feeds' => TRUE,
      'import dkan_link feeds' => TRUE,
      'manage datastore' => TRUE,
      'moderate content from draft to needs_review' => TRUE,
      'moderate content from needs_review to draft' => TRUE,
      'moderate content from needs_review to published' => TRUE,
      'reference unpublished nodes' => TRUE,
      'revert revisions' => TRUE,
      'unlock dkan_file feeds' => TRUE,
      'unlock dkan_link feeds' => TRUE,
      'use workbench_moderation my drafts tab' => TRUE,
      'use workbench_moderation needs review tab' => TRUE,
      'view all unpublished content' => TRUE,
      'view any unpublished content' => TRUE,
      'view any unpublished dataset content' => TRUE,
      'view any unpublished page content' => TRUE,
      'view any unpublished resource content' => TRUE,
      'view moderation history' => TRUE,
      'view moderation messages' => TRUE,
      'view revisions' => TRUE,
    ),
  );

  return $roles;
}
