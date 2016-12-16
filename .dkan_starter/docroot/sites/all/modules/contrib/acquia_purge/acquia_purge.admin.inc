<?php

/**
 * @file
 * Admin page callbacks and theme functions for the Acquia Purge module.
 */

/**
 * Menu callback to fulfill autocompletes on manual purging forms.
 *
 * @see _acquia_purge_manualpurge_base()
 */
function acquia_purge_autocomplete($string) {
  $query_limit = 10;
  $matches = array();

  // Overwrite $string and work around Drupal buggingly ignoring slashes.
  $string = implode('/', array_slice(explode('/', $_GET['q']), 1));

  // Scan for node/ lookups as they're quite common.
  if (drupal_substr($string, 0, 1) == 'n') {
    $query = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->range(0, $query_limit);

    // Lookup node IDs if the user entered node/, else limit on 15.
    if (drupal_strlen($string) > 5) {
      $string = (int) str_replace('node/', '', $string);
      $query->condition('n.nid', '%' . db_like($string) . '%', 'LIKE');
    }

    $results = $query->execute();
    foreach ($results as $nid) {
      $path = 'node/' . $nid->nid;
      $matches[] = $path;
    }
  }

  // Scan for URL aliases when the path module is enabled.
  if (module_exists('path')) {
    $query = db_select('url_alias', 'u')
      ->fields('u', array('alias'))
      ->condition('alias', db_like($string) . '%', 'LIKE')
      ->range(0, $query_limit)
      ->execute();
    foreach ($query as $alias) {
      $matches[] = $alias->alias;
    }
  }

  // Scan for menu_router paths, as long as they don't contain % characters.
  if (module_exists('menu')) {
    $query = db_select('menu_router', 'm')
      ->fields('m', array('path'))
      ->condition('path', db_like($string) . '%', 'LIKE')
      ->condition('path', 'acquia_purge_ajax_autocomplete', '!=')
      ->condition('path', 'acquia_purge_ajax_processor', '!=')
      ->condition('path', 'admin%', 'NOT LIKE')
      ->range(0, $query_limit)
      ->execute();
    foreach ($query as $path) {
      if (strpos($path->path, '%') !== FALSE) {
        continue;
      }
      $matches[] = $path->path;
    }
  }

  // Sort the values alphabetically.
  sort($matches);

  // Rewrite the matches array by copying the values into keys.
  $clean_url = variable_get('clean_url', 0);
  foreach ($matches as $key => $path) {
    if (!$clean_url) {
      $path = "index.php?q=$path";
    }
    unset($matches[$key]);
    $matches[$path] = $path;
  }

  // Return the output JSON'ed.
  drupal_json_output($matches);
}

/**
 * UX rich form that lets administrative users purge paths manually.
 */
