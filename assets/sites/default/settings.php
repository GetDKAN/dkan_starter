<?php

$conf['install_profile'] = 'dkan';
$sitename = "somesite";

// TODO: Make this file work on both Acquia and Pantheon.

// On Acquia Cloud, this include file configures Drupal to use the correct
// database in each site environment (Dev, Stage, or Prod). To use this
// settings.php for development on your local workstation, set $db_url
// (Drupal 5 or 6) or $databases (Drupal 7) as described in comments above.
if (file_exists('/var/www/site-php')) {
  require "/var/www/site-php/$sitename/$sitename-settings.inc";
}

// IMPORTANT. Local settings include comes first so we can fake acquia env variables on local.
// See settings.local.demo.php for how to setup your local environment.
if (!(function_exists('acquia_hosting_db_choose_active') || file_exists('/var/www/site-php'))) {
  $settings_local = DRUPAL_ROOT . '/' . conf_path() . '/settings.local.php';
  if (file_exists($settings_local)) {
    include $settings_local;
  }
}

/**
 * Add Additional Custom Settings Below This Line.
 */
