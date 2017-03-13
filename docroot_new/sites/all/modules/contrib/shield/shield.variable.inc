<?php
/**
 * @file
 * Variable module hook implementations
 */

/**
 * Implements hook_variable_group_info().
 */
function shield_variable_group_info() {
  $groups['shield'] = array(
    'title' => t('Shield'),
    'description' => t('Shield module variables'),
    'access' => 'administer site configuration',
    'path' => array('admin/config/system/shield'),
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function shield_variable_info($options) {
  $variable['shield_enabled'] = array(
    'title' => t('Enable shield module', array(), $options),
    'type' => 'boolean',
    'group' => 'shield',
    'multidomain' => TRUE,
  );

  // Exception
  $variable['shield_allow_cli'] = array(
    'title' => t('Allow command line access', array(), $options),
    'description' => t('When the site is accessed from command line (e.g. from Drush, cron), the shield should not work.', array(), $options),
    'type' => 'boolean',
    'group' => 'shield',
    'multidomain' => TRUE,
  );

  // Exception/address
  $variable['shield_ignored_addresses'] = array(
    'title' => t('Ignored addresses', array(), $options),
    'description' => t('Enter a list of IP addresses to ignore; one per line. Any requests from these addresses will be allowed. Leave blank to require authentication for all addresses.', array(), $options),
    'type' => 'text',
    'group' => 'shield'
  );
  $variable['shield_remote_address'] = array(
    'title' => t('Remote address key', array(), $options),
    'description' => t('Enter the key in the $_SERVER array that contains the remote server IP address.  This is REMOTE_ADDR in standard webserver configurations, but may be HTTP_X_FORWARDED_FOR if you are using a proxy such as Varnish.', array(), $options),
    'type' => 'string',
    'default' => 'REMOTE_ADDR',
    'group' => 'shield',
    'multidomain' => TRUE,
  );

  // Path
  $variable['shield_method'] = array(
    'title' => t('Shield method', array(), $options),
    'description' => t('Choose which method of shield protection you require.', array(), $options),
    'type' => 'select',
    'options' => array(1 => t('Shield all except the following paths (exclude)'), 2 => t('Shield no paths except the following (include)')),
    'default' => 1,
    'group' => 'shield',
    'multidomain' => TRUE,
  );
  $variable['shield_paths'] = array(
    'title' => t('Paths', array(), $options),
    'description' => t('According to the Shield method selected above, these paths will be either excluded from, or included in Shield protection. Leave this blank and select "exclude" to protect all paths. You can also use paths which do not hit index.php but bootstrapped, for example cron.php.', array(), $options),
    'type' => 'text',
    'group' => 'shield',
    'multidomain' => TRUE,
  );

  // Credentials
  $variable['shield_user'] = array(
    'title' => t('User', array(), $options),
    'description' => t('Leave it blank to disable authentication.', array(), $options),
    'type' => 'string',
    'group' => 'shield',
    'multidomain' => TRUE,
  );
  $variable['shield_pass'] = array(
    'title' => t('Password', array(), $options),
    'type' => 'string',
    'group' => 'shield',
    'multidomain' => TRUE,
  );

  $variable['shield_print'] = array(
    'title' => t('Authentication message', array(), $options),
    'description' => t("The message to print in the authentication request popup. You can use [user] and [pass] to print the user and the password respectively (for example: 'Hello, user: [user], pass: [pass]!'). You can leave it empty, if you don't want to print out any special message to the users.", array(), $options),
    'type' => 'string',
    'group' => 'shield',
    'multidomain' => TRUE,
  );

  return $variable;
}
