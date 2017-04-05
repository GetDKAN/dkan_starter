<?php

/**
 * @file
 * Contains the administrative page and form callbacks.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Form constructor for the module settings form.
 *
 * @see google_tag_settings_form_validate()
 * @see google_tag_settings_form_submit()
 *
 * @ingroup forms
 */
function google_tag_settings_form($form, &$form_state) {
  module_load_include('inc', 'google_tag', 'includes/variable');

  // Gather data.
  // @todo Set this on validation errors.
  $default_tab = !empty($form_state['default_tab']) ? $form_state['default_tab'] : '';

  $description = t('On this and the next two tabs, specify the conditions on which the GTM JavaScript snippet will either be included in or excluded from the page response, thereby enabling or disabling tracking and other analytics.');
  $description .= t(' All conditions must be satisfied for the snippet to be included. The snippet will be excluded if any condition is not met.<br /><br />');
  $description .= t(' On this tab, specify the path condition.');

  $groups = array(
    'general' => array('title' => t('General'), 'collapse' => FALSE),
    'path' => array('title' => t('Page paths'), 'description' => $description),
    'role' => array('title' => t('User roles'), 'description' => t('On this tab, specify the user role condition.')),
    'status' => array('title' => t('Response statuses'), 'description' => t('On this tab, specify the page response status condition.')),
    'advanced' => array('title' => t('Advanced')),
  );

  // Build form elements.
  $form['tabs'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => $default_tab ? $default_tab : 'edit-general',
    '#attributes' => array('class' => array('google-tag')),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'google_tag') . '/css/google_tag.admin.css'),
      'js' => array(drupal_get_path('module', 'google_tag') . '/js/google_tag.admin.js'),
    ),
  );
  foreach ($groups as $group => $items) {
    $form['tabs'][$group] = google_tag_fieldset($group, $items);
  }
  $form['#after_build'] = array('google_tag_settings_form_after_build');

  return system_settings_form($form);
}

/**
 * Fieldset builder for the module settings form.
 */
function google_tag_fieldset($group, $items) {
  // Gather data.
  $function = "_google_tag_variable_info_$group";
  $variables = $function(array());
  $items += array('description' => '', 'collapse' => TRUE);

  // Build form elements.
  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => $items['title'],
    '#description' => $items['description'],
    '#collapsible' => $items['collapse'],
    '#collapsed' => $items['collapse'],
    '#tree' => FALSE,
  );
  $fieldset += google_tag_form_elements($variables);

  return $fieldset;
}

/**
 * Returns form elements from variable definitions.
 *
 * @param array $variables
 *   Associative array of variable definitions.
 *
 * @return array
 *   Associative array of form elements.
 */
function google_tag_form_elements($variables) {
  static $keys = array(
    'type' => '#type',
    'title' => '#title',
    'description' => '#description',
    'options' => '#options',
    'default' => '#default_value',
  );
  $elements = array();
  foreach ($variables as $name => $variable) {
    $element = array();
    foreach ($keys as $key => $property) {
      if (isset($variable[$key])) {
        $element[$property] = $variable[$key];
      }
    }
    $element['#type'] = google_tag_form_element_type($element['#type']);
    $element['#default_value'] = variable_get($name, $variable['default']);
    $element += isset($variable['element']) ? $variable['element'] : array();
    $elements[$name] = $element;
  }

  return $elements;
}

/**
 * Converts variable type to form element type.
 *
 * @param string $type
 *   Variable type.
 *
 * @return string
 *   Form element type.
 */
function google_tag_form_element_type($type) {
  static $keys = array(
    'string' => 'textfield',
    'select' => 'radios',
    'text' => 'textarea',
    'options' => 'checkboxes',
    'boolean' => 'checkbox',
  );
  return isset($keys[$type]) ? $keys[$type] : 'textfield';
}

/**
 * Element after build callback for google_tag_settings_form().
 */
function google_tag_settings_form_after_build($element, &$form_state) {
  // This will follow:
  // variable_realm_variable_settings_form_submit
  // variable_settings_form_submit
  $element['#submit'][] = 'google_tag_settings_form_submit';
  return $element;
}

/**
 * Form validation handler for google_tag_settings_form().
 */
