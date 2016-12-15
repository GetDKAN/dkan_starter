<?php

/**
 * @file
 *   Acquia Agent configuration page.
 */

/**
 * Hash a password according to Drupal's password_crypt and settings from remote.
 */
function _acquia_agent_hash_password_crypt($algorithm, $pass, $setting, $extra_md5 = FALSE) {
  // Server may state that password needs to be hashed with MD5 first.
  if ($extra_md5) {
    $pass = md5($pass);
  }
  // Include core's password.inc because it has _password_crypt().
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
  $hash = _password_crypt($algorithm, $pass, $setting);
  // Match the hash stored on the server which has prefix character 'U' if
  // password was first hashed with MD5.
  if ($extra_md5) {
    $hash = 'U' . $hash;
  }
  return $hash;
}

/**
 * Migrate site to Acquia Cloud
 */
function acquia_agent_migrate_page() {
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');
  if ($identifier && $key) {
    $acquia_network_address = acquia_agent_settings('acquia_network_address');
    if (acquia_agent_valid_credentials($identifier, $key, $acquia_network_address)) {
      return drupal_get_form('acquia_agent_migrate_form');
    }
    else {
      $error = acquia_agent_connection_error_message();
    }
  }
  else {
    $error = 'Missing Acquia Subscription credentials. Please enter your Acquia Subscription Identifier and Key.';
  }

  // If there was an error.
  if (isset($error)) {
    drupal_set_message(t('There was an error in communicating with Acquia.com. @err', array('@err' => $error)), 'error');
  }
  drupal_goto('admin/config/system/acquia-agent');
}

/**
 * Migration start form
 */
