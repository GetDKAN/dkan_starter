<?php
/**
 * Page callback for defaults overview page
 */
function paragraphs_defaults_page() {
  $rows = array();
  foreach (field_read_fields(array('type' => 'paragraphs')) as $field) {
    foreach (field_read_instances(array('field_name' => $field['field_name'])) as $instance) {
      $rows[] = array(
        $instance['label'],
        $instance['field_name'],
        $instance['entity_type'],
        $instance['bundle'],
        l('Edit defaults', 'admin/structure/paragraphs/defaults/manage/' 
          . $instance['entity_type'] . '/' 
          . $instance['field_name'] . '/'
          . $instance['bundle']
        ),
        l('Batch update', 'admin/structure/paragraphs/defaults/batch/' 
          . $instance['entity_type'] . '/' 
          . $instance['field_name'] . '/'
          . $instance['bundle']
        ),
      );
    }
  }
  
  $header = array(t('Label'), t('Field name'), t('Entity type'), t('Bundle'), t('Edit link'), t('Batch update'));
  
  return array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );
}

/**
 * Page callback for defaults edit page.
 */
function paragraphs_defaults_edit_form($form, &$form_state, $entity_type, $field_name, $bundle_name) {
  drupal_set_title(t('Set paragraph defaults for %field_name on %entity_type: %bundle_name', array(
    '%field_name' => $field_name,
    '%entity_type' => $entity_type,
    '%bundle_name' => $bundle_name, 
  )), PASS_THROUGH);
  
  $instance = field_info_instance($entity_type, $field_name, $bundle_name);
  if (!empty($instance)) {
    $form = array();
    $form['#parents'] = array();

    // Add the paragraphs field.
    $field = field_info_field($field_name);
    $instance = field_info_instance($entity_type, $field_name, $bundle_name);
    $defaults = paragraphs_defaults_load_defaults($entity_type, $field_name, $bundle_name);
    $paragraphs = paragraphs_item_load_multiple($defaults);
    
    $host_entity = entity_create($entity_type, array('type' => $bundle_name));

    // Order any existing paragraphs in a form we can use to set the default value.
    $items = array();
    foreach ($paragraphs as $paragraph_item) {
      $items[]['entity'] = $paragraph_item;
    }
    $paragraphs_field = field_default_form($entity_type, $host_entity, $field, $instance, LANGUAGE_NONE, $items, $form, $form_state);
    $form += (array) $paragraphs_field;

    // Store vars.
    $form['entity_type'] = array(
      '#type' => 'value',
      '#value' => $entity_type,
    );
    
    $form['field_name'] = array(
      '#type' => 'value',
      '#value' => $field_name,
    );
    
    $form['bundle_name'] = array(
      '#type' => 'value',
      '#value' => $bundle_name,
    ); 

    // Add submit button.
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save defaults'),
      '#weight' => 40,
    );
      
    return $form;
  }
  else {
    drupal_set_message(t('Field does not seem to exist.'), 'error');
    return array();
  }
}

/**
 * Submit function for paragraphs_defaults_edit_form.
 */
function paragraphs_defaults_edit_form_submit($form, $form_state) {  
  if (!empty($form_state['values'])) {
    $entity_type = $form_state['values']['entity_type'];
    $field_name = $form_state['values'] ['field_name'];
    $bundle_name = $form_state['values'] ['bundle_name'];

    // Save the defaults.
    paragraphs_defaults_save_defaults($entity_type, $field_name, $bundle_name, $form_state['values'][$field_name]);
  }
}
