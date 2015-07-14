<?php
/**
 * @file
 * content_editor_role.features.roles_permissions.inc
 */

/**
 * Implements hook_default_roles_permissions().
 */
function content_editor_role_default_roles_permissions() {
  $roles = array();

  // Exported role: content editor
  $roles['content editor'] = array(
    'name' => 'content editor',
    'weight' => 4,
    'machine_name' => 'content_editor',
    'permissions' => array(
      'access administration menu' => TRUE,
      'access administration pages' => TRUE,
      'access content' => TRUE,
      'access content overview' => TRUE,
      'access unpublished data' => TRUE,
      'access workbench' => TRUE,
      'administer feeds' => TRUE,
      'administer nodes' => TRUE,
      'assign content editor role' => TRUE,
      'assign data contributor role' => TRUE,
      'clear dkan_file feeds' => TRUE,
      'clear dkan_link feeds' => TRUE,
      'create dataset content' => TRUE,
      'create page content' => TRUE,
      'create resource content' => TRUE,
      'delete own dataset content' => TRUE,
      'delete own group content' => TRUE,
      'delete own resource content' => TRUE,
      'edit any dataset content' => TRUE,
      'edit any page content' => TRUE,
      'edit any resource content' => TRUE,
      'edit own dataset content' => TRUE,
      'edit own page content' => TRUE,
      'edit own resource content' => TRUE,
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
      'view any unpublished content' => TRUE,
      'view any unpublished dataset content' => TRUE,
      'view any unpublished page content' => TRUE,
      'view any unpublished resource content' => TRUE,
      'view moderation history' => TRUE,
      'view moderation messages' => TRUE,
      'view own unpublished content' => TRUE,
      'view revisions' => TRUE,
    ),
  );

  return $roles;
}