function acquia_agent_migrate_form($form, &$form_state) {
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');
  $data = acquia_agent_call('acquia.agent.cloud.migration.environments', array('identifier' => $identifier), $identifier, $key, variable_get('acquia_network_address', 'https://rpc.acquia.com'));

  $error = NULL;
  if ($errno = xmlrpc_errno()) {
    acquia_agent_report_xmlrpc_error();
    drupal_goto('admin/config/system/acquia-agent');
  }
  elseif (!$data || !isset($data['result'])) {
    $error = t("Server error, please submit again.");
  }
  else {
    // Response is in $data['result'].
    $result = $data['result'];
    if (!empty($result['is_error'])) {
      $error = t('Server error, unable to retrieve environments for migration');
    }
    elseif (!empty($result['body']['error'])) {
      $error = $result['body']['error'];
    }
    elseif (empty($result['body']['environments'])) {
      $error = t('Server error, unable to retrieve environments for migration');
    }
  }

  if ($error) {
    drupal_set_message($error, 'error');
    drupal_goto('admin/config/system/acquia-agent');
  }

  foreach ($result['body']['environments'] as $stage => $env) {
    $result['body']['environments'][$stage]['secret'] = base64_decode($env['secret']);
  }

  $form['envs'] = array(
    '#type' => 'value',
    '#value' => $result['body']['environments']
  );

  $envs = array();
  foreach (array_keys($result['body']['environments']) as $env) {
    $envs[$env] = drupal_ucfirst($env);
  }

  if (count($result['body']['environments']) > 1) {
    $form['environment'] = array(
      '#type' => 'select',
      '#title' => t('Select environment for migration'),
      '#options' => $envs,
      '#description' => t('Select which environment your site should be migrated to.
        Only environments that are running trunk or branch can
        be overwritten by migration. Environments running a tag are not included.')
    );
  }
  else {
    $form['environment'] = array(
      '#markup' => t('Available environment for migration: %env', array('%env' => array_pop($envs))),
    );
  }
  $form['migrate_files'] = array(
    '#type' => 'checkbox',
    '#title' => t('Migrate files directory'),
    '#description' => t('Include files directory and all files in migration. If you are experiencing migration errors it is recommended you do not send the files directory.'),
    '#default_value' => variable_get('acquia_migrate_files', 1),
  );
  $form['reduce_db_size'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reduce database export size'),
    '#description' => t('Reduce the database export size by excluding the data from cache, session, and watchdog tables. If you are experiencing migration errors this is recommended. Table structure will still be exported.'),
    '#default_value' => 0,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Migrate'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('acquia_agent_migrate_cancel_submit'),
  );

  return $form;
}

/**
 *  Main free subscription form function
 */
function acquia_agent_an_start_form($form, &$form_state, $banner) {
  $header = acquia_agent_an_info_header();

  $form = array(
    '#prefix' =>  '<div class="an-start-form">',
    'header'  => array('#markup' => $header),
    'banner'  => array('#markup' => $banner),
    '#theme'  => 'acquia_agent_banner_form',
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Main page function
 */
function acquia_agent_settings_page($arg = NULL) {
  $banner = '';
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');
  $subscription = acquia_agent_settings('acquia_subscription_name');
  $path = drupal_get_path('module', 'acquia_agent');
  $dynamic_banner = variable_get('acquia_dynamic_banner', FALSE);

  if ($dynamic_banner) {
    drupal_add_js(array('acquia_network' => array('id' => $identifier ? $identifier : FALSE)), 'setting');
    drupal_add_js(array('acquia_network' => array('url' => variable_get('acquia_banner_service', 'https://insight.acquia.com/system/acquia-banner'))), 'setting');
    $src = variable_get('acquia_banner_serve', 'https://insight.acquia.com/acquia_banner.js');
    $banner = "<script type='text/javascript' src='" . htmlentities($src) . "'></script>";
  }
  elseif (empty($key) && empty($identifier)) {
    $banner = theme('image', array('path' => $path . '/images/action.png'));
    $banner = l($banner, 'admin/config/system/acquia-agent/setup', array('html' => TRUE));
  }
  drupal_add_css($path . '/css/acquia_agent.css');

  if (empty($identifier) && empty($key) && $arg != 'setup') {
    drupal_set_title(t('Get an Acquia Cloud Free subscription'));
    return drupal_get_form('acquia_agent_an_start_form', $banner);
  }
  if (empty($identifier) && empty($key)) {
    return drupal_get_form('acquia_agent_automatic_setup_form');
  }
  else {
    if (empty($subscription)) {
      // Subscription name isn't set but key and id are is likely because
      // user has updated from Acquia Connector 2.1. Need to clear menu cache and
      // set subscription name.
      _acquia_agent_setup_subscription_name();
    }
    return drupal_get_form('acquia_agent_settings_form', $banner);
  }
}

/**
 * Automatic setup of Network key and identifier.
 */
function acquia_agent_automatic_setup_form($form, &$form_state) {
  if (isset($form_state['choose'])) {
    return _acquia_agent_automatic_setup_form_choose($form_state);
  }
  else {
    return _acquia_agent_automatic_setup_form_start($form_state);
  }
}

function _acquia_agent_automatic_setup_form_start(&$form_state) {
  $form = array(
    '#prefix' => t('Log in or <a href="!url">configure manually</a> to connect your site to Acquia.', array('!url' => url('admin/config/system/acquia-agent/credentials'))),
    'email' => array(
      '#type' => 'textfield',
      '#title' => t('Enter the email address you use to sign in to Acquia:'),
      '#required' => TRUE,
    ),
    'pass' => array(
      '#type' => 'password',
      '#title' => t('Enter your Acquia account password:'),
      '#description' => t('Your password will not be stored locally and will be sent securely to Acquia.com. <a href="!url" target="_blank">Forgot password?</a>', array('!url' => url('https://accounts.acquia.com/user/password'))),
      '#size' => 32,
      '#required' => TRUE,
    ),
    'actions' => array(
      '#type' => 'actions',
      'continue' => array(
        '#type' => 'submit',
        '#value' => t('Next'),
      ),
      'signup' => array(
        '#markup' => t('Need a subscription? <a href="!url">Get one</a>.', array('!url' => url('https://www.acquia.com/acquia-cloud-free'))),
      ),
    ),
  );
  return $form;
}

function _acquia_agent_automatic_setup_form_choose(&$form_state) {
  $options = array();
  foreach ($form_state['subscriptions'] as $credentials) {
    $options[] = $credentials['name'];
  }
  asort($options);
  $form = array(
    '#prefix' =>  t('You have multiple subscriptions available.'),
    'subscription' => array(
      '#type' => 'select',
      '#title' => t('Available subscriptions'),
      '#options' => $options,
      '#description' => t('Choose from your available subscriptions.'),
      '#required' => TRUE,
    ),
    'continue' => array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    ),
  );
  return $form;
}

function acquia_agent_automatic_setup_form_validate($form, &$form_state) {
  if (!isset($form_state['choose'])) {
    // Validate e-mail address and get account hash settings.
    $body = array(
      'email' => $form_state['values']['email'],
    );
    $authenticator = _acquia_agent_create_authenticator($body);
    $data = array('body' => $body, 'authenticator' => $authenticator);
    // Does not use acquia_agent_call() because Network identifier and key are not available.
    $server = variable_get('acquia_network_address', 'https://rpc.acquia.com');
    $result = _acquia_agent_request(acquia_agent_network_address($server), 'acquia.agent.communication.settings', $data);

    if ($errno = xmlrpc_errno() !== NULL) {
      acquia_agent_report_xmlrpc_error();
      // Set form error to prevent switching to the next page.
      form_set_error('');
    }
    elseif (!$result) {
      // Email doesn't exist.
      form_set_error('email', t("We didn't find an Acquia account with that email address."));
    }
    else {
      // Build hashed password from account password settings for further
      // XML-RPC communications with acquia.com.
      $pass = _acquia_agent_hash_password_crypt($result['algorithm'], $form_state['values']['pass'], $result['hash_setting'], $result['extra_md5']);
      $form_state['pass'] = $pass;
    }
  }
}

function acquia_agent_automatic_setup_form_submit($form, &$form_state) {
  if (isset($form_state['choose']) && isset($form_state['subscriptions'][$form_state['values']['subscription']])) {
    $sub = $form_state['subscriptions'][$form_state['values']['subscription']];
    variable_set('acquia_key', $sub['key']);
    variable_set('acquia_identifier', $sub['identifier']);
    variable_set('acquia_subscription_name', $sub['name']);
  }
  else {
    _acquia_agent_automatic_start_submit($form_state);
  }
  // Don't set message or redirect if multistep.
  if (!form_get_errors() && empty($form_state['rebuild'])) {
    // Check subscription and send a heartbeat to Acquia Insight via XML-RPC.
    // Our status gets updated locally via the return data.
    $active = acquia_agent_check_subscription();
    // Redirect to the path without the suffix.
    $form_state['redirect'] = array('admin/config/system/acquia-agent');
    cache_clear_all();
    if ($active && count($active) > 1) {
      drupal_set_message(t('<h3>Connection successful!</h3>You are now connected to Acquia Insight.'));
    }
  }
}

function _acquia_agent_automatic_start_submit(&$form_state) {
  // Make hashed password signed request to Acquia for subscriptions.
  $body = array(
    'email' => $form_state['values']['email'],
  );
  // acquia.com authenticator uses hash of client-supplied password hashed with
  // remote settings so that the hash can match. pass was hashed in
  // _acquia_agent_setup_form_validate().
  $authenticator = _acquia_agent_create_authenticator($body, $form_state['pass']);
  $data = array('body' => $body, 'authenticator' => $authenticator);
  // Does not use acquia_agent_call() because Network identifier and key are not available.
  $server = variable_get('acquia_network_address', 'https://rpc.acquia.com');

  $result = _acquia_agent_request(acquia_agent_network_address($server), 'acquia.agent.subscription.credentials', $data);

  if ($errno = xmlrpc_errno()) {
    acquia_agent_report_xmlrpc_error();
    // Set form error to prevent switching to the next page.
    form_set_error('');
  }
  elseif (!$result) {
    // Email doesn't exist
    form_set_error('email', t('Server error, please submit again.'));
  }
  elseif ($result['is_error']) {
    form_set_error('email', t('Server error, please submit again.'));
  }
  elseif (empty($result['body']['subscription'])) {
    form_set_error('email', t('No subscriptions were found for your account.'));
  }
  elseif (count($result['body']['subscription']) > 1) {
    // Multistep form for choosing from available subscriptions.
    $form_state['choose'] = TRUE;
    $form_state['subscriptions'] = $result['body']['subscription'];
    $form_state['rebuild'] = TRUE; // Force rebuild with next step.
  }
  else {
    // One subscription so set id/key pair.
    $sub = $result['body']['subscription'][0];
    variable_set('acquia_key', $sub['key']);
    variable_set('acquia_identifier', $sub['identifier']);
    variable_set('acquia_subscription_name', $sub['name']);
  }
}

function acquia_agent_settings_credentials($form, &$form_state) {
  $form = array();
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');

  $form['#prefix'] = t('Enter your <a href="!net">identifer and key</a> from your subscriptions overview or <a href="!url">log in</a> to connect your site to Acquia Insight.', array('!net' => url('https://insight.acquia.com/subscriptions'), '!url' => url('admin/config/system/acquia-agent/setup')));
  $form['acquia_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifier'),
    '#default_value' => $identifier,
    '#required' => TRUE,
  );
  $form['acquia_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Network key'),
    '#default_value' => $key,
    '#required' => TRUE,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Connect'),
  );
  $form['actions']['signup'] = array(
    '#markup' => t('Need a subscription? <a href="!url">Get one</a>.', array('!url' => url('https://www.acquia.com/acquia-cloud-free'))),
  );

  return $form;
}

