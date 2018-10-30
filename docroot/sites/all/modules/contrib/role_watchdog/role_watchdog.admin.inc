<?php

/**
 * @file
 * Admin page callbacks file for the role_watchdog module.
 */

/**
 * Display admin settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function role_watchdog_admin_settings() {
  $form = array();

  $form['reports'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reports'),
    '#description' => t('Role watchdog presents three reports: a global !grants, that shows all role changes made on the system, and two per-user report pages, showing role history (role changes made to that user), and role grants (role changes made by that user).  You may configure how many items appear on each page of these reports here.', array('!grants' => l(t('grants report'), 'admin/reports/role_watchdog'))),
  );
  $form['reports']['role_watchdog_pager'] = array(
    '#type' => 'textfield',
    '#title' => t('Items per page'),
    '#default_value' => variable_get('role_watchdog_pager', 50),
  );

  $roles = user_roles();
  unset($roles[DRUPAL_ANONYMOUS_RID], $roles[DRUPAL_AUTHENTICATED_RID]); // No built-in roles.
  if (!count($roles)) {
    $form['watchdog'] = array(
      '#type' => 'fieldset',
      '#title' => t('No roles found'),
      '#description' => t('Role watchdog requires custom roles to function. !add', array('!add' => l(t('Add Roles'), 'admin/people/permissions/roles'))),
    );

    return $form;
  }

  $form['watchdog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logging'),
    '#description' => t('Role watchdog automatically logs all role changes made. No further configuration is necessary for this functionality. Users will need either "View role history" or "View own role history" !access to view the Role history tab.', array('!access' => l(t('access permissions'), 'admin/people/permissions', array('fragment' => 'module-role_watchdog')))),
  );
  $form['watchdog']['description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable logging'),
    '#default_value' => TRUE,
    '#attributes' => array('disabled' => 'disabled'),
  );

  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email notification'),
    '#description' => t('Role watchdog can monitor one or more roles for changes and notify members of selected roles via email whenever a change occurs. At least one Monitor and one Notify role must be selected for this functionality.'),
  );
  $form['email']['role_watchdog_monitor_roles'] = array(
    '#type' => 'select',
    '#title' => t('Monitor for change'),
    '#options' => $roles,
    '#default_value' => variable_get('role_watchdog_monitor_roles', NULL),
    '#description' => t('Select roles to monitor for change.'),
    '#multiple' => TRUE,
  );
  $form['email']['role_watchdog_notify_roles'] = array(
    '#type' => 'select',
    '#title' => t('Notify on change'),
    '#options' => array(ROLE_WATCHDOG_SUPERUSER_RID => 'Superuser (uid 1)') + $roles,
    '#default_value' => variable_get('role_watchdog_notify_roles', NULL),
    '#description' => t('Select roles to notify on change.'),
    '#multiple' => TRUE,
  );

  return system_settings_form($form);
}
