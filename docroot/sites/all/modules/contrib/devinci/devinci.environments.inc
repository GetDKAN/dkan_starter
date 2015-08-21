<?php

/**
 *
 * Standardize various environments into a single standardized set.
 *
 * @param bool $env_map
 *   array - key  => value pairs where key is the acquia, pantheon, local, or
 *   other environment and value being the standard environments that maps to.
 * @throws \Exception
 */
function devinci_set_env($env_map = false) {

  if ($env_map === FALSE) {
    $env_map = array(
      'dev' => 'dev',
      'test' => 'test',
      'live' => 'live',
      'prod' => 'live',
      'local' => 'local',
    );
  }
  // First check if the ENVIRONMENT constant is already set.
  if (defined('ENVIRONMENT')) {
    $env = ENVIRONMENT;
  }
  // Next check if the ENVIRONMENT linux environment variable is set.
  elseif (getenv('ENVIRONMENT')) {
    $env = getenv('ENVIRONMENT');
  }
  // If not, check for acquia settings.
  elseif ($acquia_env = devinci_get_acquia_env($env_map)) {
    $env = $acquia_env;
    include DRUPAL_ROOT . '/' . conf_path() . '/acquia.tools.inc';
  }
  // Check for pantheon settings.
  elseif ($pantheon_env = devinci_get_pantheon_env($env_map)) {
    $env = $pantheon_env;
    include DRUPAL_ROOT . '/' . conf_path() . '/pantheon.tools.inc';
  }
  else {
    $env = null;
  }

  // Finally check for a local environment settings file to allow custom logic.
  if (file_exists(DRUPAL_ROOT . '/' . conf_path() . '/environment.settings.php')) {
    include DRUPAL_ROOT . '/' . conf_path() . '/environment.settings.php';
  }
  // If no custom file, then go ahead and define ENVIRONMENT as long as it wasn't already defined.
  elseif (!defined('ENVIRONMENT')) {
    throw new Exception("No ENVIRONMENT defined");
  }
}

/**
 * Converts an acquia environment flag to a standardized environment.
 *
 * @param bool $env_map
 *   The mapping of acquia environments to the standard environments.
 * @return bool
 *    false if no environment found
 * @throws \Exception
 *    If an acquia environment is found, but there is no mapping, you'll need to setup a custom mapping.
 */
function devinci_get_acquia_env($env_map) {
  // Check for acquia environment settings.
  if (isset($_ENV['AH_SITE_ENVIRONMENT'])) {
    if (in_array($_ENV['AH_SITE_ENVIRONMENT'], $env_map)) {
      return $env_map[$_ENV['AH_SITE_ENVIRONMENT']];
    }
    else {
      throw new Exception("Acquia environment mapping not found: {$_ENV['AH_SITE_ENVIRONMENT']}");
    }
  }
  // No acquia environment found (not on acquia).
  return false;
}

/**
 * Converts a pantheon environment flag to a standardized environment.
 *
 * @param bool $env_map
 *   The mapping of pantheon environments to the standard environments.
 * @return bool
 *    false if no environment found
 * @throws \Exception
 *    If a pantheon environment is found, but there is no mapping, you'll need to setup a custom mapping.
 */
function devinci_get_pantheon_env($env_map) {
  // Check for pantheon environment settings.
  if (isset($_ENV["PANTHEON_ENVIRONMENT"])) {
    if (in_array($_ENV["PANTHEON_ENVIRONMENT"], $env_map)) {
      return $env_map[$_ENV["PANTHEON_ENVIRONMENT"]];
    }
    else {
      throw new Exception("Pantheon environment mapping not found: {$_ENV["PANTHEON_ENVIRONMENT"]}");
    }
  }
  // No pantheon environment found (not on pantheon).
  return false;
}
