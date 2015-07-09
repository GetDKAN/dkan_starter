<?php
/**
 * @file
 * content_editor_role.features.workbench_email.inc
 */

/**
 * Implements hook_workbench_email_export().
 */
function content_editor_role_workbench_email_export() {
  $items = array(
    'draft:needs_review:content editor' => array(
      'from_name' => 'draft',
      'to_name' => 'needs_review',
      'subject' => '[user:name] has just set [node:title] to be reviewed',
      'message' => 'Please review the recent update at [node:edit-url]',
      'author' => 0,
      'automatic' => 1,
      'role' => 'content editor',
    ),
    'needs_review:published:content editor' => array(
      'from_name' => 'needs_review',
      'to_name' => 'published',
      'subject' => ' [user:name] has just published [node:title]',
      'message' => 'Please view published content at [node:url]',
      'author' => 0,
      'automatic' => 1,
      'role' => 'content editor',
    ),
  );
  return $items;
}
