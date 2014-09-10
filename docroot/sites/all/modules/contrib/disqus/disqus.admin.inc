<?php
// $Id: disqus.admin.inc,v 1.1.2.5.2.6 2011/01/19 16:34:36 robloach Exp $

/**
 * Menu callback; Displays the administration settings for Disqus.
 */
function disqus_admin_settings() {
  $form = array();
  $form['disqus_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Shortname'),
    '#description' => t('The website shortname that you registered Disqus with. If you registered http://example.disqus.com, you would enter "example" here.'),
    '#default_value' => variable_get('disqus_domain', ''),
  );
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 50,
  );
  $form['visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility'),
    '#group' => 'settings',
  );
  $types = node_type_get_types();
  $options = array();
  foreach ($types as $type) {
    $options[$type->type] = $type->name;
  }
  $form['visibility']['disqus_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Types'),
    '#description' => t('Apply comments to only the following node types.'),
    '#default_value' => variable_get('disqus_nodetypes', array()),
    '#options' => $options,
  );
  $form['visibility']['disqus_location'] = array(
    '#type' => 'select',
    '#title' => t('Location'),
    '#description' => t('Display the Disqus comments in the given location. When "Block" is selected, the comments will appear in the <a href="@disquscomments">Disqus Comments block</a>.', array('@disquscomments' => url('admin/structure/block'))),
    '#default_value' => variable_get('disqus_location', 'content_area'),
    '#options' => array(
      'content_area' => t('Content Area'),
      'block' => t('Block'),
    ),
  );
  $form['visibility']['disqus_weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#description' => t('When the comments are displayed in the content area, you can change the position at which they will be shown.'),
    '#default_value' => variable_get('disqus_weight', 50),
    '#options' => drupal_map_assoc(array(-100, -75, -50, -25, 0, 25, 50, 75, 100)),
    '#states' => array(
      'visible' => array(
        'select[name="disqus_location"]' => array('value' => 'content_area'),
      ),
    ),
  );
  $form['behavior'] = array(
    '#type' => 'fieldset',
    '#title' => t('Behavior'),
    '#group' => 'settings',
  );
  $form['behavior']['disqus_userapikey'] = array(
    '#type' => 'textfield',
    '#title' => t('User API Key'),
    '#description' => t('The API key of the administrator account on Disqus. You can get yours <a href="@key">here</a>.', array('@key' => 'http://disqus.com/api/get_my_key/')),
    '#default_value' => variable_get('disqus_userapikey', ''),
  );
  $form['behavior']['disqus_developer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Testing'),
    '#description' => t('When enabled, uses the <a href="http://docs.disqus.com/help/2/">disqus_developer</a> flag to tell Disqus that you are in a testing environment. Threads will not display on the public community page with this set.'),
    '#default_value' => variable_get('disqus_developer', FALSE),
  );
  return system_settings_form($form);
}