/**
 * Validate credentials form submit.
 */
function acquia_agent_settings_credentials_validate($form, &$form_state) {
  // Trim all input to get rid of possible whitespace pasted from the website.
  foreach ($form_state['values'] as $key => $value) {
    $form_state['values'][$key] = trim($value);
  }
  $identifier = $form_state['values']['acquia_identifier'];
  $key = $form_state['values']['acquia_key'];
  // Validate credentials and get subscription name.
  $body = array('identifier' => $identifier);
  $data = acquia_agent_call('acquia.agent.subscription.name', $body, $identifier, $key, variable_get('acquia_network_address', 'https://rpc.acquia.com'));

  $error = NULL;
  if ($errno = xmlrpc_errno()) {
    acquia_agent_report_xmlrpc_error();
    // Set form error to prevent switching to the next page.
    form_set_error('');
  }
  elseif (!$data || !isset($data['result'])) {
    form_set_error('', t('Server error, please submit again.'));
  }
  $result = $data['result'];
  if (!empty($result['is_error'])) {
    form_set_error('', t('Server error, please submit again.'));
  }
  elseif (isset($result['body']['error'])) {
    form_set_error('', $result['body']['error']);
  }
  elseif (empty($result['body']['subscription'])) {
    form_set_error('acquia_identifier', t('No subscriptions were found.'));
  }
  else {
    // Store subscription.
    $form_state['sub'] = $result['body']['subscription'];
  }
}

