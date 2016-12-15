<?php
/**
 * @file
 * Shield module's admin funcions.
 */

/**
 * Generates and admin settings form.
 */
function shield_admin_settings($form, &$form_state) {
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Shield settings'),
    '#description' => t('Set up credentials for an authenticated user. You can also decide whether you want to print out the credentials or not.'),
  );

  $form['shield_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable shield module'),
    '#description' => t('Set whether shield is active or not.'),
    '#default_value' => variable_get('shield_enabled', 1),
  );

  $form['exceptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Exceptions'),
    '#description' => t('If any of these exceptions is granted, Shield protection will be disabled.'),
  );

  $form['exceptions']['shield_allow_cli'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow command line access'),
    '#description' => t('When the site is accessed from command line (e.g. from Drush, cron), the shield should not work.'),
    '#default_value' => variable_get('shield_allow_cli', 1),
  );

  $form['exceptions']['address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remote addresses white list'),
  );
  $form['exceptions']['address']['shield_ignored_addresses'] = array(
    '#type' => 'textarea',
    '#title' => t('Ignored addresses'),
    '#description' => t('Enter a list of IP addresses to ignore; one per line. Any requests from these addresses will be allowed. Leave blank to require authentication for all addresses.'),
    '#default_value' => variable_get('shield_ignored_addresses', ''),
  );

  $form['exceptions']['address']['shield_remote_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Remote address key'),
    '#description' => t('Enter the key in the $_SERVER array that contains the remote server IP address.  This is REMOTE_ADDR in standard webserver configurations, but may be HTTP_X_FORWARDED_FOR if you are using a proxy such as Varnish.'),
    '#default_value' => variable_get('shield_remote_address', 'REMOTE_ADDR'),
  );

  $form['paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path settings'),
  );
  $methods = array(1 => t('Shield all except the following paths (exclude)'), 2 => t('Shield no paths except the following (include)'));
  $form['paths']['shield_method'] = array(
    '#type' => 'radios',
    '#title' => t('Shield method'),
    '#default_value' => variable_get('shield_method', 1),
    '#options' => $methods,
    '#description' => t('Choose which method of shield protection you require.'),
  );
  $form['paths']['shield_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => t('According to the Shield method selected above, these paths will be either excluded from, or included in Shield protection. Leave this blank and select "exclude" to protect all paths. You can also use paths which do not hit index.php but bootstrapped, for example cron.php.'),
    '#default_value' => variable_get('shield_paths', ''),
  );

  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credentials'),
  );

  $form['credentials']['shield_user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#default_value' => variable_get('shield_user', ''),
    '#description' => t('Leave it blank to disable authentication.'),
  );

  $form['credentials']['shield_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('shield_pass', ''),
  );

  $form['shield_print'] = array(
    '#type' => 'textfield',
    '#title' => t('Authentication message'),
    '#description' => t("The message to print in the authentication request popup. You can use [user] and [pass] to print the user and the password respectively (for example: 'Hello, user: [user], pass: [pass]!'). You can leave it empty, if you don't want to print out any special message to the users."),
    '#default_value' => variable_get('shield_print', ''),
  );

  return system_settings_form($form);
}
