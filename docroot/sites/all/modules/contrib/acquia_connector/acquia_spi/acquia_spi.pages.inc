<?php

/**
 * @file
 *   Site environment change detected form.
 */

/**
 * Acquia SPI Environment Change Actions form.
 */
function acquia_spi_environment_change_form($form, &$form_state) {
  $form               = array();
  $blocked            = variable_get('acquia_spi_blocked', FALSE);
  $acquia_hosted      = acquia_spi_check_acquia_hosted();
  $env_changes        = variable_get('acquia_spi_environment_changes');
  $env_changes_list   = theme('item_list', array('items' => $env_changes));
  $off_acquia_hosting = array_key_exists('acquia_hosted', $env_changes) && !acquia_spi_check_acquia_hosted();

  if (!acquia_spi_environment_change_detected() && !$blocked) {
    $form['#prefix'] = t('<h2>No changes detected</h2><p>This form is used to address changes in your site\'s environment. No changes are currently detected.</p>');
    return $form;
  }
  elseif ($blocked) {
    $form['env_change_action'] = array(
      '#type' => 'checkboxes',
      '#title' => t('The Acquia Connector is blocked and is not sending site profile data to Acquia Cloud for evaluation.'),
      '#options' => array(
        'unblock' => t('Unblock this site and send data to Acquia Cloud.'),
      ),
      '#required' => TRUE,
    );
  }
  else {
    if ($off_acquia_hosting) {
      drupal_set_title(t('Connect to Acquia Insight'));
    }

    $form['#prefix'] = t('<strong>The following changes have been detected in your site environment:</strong> !env_changes', array('!env_changes' => $env_changes_list));

    $form['env_change_action'] = array(
      '#type' => 'radios',
      '#title' => t('How would you like to proceed?'),
      '#options' => array(
        'block' => t('Block this site from sending profile data to Acquia Insight.'),
        'update' => t('Update existing site with these changes.'),
        'create' => t('Track this as a new site on Acquia Insight.'),
      ),
      '#required' => TRUE,
    );

    $form['identification'] = array(
      '#type' => 'fieldset',
      '#title' => t('Site Identification'),
      '#collapsible' => FALSE,
      '#states' => array(
        'visible' => array(
          ':input[name="env_change_action"]' => array('value' => 'create'),
        ),
      ),
    );

    $form['identification']['site'] = array(
      '#prefix' => '<div class="acquia-identification">',
      '#suffix' => '</div>',
      '#weight' => -2,
    );

    if ($off_acquia_hosting) {
      $form['identification']['site']['explanation'] = array(
        '#markup' => t('<p>This site is not hosted on Acquia Cloud and is not connected to Acquia Insight.</p><p>To connect it to Acquia Insight, enter a name for your site.</p>'),
      );
    }

    $form['identification']['site']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => variable_get('acquia_spi_site_name'),
    );

    $form['identification']['site']['machine_name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Machine name'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#machine_name' => array(
        'exists' => 'taxonomy_vocabulary_machine_name_load',
        'source' => array('identification', 'site', 'name'),
      ),
      '#default_value' => variable_get('acquia_spi_site_machine_name'),
    );

    if ($acquia_hosted) {
      $form['identification']['site']['name']['#disabled'] = TRUE;
      $form['identification']['site']['machine_name']['#disabled'] = TRUE;
    }
    elseif ($off_acquia_hosting) {
      unset($form['env_change_action']['#options']['block']);
      unset($form['env_change_action']['#options']['update']);
      unset($form['env_change_action']['#states']);
      unset($form['identification']['site']['name']['#default_value']);
      unset($form['identification']['site']['machine_name']['#default_value']);
      $form['env_change_action']['#default_value'] = 'create';
      $form['env_change_action']['#access'] = FALSE;
    }

  }

  $form['actions'] = array(
    '#type' => 'actions',
    'continue' => array(
      '#type' => 'submit',
      '#value' => $off_acquia_hosting ? t('Connect') : t('Submit'),
    ),
  );

  return $form;
}

/**
 * Save environment change form submissions.
 */
function acquia_spi_environment_change_form_submit($form, &$form_state) {
  if (isset($form_state['values']['env_change_action']['unblock']) && $form_state['values']['env_change_action']['unblock'] == 'unblock') {
    variable_set('acquia_spi_environment_changed_action', $form_state['values']['env_change_action']['unblock']);
  }
  else {
    variable_set('acquia_spi_environment_changed_action', $form_state['values']['env_change_action']);
  }

  if ($form_state['values']['env_change_action'] == 'create') {
    variable_set('acquia_spi_site_name', $form_state['values']['name']);
    variable_set('acquia_spi_site_machine_name', $form_state['values']['machine_name']);
  }

  // Send information as soon as action is requested.
  $response = acquia_spi_send_full_spi(ACQUIA_SPI_METHOD_CREDS);
  acquia_spi_process_messages($response);

  $form_state['redirect'] = array('admin/reports/status');
}