/**
 * Save credentials form submissions.
 */
function acquia_agent_settings_credentials_submit($form, &$form_state) {
  variable_set('acquia_key', $form_state['values']['acquia_key']);
  variable_set('acquia_identifier', $form_state['values']['acquia_identifier']);
  variable_set('acquia_subscription_name', $form_state['sub']['site_name']);
  // Check subscription and send a heartbeat to Acquia Insight via XML-RPC.
  // Our status gets updated locally via the return data.
  $active = acquia_agent_check_subscription();
  // Redirect to the path without the suffix.
  $form_state['redirect'] = array('admin/config/system/acquia-agent');
  cache_clear_all();
  if ($active && count($active) > 1) {
    drupal_set_message(t('<h3>Connection successful!</h3>You are now connected to Acquia Insight.'));
  }
}

/**
 * Set subscription name and clear cache. Requires key and id to be set.
 */
function _acquia_agent_setup_subscription_name() {
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');

  // Get subscription name.
  $body = array('identifier' => $identifier);
  $data = acquia_agent_call('acquia.agent.subscription.name', $body, $identifier, $key, variable_get('acquia_network_address', 'https://rpc.acquia.com'));

  if ($errno = xmlrpc_errno()) {
    $error = TRUE;
  }
  elseif (!$data || !isset($data['result'])) {
    $error = TRUE;
  }
  $result = $data['result'];
  if (!empty($result['is_error'])) {
    $error = TRUE;
  }
  elseif (isset($result['body']['error'])) {
    $error = TRUE;
  }
  elseif (empty($result['body']['subscription'])) {
    $error = TRUE;
  }
  else {
    variable_set('acquia_subscription_name', $result['body']['subscription']['site_name']);
    // Rebuild menu since there are new callbacks.
    menu_rebuild();
  }
  if (isset($error)) {
    drupal_set_message(t('Unable to automatically set subscription name, please reconnect to your subscription.'), 'error');
    drupal_goto('admin/config/system/acquia-agent/setup');
  }
}

/**
 * Settings form builder function.
 */