function acquia_purge_manualpurge_form_full($form, &$form_state) {

  // Do not render the full form if error conditions are found.
  if (!_acquia_purge_are_we_allowed_to_purge()) {
    include_once DRUPAL_ROOT . '/includes/install.inc';
    return array(
      'status' => array(
        '#type' => 'item',
        '#title' => t("We're sorry, but due a critical error condition it is not possible to use this form. Please contact your administrator."),
        '#markup' => theme(
          'status_report',
          array(
            'requirements' => _acquia_purge_get_diagnosis(ACQUIA_PURGE_SEVLEVEL_ERROR),
          )
        ),
      ),
    );
  }

  // Retrieve the base form and register our validation and submit callbacks.
  _acquia_purge_manualpurge_base($form, $form_state);
  $form['#validate'] = array('_acquia_purge_manualpurge_validate');
  $form['#submit'] = array('_acquia_purge_manualpurge_submit');
  unset($form['submits']['page']);

  // Start adding on-screen documentation.
  $form['paths']['#prefix'] .= '<h3>' . t('Paths to be purged:') . '</h3>';
  $form['description'] = array(
    '#markup' => t('<p>This form allows you to purge one or more paths from your
    Acquia Cloud load balancers. This form is not intended for day-to-day use
    and only meant for site administrators, for instance in emergency cases when
    a outdated copy of a page remains being served. It is highly recommended to
    automate these purges using rules so editorial users can rely on a fully
    fresh site instead of relying on this form.</p>'),
  );

  // Define a list of example paths so users know what they can/should purge.
  $examples = array(
    '&lt;front&gt;',
    '&lt;front&gt;?page=0',
    '&lt;front&gt;/',
    'node/1',
    'node/1/',
    'news?page=0',
  );
  if (_acquia_purge_variable('acquia_purge_variations')) {
    $examples = array('&lt;front&gt;', 'node/1', 'news');
  }

  // Build up table rows with domains on the left and examples on the right.
  $domains = _acquia_purge_get_domains($form, $form_state);
  $rows = array();
  $rowsc = count($domains);
  if (count($examples) > count($domains)) {
    $rowsc = count($examples);
  }
  for ($i = 0; $i < $rowsc; $i++) {
    $row = array();
    $row[0] = isset($domains[$i]) ? $domains[$i] : '';
    $row[1] = isset($examples[$i]) ? $examples[$i] : '';
    $rows[] = $row;
  }
  $form['guidancetable'] = array(
    '#theme' => 'table',
    '#header' => array(t('Domains to purge paths on'), t('Example paths')),
    '#rows' => $rows,
    '#empty' => '',
  );

  // Decorate the textfields and prepend URL's as helpful guide.
  $schemes = _acquia_purge_get_protocol_schemes();
  $base_path = _acquia_purge_variable('acquia_purge_base_path');
  $prefix = sprintf('<b>%s://%s%s</b>', $schemes[0], $domains[0], $base_path);
  foreach ($form['paths']['path'] as $i => $item) {
    $form['paths']['path'][$i]['#field_prefix'] = $prefix;
    $form['paths']['path'][$i]['#size'] = 30;
  }

  return $form;
}

/**
 * Bare bones form that lets administrative users purge paths manually.
 */
function acquia_purge_manualpurge_form_paths($form, &$form_state) {
  if (!_acquia_purge_are_we_allowed_to_purge()) {
    return array();
  }

  // Retrieve the base form and register our validation and submit callbacks.
  _acquia_purge_manualpurge_base($form, $form_state);
  $form['#validate'] = array('_acquia_purge_manualpurge_validate');
  $form['submits']['paths']['#submit'] = array('_acquia_purge_manualpurge_submit');
  unset($form['submits']['page']);

  // Decorate the path items.
  foreach ($form['paths']['path'] as $i => $item) {
    $form['paths']['path'][$i]['#size'] = 20;
  }

  return $form;
}

/**
 * Single button form that lets administrative users purge the current page.
 */
function acquia_purge_manualpurge_form_page($form, &$form_state) {
  if (!_acquia_purge_are_we_allowed_to_purge()) {
    return array();
  }

  // Retrieve the base form and register our validation and submit callbacks.
  _acquia_purge_manualpurge_base($form, $form_state);
  $form['submits']['page']['#submit'] = array('_acquia_purge_manualpurge_submit');
  unset($form['submits']['paths']);

  // Rewrite the paths array to just contain this page, variations might be
  // added later on if _acquia_purge_input_path_variations() gets called.
  $request_uri = str_replace('##' . base_path(), '', '##' . request_uri());
  $form['paths'] = array();
  $form['paths']['path'][] = array(
    '#type' => 'hidden',
    '#default_value' => $request_uri,
  );

  return $form;
}

/**
 * A (base)form that lets users purge manually.
 */
function _acquia_purge_manualpurge_base(&$form, &$form_state) {
  $form['#tree'] = TRUE;
  $form['paths'] = array(
    '#prefix' => '<div id="paths-wrapper">',
    '#suffix' => '</div>',
    '#weight' => 5,
  );

  // Generate the path textfields, add and remove buttons.
  if (empty($form_state['num_paths'])) {
    $form_state['num_paths'] = 1;
  }
  for ($i = 0; $i < $form_state['num_paths']; $i++) {
    $form['paths']['path'][$i] = array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'acquia_purge_ajax_autocomplete',
    );
  }
  $form['paths']['add_path'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#submit' => array('_acquia_purge_manualpurge_add'),
    '#ajax' => array(
      'callback' => '_acquia_purge_manualpurge_paths',
      'wrapper' => 'paths-wrapper',
    ),
  );
  if ($form_state['num_paths'] > 1) {
    $form['paths']['remove_path'] = array(
      '#type' => 'submit',
      '#value' => t('Remove last item'),
      '#submit' => array('_acquia_purge_manualpurge_remove'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => '_acquia_purge_manualpurge_paths',
        'wrapper' => 'paths-wrapper',
      ),
    );
  }

  // Render the submit button and return the form.
  $form['submits'] = array('#weight' => 10);
  $form['submits']['paths'] = array(
    '#type' => 'submit',
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#value' => _acquia_purge_variable('acquia_purge_cron') ? t("Queue") : t("Refresh"),
  );
  $form['submits']['page'] = array(
    '#type' => 'submit',
    '#value' => t("Refresh this page"),
  );

  return $form;
}

/**
 * Manual base form: add.
 *
 * @see _acquia_purge_manualpurge_base()
 */
function __acquia_purge_manualpurge_add($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['num_paths']++;
}

/**
 * Manual base form: remove.
 *
 * @see _acquia_purge_manualpurge_base()
 */
function __acquia_purge_manualpurge_remove($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  if ($form_state['num_paths'] > 1) {
    $form_state['num_paths']--;
  }
}

/**
 * Manual base form: paths.
 *
 * @see _acquia_purge_manualpurge_base()
 */
function __acquia_purge_manualpurge_paths($form, &$form_state) {
  return $form['paths'];
}

/**
 * Form validation callback.
 *
 * @see acquia_purge_manualpurge_form_full()
 * @see acquia_purge_manualpurge_form_paths()
 */
function __acquia_purge_manualpurge_validate($form, &$form_state) {
  foreach ($form_state['values']['paths']['path'] as $id => $path) {
    if ($msg = _acquia_purge_input_validate($path)) {
      form_set_error('paths][path][' . $id, $msg);
    }
  }
}

