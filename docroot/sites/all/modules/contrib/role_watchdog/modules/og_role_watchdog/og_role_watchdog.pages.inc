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
function og_role_watchdog_history($account) {
  $output = '';
  $rows = $rows2 = array();

  $roles = user_roles();
  $header = array(
    array(
      'data' => t('Date'),
      'style' => 'width: 25%;',
    ),
    array(
      'data' => t('Group'),
      'style' => 'width: 20%;',
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

  $view_profile = user_access('access user profiles');

  $query = db_select('role_watchdog', 'rw');
  $query->innerJoin('users', 'u', 'rw.uid = u.uid');
  $query->leftJoin('og_role_watchdog', 'orw', 'orw.hid = rw.hid');
  $query->addField('orw', 'rid', 'orid');
  $result = $query
    ->extend('PagerDefault')
    ->limit(variable_get('role_watchdog_pager', 50))
    ->fields('rw', array('hid', 'rid', 'uid', 'action', 'stamp'))
    ->fields('u', array('name'))
    ->fields('orw', array('gid'))
    ->condition('rw.aid', $account->uid)
    ->orderBy('rw.stamp', 'DESC')
    ->execute()
    ->fetchAllAssoc('hid');
  $actions =  array(t('removed by'), t('added by'), t('requested for'), t('approved by'), t('blocked by'), t('unblocked by'), t('rejected by'));
  foreach ($result as $hid => $row) {
    list($group_name, $role) = _og_role_watchdog_row_group_name($row->gid, $row->rid, $row->orid, $roles);
    $rows[] = array(
      format_date($row->stamp),
      $group_name,
      $role,
      $actions[$row->action],
      $view_profile ? l($row->name, 'user/' . $row->uid) : $row->name,
    );
  }

  if (sizeof($rows)) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('style' => 'width: 99%;')));
    $output .= theme('pager', array('tags' => NULL));
  }
  return $output ? $output : t('No role history found.');
}

function og_role_watchdog_grants($account) {
  $output = '';
  $rows = $rows2 = array();

  $roles = user_roles();

  $header = array(
    array(
      'data' => t('Date'),
      'style' => 'width: 25%;',
    ),
    array(
      'data' => t('Group'),
      'style' => 'width: 20%;',
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
  $view_profile = user_access('access user profiles');
  $query = db_select('role_watchdog', 'rw');
  $query->innerJoin('users', 'u', 'rw.aid = u.uid');
  $query->leftJoin('og_role_watchdog', 'orw', 'orw.hid = rw.hid');
  $query->addField('orw', 'rid', 'orid');
  $result = $query
    ->extend('PagerDefault')
    ->limit(variable_get('role_watchdog_pager', 50))
    ->fields('rw', array('hid', 'rid', 'action', 'aid', 'stamp'))
    ->fields('u', array('name'))
    ->fields('orw', array('gid'))
    ->condition('rw.uid', $account->uid)
    ->orderBy('rw.stamp', 'DESC')
    ->execute()
    ->fetchAllAssoc('hid');
  $actions = array(t('removed from'), t('added to'), t('requested by'), t('approved for'), t('blocked'), t('unblocked'), t('rejected for'));
  foreach ($result as $hid => $row) {
    list($group_name, $role) = _og_role_watchdog_row_group_name($row->gid, $row->rid, $row->orid, $roles);
    $rows2[] = array(
      format_date($row->stamp),
      $group_name,
      $role,
      $actions[$row->action],
      $view_profile ? l($row->name, 'user/' . $row->aid) : $row->name,
    );
  }

  if (sizeof($rows2)) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows2, 'attributes' => array('style' => 'width: 99%;')));
    $output .= theme('pager', array('tags' => NULL));
  }

  return $output ? $output : t('No role grants made.');
}

