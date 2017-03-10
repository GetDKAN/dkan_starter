<?php
/**
 * @file
 * Admin settings.
 */

/**
 * Panels expiration settings.
 */
function expire_panels_admin_settings_form_elements(&$form, &$form_state, $form_id) {
  $form['tabs']['panels'] = array(
    '#type' => 'fieldset',
    '#title' => t('Panels expiration'),
    '#group' => 'tabs',
    '#weight' => 7,
  );

  $form['tabs']['panels']['actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Panels actions'),
  );

  $form['tabs']['panels']['actions']['expire_panels_actions'] = array(
    '#type' => 'checkboxes',
    '#description' => t('Page cache for panels will be flushed after selected actions.'),
    '#options' => array(
      EXPIRE_PANELS_DISPLAY_UPDATE => t('Panel display update'),
      EXPIRE_PANELS_DISPLAY_DELETE => t('Panel display delete'),
    ),
    '#default_value' => variable_get('expire_panels_actions', array()),
  );

  $form['tabs']['panels']['expire'] = array(
    '#type' => 'fieldset',
    '#title' => t('What URLs should be expired when panels action is triggered?'),
  );

  $form['tabs']['panels']['expire']['expire_panels_front_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Front page'),
    '#description' => t('Expire url of the site front page'),
    '#default_value' => variable_get('expire_panels_front_page', EXPIRE_PANELS_FRONT_PAGE),
  );

  $form['tabs']['panels']['expire']['expire_panels_panel_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Panels page'),
    '#description' => t('Expire url of the expiring panel page.'),
    '#default_value' => variable_get('expire_panels_panel_page', EXPIRE_PANELS_PANEL_PAGE),
  );

  // Panelizer settings.
  if (module_exists('panelizer')) {
    $form['tabs']['panels']['expire']['expire_panels_panelizer_page'] = array(
      '#type' => 'checkbox',
      '#title' => t('Panelizer page'),
      '#description' => t('Expire url of the expiring panelizer page.'),
      '#default_value' => variable_get('expire_panels_panelizer_page', EXPIRE_PANELS_PANELIZER_PAGE),
    );
  }

  $form['tabs']['panels']['expire']['expire_panels_custom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Custom pages'),
    '#description' => t('Expire user-defined custom urls.'),
    '#default_value' => variable_get('expire_panels_custom', EXPIRE_PANELS_CUSTOM),
  );

  // @codingStandardsIgnoreStart
  $form['tabs']['panels']['expire']['expire_panels_custom_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter custom URLs'),
    '#description' => t('Enter one relative URL per line. It can be the path of a node (e.g. !example1) or of any alias (e.g. !example2). However, it has to be the final URL, not a redirect (use the !link1 and !link2 modules).', array('!example1' => '<strong>user/[user:uid]</strong>', '!example2' => '<strong>my/path</strong>', '!link1' => l('Global Redirect', 'https://drupal.org/project/globalredirect'), '!link2' => l('Redirect', 'https://drupal.org/project/redirect'))) . '<br/>'
      . t('If you want to match a path with any ending, add "|wildcard" to the end of the line (see !link1 for details). Example: !example1 will match !example1a, but also !example1b, !example1c, etc.', array('!link1' => l('function cache_clear_all', 'https://api.drupal.org/api/drupal/includes%21cache.inc/function/cache_clear_all/7'), '!example1' => '<strong>my/path|wildcard</strong>', '!example1a' => '<strong>my/path</strong>', '!example1b' => '<strong>my/path/one</strong>', '!example1c' => '<strong>my/path/two</strong>')) . '<br/>'
      . t('You may use tokens here.'),
    '#states' => array(
      'visible' => array(
        ':input[name="expire_panels_custom"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => variable_get('expire_panels_custom_pages'),
  );
  // @codingStandardsIgnoreEnd
}