function acquia_agent_settings_form($form, &$form_state, $banner) {
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');
  $subscription = acquia_agent_settings('acquia_subscription_name');
  // Help documentation is local unless the Help module is disabled.
  if (module_exists('help')) {
    $help_url = url('admin/help/acquia_agent');
  }
  else {
    $help_url = url('https://docs.acquia.com/network/install');
  }

  if (empty($_POST)) {
    // Check our connection to Acquia and validity of the credentials.
    $acquia_network_address = acquia_agent_settings('acquia_network_address');
    if (!acquia_agent_valid_credentials($identifier, $key, $acquia_network_address)) {
      $error_message = acquia_agent_connection_error_message();
      $ssl_available = in_array('ssl', stream_get_transports(), TRUE) && !defined('ACQUIA_DEVELOPMENT_NOSSL') && variable_get('acquia_agent_verify_peer', 0);
      if (empty($error_message) && $ssl_available) {
        $error_message = 'There was an error in validating your subscription credentials. You may want to try disabling SSL peer verification by setting the variable acquia_agent_verify_peer to false.';
      }
      drupal_set_message(t($error_message), 'error', FALSE);
    }
  }

  $form['connected'] = array(
    '#markup' => t('<h3>Connected to Acquia Insight</h3>'),
  );
  if (!empty($subscription)) {
    $form['subscription'] = array(
      '#markup' => t('Subscription: @sub <a href="!url">change</a>', array('@sub' => $subscription, '!url' => url('admin/config/system/acquia-agent/setup'))),
    );
  }
  $form['connection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Acquia Subscription Settings'),
    '#collapsible' => FALSE,
  );
  $form['connection']['acquia_dynamic_banner'] = array(
    '#type' => 'checkbox',
    '#title' => t('Receive updates from Acquia Insight'),
    '#default_value' => variable_get('acquia_dynamic_banner', FALSE),
  );

  $collapsed = FALSE;
  if (isset($_SERVER['AH_SITE_GROUP'])) { // Collapse migrate if Acquia hosting.
    $collapsed = TRUE;
  }
  $form['migrate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Acquia Cloud Migrate'),
    '#description' => t('Transfer a fully-functional copy of your site to Acquia Cloud. <a href="!url">Learn more</a>.', array('!url' => url('https://docs.acquia.com/cloud/site/import/connector'))),
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
  );
  $form['migrate']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Migrate'),
    '#submit' => array('acquia_agent_migrate_go_submit'),
  );
  $last_migration = variable_get('acquia_agent_cloud_migration', array());
  if (!empty($last_migration['db_file']) || !empty($last_migration['tar_file']) || !empty($last_migration['dir'])) {
    // Replace Upload button with Cleanup.
    unset($form['migrate']['#description']);
    $form['migrate']['#prefix'] = '<div class="messages error">' . t('Temporary files were leftover from last migration attempt.') . '</div>';
    $form['migrate']['submit']['#value'] = t('Cleanup files');
    $form['migrate']['submit']['#submit'] = array('acquia_agent_migrate_cleanup_submit');
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#submit' => array('acquia_agent_settings_submit'),
  );

  $form['banner'] = array('#markup' => $banner);
  $form['#theme'] = 'acquia_agent_banner_form';
  return $form;
}

/**
 * Submit acquia_agent_settings ssl setting.
 */
