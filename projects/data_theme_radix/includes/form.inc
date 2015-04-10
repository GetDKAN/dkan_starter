<?php
/**
 * @file
 * Theme and preprocess functions for forms
 */
function hhs_demo_form_alter(&$form, &$form_state, $form_id) {
  switch ($form_id) {
    case 'views_exposed_form':
    	$view = $form_state['view'];
      if ($view->name == 'dkan_datasets' && $view->current_display == 'panel_pane_1') {
        $form['query'] = array(
        	'#type' => 'textfield',
			    // Add field label for 508 compliance.
			    '#title' => 'Search',
			    '#attributes' => array('placeholder' => t('search')),
			    '#size' => 60,
			  );
			  $form['#info']['filter-search_api_views_fulltext']['description'] = '';
      } 
      break;

  }
}