function google_tag_settings_form_validate($form, &$form_state) {
  $values = &$form_state['values'];

  // Trim the text values.
  $values['google_tag_container_id'] = trim($values['google_tag_container_id']);
  $values['google_tag_data_layer'] = trim($values['google_tag_data_layer']);
  google_tag_text_clean($values['google_tag_path_list']);
  google_tag_text_clean($values['google_tag_status_list']);
  google_tag_text_clean($values['google_tag_whitelist_classes']);
  google_tag_text_clean($values['google_tag_blacklist_classes']);

  // Replace all types of dashes (n-dash, m-dash, minus) with a normal dash.
  $values['google_tag_container_id'] = str_replace(array('–', '—', '−'), '-', $values['google_tag_container_id']);
  $values['google_tag_environment_id'] = str_replace(array('–', '—', '−'), '-', $values['google_tag_environment_id']);

  if (!preg_match('/^GTM-\w{4,}$/', $values['google_tag_container_id'])) {
    // @todo Is there a more specific regular expression that applies?
    // @todo Is there a way to "test the connection" to determine a valid ID for
    // a container? It may be valid but not the correct one for the website.
    form_set_error('google_tag_container_id', t('A valid container ID is case sensitive and formatted like GTM-xxxxxx.'));
  }
  if ($values['google_tag_include_environment'] && !preg_match('/^env-\d{1,}$/', $values['google_tag_environment_id'])) {
    form_set_error('google_tag_environment_id', t('A valid environment ID is case sensitive and formatted like env-x.'));
  }
  if ($message = _google_tag_verify_data_layer($values['google_tag_data_layer'])) {
    form_set_error('google_tag_data_layer', $message);
  }
}

/**
 * Form submission handler for google_tag_settings_form().
 */
function google_tag_settings_form_submit($form, &$form_state) {
  $realm_name = $realm_key = '';
  if (!empty($form['#realm_keys'])) {
    $realm_name = key($form['#realm_keys']);
    $realm_key = current($form['#realm_keys']);
    _google_tag_prepare_directory($realm_name);
  }

  _google_tag_snippets_save($realm_name, $realm_key);
  _google_tag_classes_save($realm_name, $realm_key);
}

/**
 * Prepares directory for realm specific snippet files.
 */
function _google_tag_prepare_directory($realm_name = '') {
  // From google_tag_requirements(); this should be a helper function (in core).
  if (!empty($realm_name)) {
    $directory = "public://google_tag/$realm_name";
    if (!is_dir($directory) || !is_writable($directory)) {
      file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
    }
    $is_writable = is_writable($directory);
    $is_directory = is_dir($directory);
    if (!$is_writable || !$is_directory) {
      // @todo Do something.
    }
  }
}

/**
 * Saves JS snippet files based on current settings.
 *
 * @return bool
 *   Whether the files were saved.
 */
function _google_tag_snippets_save($realm_name = '', $realm_key = '') {
  $realm_name .= $realm_name ? '/' : '';
  $realm_key .= $realm_key ? '.' : '';
  // Save the altered snippets after hook_google_tag_snippets_alter().
  module_load_include('inc', 'google_tag', 'includes/snippet');
  $result = TRUE;
  $snippets = google_tag_snippets();
  foreach ($snippets as $type => $snippet) {
    $path = file_unmanaged_save_data($snippet, "public://google_tag/{$realm_name}google_tag.$realm_key$type.js", FILE_EXISTS_REPLACE);
    $result = !$path ? FALSE : $result;
  }
  if (!$result) {
    drupal_set_message(t('An error occurred saving one or more snippet files. Please try again or contact the site administrator if it persists.'));
  }
  else {
    drupal_set_message(t('Created three snippet files based on configuration.'));
    _drupal_flush_css_js();
    drupal_clear_js_cache();
  }
}

/**
 * Stores data layer classes based on current settings.
 */
function _google_tag_classes_save($realm_name = '', $realm_key = '') {
  if (module_exists('datalayer') && $data_layer == 'dataLayer') {
    // Save classes for data layer.
    // @todo How to or can we set the name of the data layer?
    // This is an open issue on datalayer project; after it is implemented in
    // datalayer then remove second condition on if block.
    _google_tag_data_layer_snippet($classes);
    if ($realm_name) {
      variable_realm_set($realm_name, $realm_key, 'google_tag_data_layer_classes', $classes);
    }
    else {
      variable_set('google_tag_data_layer_classes', $classes);
    }
  }
}

/**
 * Cleans a string representing a list of items.
 *
 * @param string $text
 *   The string to clean.
 * @param string $format
 *   The final format of $text, either 'string' or 'array'.
 */
function google_tag_text_clean(&$text, $format = 'string') {
  $text = explode("\n", $text);
  $text = array_map('trim', $text);
  $text = array_filter($text, 'trim');
  if ($format == 'string') {
    $text = implode("\n", $text);
  }
}

/**
 * Verifies presence of dataLayer module and compares name of data layer.
 *
 * @return bool|null
 *   Whether data layer name is incompatible with dataLayer module, if present.
 */
function _google_tag_verify_data_layer($value) {
  if (module_exists('datalayer') && $value != 'dataLayer') {
    // @todo Setting form error does not allow user to save a different name,
    // i.e. to do what message text says.
    return t('The 1.1 release of the dataLayer module does not support a data layer name other than "dataLayer." If you need a different layer name, then either disable the dataLayer module or alter the JavaScript added to the page response.');
  }
}