/**
 * Form submit callback.
 *
 * @see acquia_purge_manualpurge_form_full()
 * @see acquia_purge_manualpurge_form_paths()
 * @see acquia_purge_manualpurge_form_page()
 */
function __acquia_purge_manualpurge_submit($form, &$form_state) {
  if (!isset($form_state['values']['paths']['path'])) {
    return;
  }
  if (!count($form_state['values']['paths']['path'])) {
    return;
  }

  // Improve the administrative experience by automatically adding variations
  // on the given paths, which includes trailing slash versions and pagination.
  if (_acquia_purge_variable('acquia_purge_variations')) {
    _acquia_purge_input_path_variations(
      $form_state['values']['paths']['path']);
  }

  // Notify when there is no other visual feedback to be expected.
  if (_acquia_purge_variable('acquia_purge_silentmode')) {
    drupal_set_message(t('The requested content will be invalidated quickly.'));
  }

  // Only submit the paths if they really contain something.
  if (count($form_state['values']['paths']['path'])) {
    _acquia_purge_service()->addPaths($form_state['values']['paths']['path']);
  }
}

/**
 * Returns HTML for the Acquia Purge progress bar widget.
 *
 * @param array $variables
 *   An associative array containing:
 *   - total: Total amount of actions initially queued for purging.
 *   - remaining: Remaining number of purge actions still to be processed.
 *   - good: Number of purge actions that have already taken place.
 *   - bad: Number of queue items that failed purging.
 *   - percent: Value between 0-100 representing the progress percentage.
 *   - running: Whether URLs are being purged or not.
 *   - purgehistory: Array with recently purged URL's.
 *
 * @ingroup themeable
 */
function theme_acquia_purge_status_bar_widget(array $variables) {

  // Determine status messages based on what the statistics tells us.
  if ($variables['locked']) {
    $message = t("Site content is being refreshed, please wait a moment...
    %remaining items to go...", array('%remaining' => $variables['remaining']));
  }
  elseif ($variables['running']) {
    $message = t("%remaining items about to be processed...", array('%remaining' => $variables['remaining']));
  }
  else {
    $message = t("Content changes are now visible for everybody!");
  }

  return theme('progress_bar', array(
    'percent' => $variables['percent'],
    'message' => $message,
    )
  );
}

/**
 * Returns HTML for the status report.
 *
 * @param array $variables
 *   Requirements array as returned by _acquia_purge_get_diagnosis().
 *
 * @see theme_status_report()
 *
 * @ingroup themeable
 */
function theme_acquia_purge_status_report(array $variables) {
  $output = '<table class="system-status-report">';
  $severities = array(
    ACQUIA_PURGE_SEVLEVEL_INFO => array(
      'title' => t('Info'),
      'class' => 'info',
    ),
    ACQUIA_PURGE_SEVLEVEL_OK => array(
      'title' => t('OK'),
      'class' => 'ok',
    ),
    ACQUIA_PURGE_SEVLEVEL_WARNING => array(
      'title' => t('Warning'),
      'class' => 'warning',
    ),
    ACQUIA_PURGE_SEVLEVEL_ERROR => array(
      'title' => t('Error'),
      'class' => 'error',
    ),
  );

  // Require all requirement constants from the installer code.
  include_once DRUPAL_ROOT . '/includes/install.inc';

  foreach ($variables as $requirement) {
    if (empty($requirement) || !is_array($requirement)) {
      continue;
    }
    $severity = isset($requirement['severity']) ? (int) $requirement['severity'] : REQUIREMENT_OK;
    $severity = $severities[$severity];

    // Output table row(s)
    if (!empty($requirement['description'])) {
      $output .= '<tr class="' . $severity['class'] . ' merge-down"><td class="status-title"><b>' . $requirement['name'] . '</b></td><td class="status-value">' . drupal_strtolower($requirement['value_plain']) . '</td></tr>';
      $output .= '<tr class="' . $severity['class'] . ' merge-up"><td colspan="3" class="status-description">' . $requirement['description'] . '</td></tr>';
    }
    else {
      $output .= '<tr class="' . $severity['class'] . '"><td class="status-title"><b>' . $requirement['name'] . '</b></td><td class="status-value">' . drupal_strtolower($requirement['value_plain']) . '</td></tr>';
    }
  }

  $output .= '</table>';
  return $output;
}

/**
 * Returns HTML for the Acquia Purge status widget.
 *
 * @param array $variables
 *   An associative array containing:
 *   - total: Total amount of actions initially queued for purging.
 *   - remaining: Remaining number of purge actions still to be processed.
 *   - good: Number of purge actions that have already taken place.
 *   - bad: Number of queue items that failed purging.
 *   - percent: Value between 0-100 representing the progress percentage.
 *   - running: Whether URLs are being purged or not.
 *   - purgehistory: Array with recently purged URL's.
 *
 * @ingroup themeable
 */
function theme_acquia_purge_status_widget(array $variables) {
  if ($variables['running']) {
    return theme('acquia_purge_status_bar_widget', $variables);
  }
  else {
    $message = t('Idle, accepting purges');
    return "<div class=\"messages status\">\n$message</div>\n";
  }
}