function og_role_watchdog_report($node = NULL) {
  $output = '';
  $rows = array();

  $roles = user_roles();

  $header = array(
    array(
      'data' => t('Admin'),
      'style' => 'width: 20%;',
    ),
    array(
      'data' => t('Date'),
      'style' => 'width: 20%;',
    ),
    array(
      'data' => t('Group'),
      'style' => 'width: 15%;',
    ),
    array(
      'data' => t('Role'),
      'style' => 'width: 15%;',
    ),
    array(
      'data' => t('Change'),
      'style' => 'width: 15%;',
    ),
    array(
      'data' => t('User'),
      'style' => 'width: 15%;',
    ),
  );
  $query = db_select('role_watchdog', 'rw');
  $query->innerJoin('users', 'u', 'rw.aid = u.uid');
  $query->innerJoin('users', 'u2', 'rw.uid = u2.uid');
  $query->addField('u2', 'name', 'admin');
  $query->addField('rw', 'uid', 'adminid');
  $query->addField('rw', 'aid', 'uid');
  if ($node) {
    $query->join('og_role_watchdog', 'orw', 'orw.hid = rw.hid');
    list($id, $vid, $bundle_name) = entity_extract_ids('node', $node);
    $group = og_get_group('node', $id);
    $query->condition('orw.gid', $group->gid);
    unset($header[2]);
  }
  else {
    $query->leftJoin('og_role_watchdog', 'orw', 'orw.hid = rw.hid');
  }
  $query->addField('orw', 'rid', 'orid');
  $result = $query
    ->extend('PagerDefault')
    ->limit(variable_get('role_watchdog_pager', 50))
    ->fields('rw', array('hid', 'action', 'rid', 'aid', 'stamp'))
    ->fields('u', array('name'))
    ->fields('orw', array('gid'))
    ->orderBy('rw.stamp', 'DESC')
    ->execute()
    ->fetchAllAssoc('hid');

  $actions = array(t('removed from'), t('added to'), t('requested by'), t('approved for'), t('blocked'), t('unblocked'), t('rejected for'));
  $view_profile = user_access('access user profiles');
  foreach ($result as $hid => $row) {
    list($group_name, $role) = _og_role_watchdog_row_group_name($row->gid, $row->rid, $row->orid, $roles);
    $row_data = array(
      $view_profile ? l($row->admin, 'user/' . $row->adminid) : $row->admin,
      format_date($row->stamp),
      $group_name,
      $role,
      $actions[$row->action],
      $view_profile ? l($row->name, 'user/' . $row->uid) : $row->name,
    );
    if ($node) {
      unset($row_data[2]);
    }
    $rows[] = $row_data;
  }

  if (sizeof($rows)) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('style' => 'width: 99%;')));
    $output .= theme('pager', array('tags' => NULL));
  }
  return $output ? $output : t('No role grants made.');
}

function _og_role_watchdog_history_rows($result, $roles, $actions) {
  $view_profile = user_access('access user profiles');
  foreach ($result as $hid => $row) {
    $group = '-';
    if ($row->rid == ROLE_WATCHDOG_NO_ROLE) {
      $group = "GROUP!";
      $role = "GROUP ROLE!";
    }
    else {
      $role = $roles[$row->rid];
    }
    $row_data = array(
      $view_profile ? l($row->admin, 'user/' . $row->adminid) : $row->admin,
      format_date($row->stamp),
      $group,
      $role,
      $actions[$row->action],
      $view_profile ? l($row->name, 'user/' . $row->uid) : $row->name,
    );
    $rows[] = $row_data;
  }
  return $rows;
}

/**
 * Determine the string to put in the "group name" column of
 * an og role watchdog report table.
 *
 * @param $gid
 *  The group ID that the role change happened in, or NULL
 * @param $rid
 *  The global role id that was given or removed, or
 *  ROLE_WATCHDOG_NO_ROLE if a group role was given or removed.
 * @param $og_rid
 *  The group role id that was given or removed, or NULL
 *  if a global role was given or removed.
 * @param $roles
 *  An associative array mapping each global role id to its label
 *
 * Note that $gid and $og_rid are set iff $rid is ROLE_WATCHDOG_NO_ROLE.
 */
function _og_role_watchdog_row_group_name($gid, $rid, $og_rid, $roles) {
  $group_name = '-';
  if ($rid == ROLE_WATCHDOG_NO_ROLE) {
    $group = og_load($gid);
    if ($group) {
      $group_entity = og_get_group($group->entity_type, $group->etid);
      $query_gid = og_is_group_default_access($gid) ? 0 : $gid;
      $og_roles = og_roles($query_gid);
      $group_name = l($group_entity->label, $group->entity_type . '/' . $group->etid);
      $role = $og_roles[$og_rid];
    }
  }
  else {
    $role = $roles[$rid];
  }
  return array($group_name, $role);
}


