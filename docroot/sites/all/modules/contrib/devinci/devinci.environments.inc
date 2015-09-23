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
  global $databases, $conf;

  if ($env_map === FALSE) {
    $env_map = array(
      'local' => 'local',
      'dev' => 'development',
      'test' => 'test',
      'live' => 'production',
      'prod' => 'production',
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
    require __DIR__ . '/acquia.tools.inc';
  }
  // Check for pantheon settings.
  elseif ($pantheon_env = devinci_get_pantheon_env($env_map)) {
    $env = $pantheon_env;
    require __DIR__ . '/pantheon.tools.inc';
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
    define('ENVIRONMENT', $env);
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
  if ($acquia_env = getenv('AH_SITE_ENVIRONMENT')) {
    if (array_key_exists($acquia_env, $env_map)) {
      return $env_map[$acquia_env];
    }
    else {
      // Print a simple warning and exit.
      print "Acquia environment mapping not found: $acquia_env";
      exit();
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
  if ($pantheon_env = getenv('PANTHEON_ENVIRONMENT')) {
    if (array_key_exists($pantheon_env, $env_map)) {
      return $env_map[$pantheon_env];
    }
    else {
      // Print a simple warning and exit.
      print "Pantheon environment mapping not found: {$_ENV["PANTHEON_ENVIRONMENT"]}";
      exit();
    }
  }
  // No pantheon environment found (not on pantheon).
  return false;
}
