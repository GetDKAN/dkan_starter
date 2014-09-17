<?php
/**
 * @file
 * portal_administrator_role.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function portal_administrator_role_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'roleassign_roles';
  $strongarm->value = array(
    34915189 => '34915189',
    254633039 => '254633039',
    27274083 => '27274083',
    4 => 0,
    3 => 0,
  );
  $export['roleassign_roles'] = $strongarm;

  return $export;
}
