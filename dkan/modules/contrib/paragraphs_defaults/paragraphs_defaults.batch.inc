<?php

/**
 * Paragraphs defaults batch form callback
 */
function paragraphs_defaults_batch_form($form, &$form_state, $entity_type, $field_name, $bundle_name) {
  if (isset($form_state['values']['operation'])) {
    return paragraphs_defaults_batch_form_confirm($form_state);
  }

  $form = array();
  
  $form['info'] = array(
    '#markup' => t('Apply default paragraphs to %field_name field for items of %entity_type: %bundle_name.',
      array(
        '%entity_type' => $entity_type,
        '%bundle_name' => $bundle_name,
        '%field_name' => $field_name,
      )
    ),
  );  
  
  $form['operation'] = array(
    '#title' => t("Operation"),
    '#description' => t("Select which items to apply the defaults to."),
    '#type' => 'radios',
    '#required' => TRUE,
    '#options' => _paragraphs_defaults_get_operation_options(),
  );
  
  $form['mode'] = array(
    '#title' => t('Mode'),
    '#description' => t("Select how to deal with existing paragraphs."),
    '#type' => 'radios',
    '#options' => _paragraphs_defaults_get_mode_options(),
    '#states' => array(
      'visible' => array(
        ':input[name="operation"]' => array(
          array('value' => 'update_paragraphs'), 
          array('value' => 'update_all'),
        ),
      ),
      'required' => array(
        ':input[name="operation"]' => array(
          array('value' => 'update_paragraphs'), 
          array('value' => 'update_all'),
        ),
      ),
    ),
  );
  
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
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process now'),
  );
  
  return $form;
}

/**
 * Wrap the operation options in an array for easy string re-use.
 */
function _paragraphs_defaults_get_operation_options() {
  return array(
    'update_blank' => t("Apply defaults only to items that don't have any paragraphs yet"),
    'update_paragraphs' => t("Apply defaults only to items that already have paragraphs"),
    'update_all' => t("Apply defaults to all items"),
  );
}  

/**
 * Wrap the options in an array for easy string re-use.
 */
function _paragraphs_defaults_get_mode_options() {
  return array(
    'replace' => t("Replace existing paragraphs"),
    'append' => t("Append to existing paragraphs"),
  );
}  

/**
 * Batch confirm function.
 */
function paragraphs_defaults_batch_form_confirm($form_state) {
  $entity_type = $form_state['storage']['entity_type'];
  $field_name = $form_state['storage']['field_name'];
  $bundle_name = $form_state['storage']['bundle_name'];
  
  $description = t('You are about to apply default paragraphs to %field_name field for items of %entity_type: %bundle_name.',
    array(
      '%entity_type' => $entity_type,
      '%bundle_name' => $bundle_name,
      '%field_name' => $field_name,
    )
  );
  
  $operation_options = _paragraphs_defaults_get_operation_options();
  $mode_options = _paragraphs_defaults_get_mode_options();
  
  $description .= '<br />' . t('You have selected:') . '<ul>'; 
  $description .= '<li>' . $operation_options[$form_state['storage']['operation']] . '</li>';
  
  if ($form_state['storage']['operation'] == 'update_paragraphs' || $form_state['storage']['operation'] == 'update_all') {
    $description .= '<li>' . $mode_options[$form_state['storage']['mode']] . '</li>';
  }
  $description .= '</ul>';
  
  $description .= '<strong>' . t('Warning: there is no undo!') . '</strong>';

  // Tell the submit handler to process the form.
  $form['process'] = array('#type' => 'hidden', '#value' => 'true');
  
  // Make sure the form redirects in the end
  $form['destination'] = array('#type' => 'hidden', '#value' => 'admin/structure/paragraphs/defaults');

  return confirm_form(
    $form,
    t('Are you sure you want to proceed?'),
    current_path(),
    $description,
    t('Continue'),
    t('Cancel')
  );
}

/**
 * Submit function for paragraphs_defaults_batch_form.
 */
function paragraphs_defaults_batch_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['process'])) {
    
    $params = array(
      'entity_type' => $form_state['storage']['entity_type'],
      'field_name' => $form_state['storage']['field_name'],
      'bundle_name' => $form_state['storage']['bundle_name'],
      'operation' => $form_state['storage']['operation'],
      'mode' => $form_state['storage']['mode'],
    );

    $batch = array(
      'title'  => t('Applying paragraph defaults'),
      'operations' => array(
        array('paragraphs_defaults_batch', array($params)),
      ),
      'finished' => 'paragraphs_defaults_batch_finished',
      'file' => drupal_get_path('module', 'paragraphs_defaults') . '/paragraphs_defaults.batch.inc',
    );
  
    batch_set($batch);    
  }
  
  $form_state['storage'] = $form_state['values'];
    
  $form_state["rebuild"] = TRUE;
}

