<?php

/**
 * @file
 * User page callbacks for the role_watchdog module.
 */

/**
 * Display tab page from menu callback.
 *
 * @param $account
 *   User object.
 */
function role_watchdog_history($account) {
  $output = '';
  $rows = $rows2 = array();

  $roles = user_roles();

  $view_profile = user_access('access user profiles');
  $header = array(
    array(
      'data' => t('Date'),
      'style' => 'width: 25%;',
    ),
    array(
      'data' => t('Role'),
      'style' => 'width: 30%;',
    ),
    array(
      'data' => t('Change'),
      'style' => 'width: 15%;',
    ),
    array(
      'data' => t('User'),
      'style' => 'width: 40%;',
    ),
  );

  $query = db_select('role_watchdog', 'rw');
  $query->innerJoin('users', 'u', 'rw.uid = u.uid');
  $result = $query
    ->extend('PagerDefault')
    ->limit(variable_get('role_watchdog_pager', 50))
    ->fields('rw', array('hid', 'rid', 'uid', 'action', 'stamp'))
    ->fields('u', array('name'))
    ->condition('rw.aid', $account->uid)
    ->condition('rw.rid', ROLE_WATCHDOG_NO_ROLE, '<>')
    ->orderBy('rw.stamp', 'DESC')
    ->execute()
    ->fetchAllAssoc('hid');
  foreach ($result as $hid => $row) {
    $rows[] = array(
      format_date($row->stamp),
      $roles[$row->rid],
      $row->action ? t('added by') : t('removed by'),
      $view_profile ? l($row->name, 'user/' . $row->uid) : $row->name,
    );
  }

  if (sizeof($rows)) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('style' => 'width: 99%;')));
    $output .= theme('pager', array('tags' => NULL));
  }
  return $output ? $output : t('No role history found.');
}

function role_watchdog_grants($account) {
  $output = '';
  $rows = $rows2 = array();

  $roles = user_roles();

  $view_profile = user_access('access user profiles');
  $header = array(
    array(
      'data' => t('Date'),
      'style' => 'width: 25%;',
    ),
    array(
      'data' => t('Role'),
      'style' => 'width: 30%;',
    ),
    array(
      'data' => t('Change'),
      'style' => 'width: 15%;',
    ),
    array(
      'data' => t('User'),
      'style' => 'width: 40%;',
    ),
  );
  $query = db_select('role_watchdog', 'rw');
  $query->innerJoin('users', 'u', 'rw.aid = u.uid');
  $result = $query
    ->extend('PagerDefault')
    ->limit(variable_get('role_watchdog_pager', 50))
    ->fields('rw', array('hid', 'rid', 'action', 'aid', 'stamp'))
    ->fields('u', array('name'))
    ->condition('rw.uid', $account->uid)
    ->condition('rw.rid', ROLE_WATCHDOG_NO_ROLE, '<>')
    ->orderBy('rw.stamp', 'DESC')
    ->execute()
    ->fetchAllAssoc('hid');
  foreach ($result as $hid => $row) {
      $rows2[] = array(
        format_date($row->stamp),
        $roles[$row->rid],
        $row->action ? t('added to') : t('removed from'),
        $view_profile ? l($row->name, 'user/' . $row->aid) : $row->name,
      );
  }

  if (sizeof($rows2)) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows2, 'attributes' => array('style' => 'width: 99%;')));
    $output .= theme('pager', array('tags' => NULL));
  }

  return $output ? $output : t('No role grants made.');
}

function role_watchdog_report() {
  $output = '';
  $rows = $rows2 = array();

  $roles = user_roles();

  $header = array(
    array(
      'data' => t('Admin'),
      'style' => 'width: 20%;',
    ),
    array(
      'data' => t('Date'),
      'style' => 'width: 25%;',
    ),
    array(
      'data' => t('Role'),
      'style' => 'width: 20%;',
    ),
    array(
      'data' => t('Change'),
      'style' => 'width: 15%;',
    ),
    array(
      'data' => t('User'),
      'style' => 'width: 20%;',
    ),
  );
  $query = db_select('role_watchdog', 'rw');
  $query->innerJoin('users', 'u', 'rw.aid = u.uid');
  $query->innerJoin('users', 'u2', 'rw.uid = u2.uid');
  $query->addField('u2', 'name', 'admin');
  $query->addField('rw', 'uid', 'adminid');
  $query->addField('rw', 'aid', 'uid');
  $result = $query
    ->extend('PagerDefault')
    ->limit(variable_get('role_watchdog_pager', 50))
    ->fields('rw', array('hid', 'rid', 'action', 'aid', 'stamp'))
    ->fields('u', array('name'))
    ->condition('rw.rid', ROLE_WATCHDOG_NO_ROLE, '<>')
    ->orderBy('rw.stamp', 'DESC')
    ->execute()
    ->fetchAllAssoc('hid');

  $rows2 = _role_watchdog_history_rows($result, $roles, array(t('removed from'), t('added to'), t('requested by'), t('approved for')));

   if (sizeof($rows2)) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows2, 'attributes' => array('style' => 'width: 99%;')));
    $output .= theme('pager', array('tags' => NULL));
  }
  return $output ? $output : t('No role grants made.');
}

function _role_watchdog_history_rows($result, $roles, $actions) {
  $view_profile = user_access('access user profiles');
  foreach ($result as $hid => $row) {
    $row_data = array(
      format_date($row->stamp),
      $roles[$row->rid],
      $actions[$row->action],
      $view_profile ? l($row->name, 'user/' . $row->uid) : $row->name,
    );
    if (isset($row->admin)) {
      array_unshift($row_data,  $view_profile ? l($row->admin, 'user/' . $row->adminid) : $row->admin);
    }
    $rows[] = $row_data;
  }
  return $rows;
}