function acquia_agent_settings_submit($form, &$form_state) {
  variable_set('acquia_dynamic_banner', $form_state['values']['acquia_dynamic_banner']);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Submit handler for Migrate button on settings form.
 */
function acquia_agent_migrate_go_submit($form, &$form_state) {
  $form_state['redirect'] = array('admin/config/system/acquia-agent/migrate');
}

/**
 * Submit handler for Cancel button on migrate form.
 */
function acquia_agent_migrate_cancel_submit($form, &$form_state) {
  $form_state['redirect'] = array('admin/config/system/acquia-agent');
}

/**
 * Submit handler for Acquia Cloud migrate button.
 */
function acquia_agent_migrate_form_submit($form, &$form_state) {
  module_load_include('inc', 'acquia_agent', 'acquia_agent.migrate');

  if (empty($form_state['values']['envs'])) { // Sanity check.
    return;
  }

  $migrate_files = isset($form_state['values']['migrate_files']) ? $form_state['values']['migrate_files'] : 1;
  variable_set('acquia_migrate_files', $migrate_files);
  $reduce_db_size = !empty($form_state['values']['reduce_db_size']) ? $form_state['values']['reduce_db_size'] : 0;
  if (count($form_state['values']['envs']) > 1) {
    // Use selected environment.
    $env = $form_state['values']['envs'][$form_state['values']['environment']];
    $site_name = $form_state['values']['environment'];
  }
  else {
    $env = array_pop($form_state['values']['envs']);
    $site_name = $env;
  }

  // Prepare for migration.
  $migration = acquia_migrate_prepare($env);
  $migration['site_name'] = $site_name;
  if ($reduce_db_size) {
    $migration['no_data_tables'] = array('cache', 'cache_menu', 'cache_page', 'cache_field', 'sessions', 'watchdog');
  }

  if (isset($migration['error']) && $migration['error'] !== FALSE) {
    drupal_set_message(t('Unable to begin migration. @error', array('@error' => $migration['error'])), 'error');
    $form_state['redirect'] = array('admin/config/system/acquia-agent');
  }
  else {
    $batch = array(
      'title' => t('Acquia Cloud Migrate'),
      'operations' => array(
        array('acquia_migrate_batch_test', array($migration)),
        array('acquia_migrate_batch_db', array($migration)),
        array('acquia_migrate_batch_tar', array($migration)),
        array('acquia_migrate_batch_transmit', array($migration)),
      ),
      'init_message' => t('Preparing for migration'),
      'progress_message' => t('Completed @current of @total steps.'),
      'finished' => 'acquia_migrate_batch_finished',
      'file' => drupal_get_path('module', 'acquia_agent') . '/acquia_agent.migrate.inc',
    );
    batch_set($batch);
  }
}

function acquia_agent_migrate_cleanup_submit($form, &$form_state) {
  module_load_include('inc', 'acquia_agent', 'acquia_agent.migrate');
  $migration = variable_get('acquia_agent_cloud_migration', array());
  _acquia_migrate_cleanup($migration);
  drupal_set_message(t('Temporary files removed'));
  $form_state['redirect'] = array('admin/config/system/acquia-agent');
}

function theme_acquia_agent_banner_form($variables) {
  $form = $variables['form'];
  if (empty($form['banner'])) {
    return drupal_render_children($form);
  }
  $banner = drupal_render($form['banner']);
  $output = '<div id="an-pg-container"><div id="an-pg-form">';
  $output .= drupal_render_children($form);
  $output .= "\n</div>\n";
  $output .= '<div class="an-pg-banner" id="' . $form['#id'] . '-banner">';
  $output .= $banner;
  $output .= "\n</div>\n</div>\n";
  return $output;
}

function acquia_agent_an_info_header() {
  $path = drupal_get_path('module', 'acquia_agent');

  $l_opt = array('attributes' => array('target' => '_blank'));
  $output  = '<div class="an-wrapper">';
  $output .= '<h2 id="an-info-header">' . t('Acquia Subscription', array('@acquia-network' => 'http://acquia.com/products-services/acquia-network')) . '</h2>';
  $output .= '<p class="an-slogan">' . t('A suite of products and services to create & maintain killer web experiences built on Drupal') . '</p>';
  $output .= '<div id="an-info-box">';
  $output .=   '<div class="cell with-arrow an-left">';
  $output .=     '<h2 class="cell-title"><i>' . t('Answers you need') . '</i></h2>';
  $output .=     '<a href="http://library.acquia.com/" target="_blank">' . theme('image', array('path' => $path . '/images/icon-library.png')) . '</a>';
  $output .=     '<p class="cell-p">' . t("Tap the collective knowledge of Acquia’s technical support team & partners.") . '</p>';
  $output .=   '</div>';
  $output .=   '<div class="cell with-arrow an-center">';
  $output .=     '<h2 class="cell-title"><i>' . t('Tools to extend your site') . '</i></h2>';
  $output .=     '<a href="http://www.acquia.com/products-services/acquia-network/cloud-services" target="_blank">' . theme('image', array('path' => $path . '/images/icon-tools.png')) . '</a>';
  $output .=     '<p class="cell-p">' . t('Enhance and extend your site with an array of <a href="@services" target="_blank">services</a> from Acquia & our partners.', array('@services' => 'http://www.acquia.com/products-services/acquia-network/cloud-services')) . '</p>';
  $output .=   '</div>';
  $output .=   '<div class="cell an-right">';
  $output .=     '<h2 class="cell-title"><i>' . t('Support when you want it') . '</i></h2>';
  $output .=     '<a href="http://www.acquia.com/drupal-support" target="_blank">' . theme('image', array('path' => $path . '/images/icon-support.png')) . '</a>';
  $output .=     '<p class="cell-p">' . t("Experienced Drupalists are available to support you whenever you need it.") . '</p>';
  $output .=   '</div>';
  $output .=  '</div>';
  $output .= '</div>';
  return $output;
}

/**
 * Returns basic site information in JSON.
 */
function acquia_agent_site_status() {
  // We don't want this page cached.
  drupal_page_is_cacheable(FALSE);
  $data = array(
    'version' => '1.0',
    'data' => array(
      'maintenance_mode' => (bool) variable_get('maintenance_mode', 0),
      'cache' => (bool) variable_get('cache', 0),
      'block_cache' => (bool) variable_get('block_cache', 0),
    ),
  );

  drupal_json_output($data);
  drupal_exit();
}