/**
 * Batch operation to apply defaults to host entities.
 */
function paragraphs_defaults_batch($params, &$context) {
  $limit = 5;
  extract($params);

  if (!isset($context['sandbox']['progress'])) {
    $context['results']['total'] = 0;
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['items_processed'] = 0;
    $context['sandbox']['items_skipped'] = 0;
    $context['sandbox']['paragraphs_added'] = 0;
    $context['sandbox']['paragraphs_deleted'] = 0;
    
    $efq = new EntityFieldQuery();
    $max = $efq
      ->entityCondition('entity_type', $entity_type)
      ->entityCondition('bundle', $bundle_name)
      ->count()
      ->execute();
    $context['sandbox']['max'] = $max;
  }
   
  $efq = new EntityFieldQuery();
  $result = $efq
    ->entityCondition('entity_type', $entity_type)
    ->entityCondition('bundle', $bundle_name)
    ->range($context['sandbox']['progress'], $limit)
    ->execute();
    
  $ids = array();
  if (isset($result[$entity_type])) {
    $ids = array_keys($result[$entity_type]);
  }

  $defaults = paragraphs_defaults_load_defaults($entity_type, $field_name, $bundle_name);
  $default_paragraphs = paragraphs_item_load_multiple($defaults);

  foreach ($ids as $entity_id) {
    $host_entity = entity_load_single($entity_type, $entity_id);

    if ($host_entity) {
      switch ($operation) {
        case 'update_all': $process = TRUE;
          break;
        case 'update_blank': $process = empty($host_entity->{$field_name});
          break;
        case 'update_paragraphs': $process = empty($host_entity->{$field_name});
          break;
        default: $process = FALSE;
      }
      
      if ($process) {
        if ($mode == 'replace') {        
          $items = field_get_items($entity_type, $host_entity, $field_name);
          if (!empty($items)) {
            $old_paragraphs = paragraphs_item_load_multiple(paragraphs_field_item_to_ids($items));
            foreach ($old_paragraphs as $old_paragraph) {
              $old_paragraph->delete();
              $context['sandbox']['paragraphs_deleted']++;
            }
            // Need to do this because otherwise we'll end up with ghost entires on the entity.
            $host_entity->{$field_name} = array();
            entity_save($entity_type, $host_entity);
          }
        } 
        
        foreach ($default_paragraphs as $default_paragraph) {
          $new_paragraph = replicate_clone_entity('paragraphs_item', $default_paragraph);
          $new_paragraph->setHostEntity($entity_type, $host_entity);
          $new_paragraph->save();
          $context['sandbox']['paragraphs_added']++;
        }
        $context['sandbox']['items_processed']++;
      }
      else {
        $context['sandbox']['items_skipped']++;
      }
      $context['results']['total']++;
    }
  }

  $context['sandbox']['progress']++;
  $context['message'] = t('Step @progress of @max. Processed: @processed, Skipped: @skipped, Paragraphs added: @added, Paragraphs deleted @deleted.', 
    array(
      '@progress' => $context['sandbox']['progress'],
      '@max' => $context['sandbox']['max'],
      '@processed' => $context['sandbox']['items_processed'],
      '@skipped' => $context['sandbox']['items_skipped'],
      '@added' => $context['sandbox']['paragraphs_added'],
      '@deleted' => $context['sandbox']['paragraphs_deleted'],
    )
  );
  
  if ($context['sandbox']['progress'] <= ($context['sandbox']['max'])) {
    $context['finished'] = $context['sandbox']['max'] == 0 ? 1 : $context['sandbox']['progress'] / ($context['sandbox']['max']);
  }
}

/**
 * Batch 'finished' callback.
 */
function paragraphs_defaults_batch_finished($success, $results, $operations) {
  if ($success) {
    // Display the number of items processed.
    drupal_set_message(t('@count items checked and updated where needed.', array('@count' => $results['total'])));
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(
      t('An error occurred while processing @operation with arguments : @args',
        array(
          '@operation' => $error_operation[0],
          '@args' => print_r($error_operation[0], TRUE),
        )
      )
    );
  }
}