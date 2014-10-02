<?php
/**
 * @file
 * Non-hook functions for the choropleth_dataset module.
 */


/**
 * Page callback for viewing the choropleth by dataset slug.
 *
 * @param string $data_set_slug
 *   String from url argument(2) representing the dataset slug.
 *
 * @return string
 *   String forming the html of the view of the page.
 */
function _choropleth_dataset_page_by_dataset_slug($data_set_slug = '') {
  $uri = current_path();
  // Strip out /choropleth to get what should be the dataset path.
  $dataset_path = str_replace('/choropleth', '', $uri);
  $nuri = drupal_get_normal_path($dataset_path);
  // Check to make sure the alias translates to a node.
  if ($uri != $nuri) {
    $nid = arg(1, $nuri);
    return _choropleth_dataset_page_by_nid($nid);
  }
  return MENU_NOT_FOUND;
}

/**
 * Page callback for viewing the choropleth by dataset nid.
 *
 * @param int $nid
 *   Integer from url argument(2) representing the node id.
 *
 * @return string
 *   String forming the html of the view of the page.
 */
function _choropleth_dataset_page_by_nid($nid = '') {
  // Attempt to load the node.
  $node = _choropleth_dataset_node_load($nid);
  if (!empty($node)) {
    drupal_set_title($node->title);
    choropleth_dataset_set_settings($nid);
    // Generate the choropleth view.
    $node_view = node_view($node, $view_mode = 'choropleth');
    $map_view_html = drupal_render($node_view);

    // Define the embedable url, based on config settings.
    $embed_domain = variable_get('choropleth_embed_domain', FALSE);
    if ($embed_domain != FALSE) {
      $embed_url = url(variable_get('choropleth_embed_domain') . '/choropleth-map/' . $node->nid, array(
        'absolute' => TRUE,
        'external' => TRUE,
      ));
    }
    else {
      $embed_url = url('choropleth-map/' . $node->nid, array('absolute' => TRUE));
    }

    $page_html = theme('choropleth-dataset-page', array(
      'node' => $node,
      'dataset_url' => url('node/' . $nid),
      'choroplethable' => choropleth_dataset_choroplethable($node),
      'choropleth' => theme('choropleth-dataset-map-wrapper', array(
        'node' => $node,
        'dataset_url' => url('node/' . $node->nid, array('absolute' => TRUE)),
        'embedable_url' => $embed_url,
        'choropleth' => theme('choropleth-dataset-map-view', array('node' => $node, 'map_view_html' => $map_view_html)),
        'privacy_policy_url' => url(variable_get('choropleth_privacy_url', '')),
        'privacy_policy_url_text' => t(variable_get('choropleth_privacy_url_text', '')),
        'privacy_policy_url_title_text' => t(variable_get('choropleth_privacy_url_title_text', '')),
        )),
    ));
    return $page_html;
  }
  else {
    return MENU_NOT_FOUND;
  }
}

/**
 * Page callback to render the chorpleth iframeable view from a dataset node id.
 *
 * @param int $nid
 *   The node id for a dataset node.
 *
 * @return string
 *   Html of choropleth if available, or 404.
 */
function _choropleth_map_page_by_nid($nid = '') {
  $page_html = '';
  // Attempt to load the node.
  $node = _choropleth_dataset_node_load($nid);
  if (!empty($node)) {
    drupal_set_title($node->title);
    choropleth_dataset_set_settings($nid);
    // Generate the choropleth view.
    $map_view_html = drupal_render(node_view($node, $view_mode = 'choropleth'));
    drupal_add_css(drupal_get_path('module', 'choropleth_dataset') . '/css/choropleth_dataset_iframe.css', array(
      'group' => CSS_THEME,
      'weight' => 900,
      'every_page' => FALSE,)
       );

    // Define the embedable url, based on config settings.
    $embed_domain = variable_get('choropleth_embed_domain', FALSE);
    if ($embed_domain != FALSE) {
      $embed_url = url(variable_get('choropleth_embed_domain') . '/choropleth-map/' . $node->nid, array(
        'absolute' => TRUE,
        'external' => TRUE,
      ));
    }
    else {
      $embed_url = url('choropleth-map/' . $node->nid, array('absolute' => TRUE));
    }

    $page_html .= theme('choropleth-dataset-iframe', array(
      'node' => $node,
      'choroplethable' => choropleth_dataset_choroplethable($node),
      'choropleth' => theme('choropleth-dataset-map-wrapper', array(
        'node' => $node,
        'dataset_url' => url('node/' . $node->nid, array('absolute' => TRUE)),
        'embedable_url' => $embed_url,
        'choropleth' => theme('choropleth-dataset-map-view', array(
          'node' => $node,
          'map_view_html' => $map_view_html,
          )),
        'privacy_policy_url' => url(variable_get('choropleth_privacy_url', '')),
        'privacy_policy_url_text' => t(variable_get('choropleth_privacy_url_text', '')),
        'privacy_policy_url_title_text' => t(variable_get('choropleth_privacy_url_title_text', '')),
        )),
      ));
    return $page_html;
  }
  else {
    return MENU_NOT_FOUND;
  }
}

/**
 * Load a DKAN resource with a resource nid.
 *
 * @param int $nid
 *   The node id for a resource node.
 *
 * @return mixed
 *   - The node object if it is a resource.
 *   - FALSE if it is not.
 */
function _choropleth_resource_node_load($nid = '') {
  // Simple check to see if the $nid provided is nid-like.
  if (!empty($nid) && (ctype_digit(strval($nid)))) {
    // Attempt to load the node.
    $node = entity_load('node', array($nid));
    $node = array_shift($node);
    if ((!empty($node)) && ($node->type == 'resource')) {
      return $node;
    }
    else {
      return FALSE;
    }
  }
}

/**
 * Load DKAN resources with a dataset nid.
 *
 * @param int $nid
 *   The node id for a dataset node.
 *
 * @return mixed
 *   - The node object if it is a dataset.
 *   - FALSE if it is not.
 */
function _choropleth_dataset_node_load($nid = '') {
  // Simple check to see if the $nid provided is nid-like.
  if (!empty($nid) && (ctype_digit(strval($nid)))) {
    // Attempt to load the node.
    $node = entity_load('node', array($nid));
    $node = array_shift($node);
    if ((!empty($node)) && ($node->type == 'dataset') && isset($node->field_resources)) {
      return $node;
    }
    else {
      return FALSE;
    }
  }
}

/**
 * Adds nid, embedable_url and dataset_url to Drupal.settings.
 *
 * @param int $nid
 *   The node id of the dataset.
 */
function choropleth_dataset_set_settings($nid = '') {
  if (!empty($nid) && (ctype_digit(strval($nid)))) {
    global $_choropleth_dataset_iframe_path_slug;

    // Define the embedable url, based on config settings.
    $embed_domain = variable_get('choropleth_embed_domain', FALSE);
    if ($embed_domain != FALSE) {
      $embed_url = url(variable_get('choropleth_embed_domain') . '/' . $_choropleth_dataset_iframe_path_slug . '/' . $nid, array('absolute' => TRUE, 'external' => TRUE));
    }
    else {
      $embed_url = url($_choropleth_dataset_iframe_path_slug . '/' . $nid, array('absolute' => TRUE));
    }

    drupal_add_js(array(
        'choropleth_dataset' => array(
          'nid' => $nid,
          'embedable_url' => $embed_url,
          'dataset_url' => url("node/$nid", array('absolute' => TRUE)),
        )), 'setting');
  }
}

/**
 * See if at least one resource in the dataset is set to show a choropleth map.
 *
 * @param object $node
 *   The dataset node object.
 *
 * @return bool
 *   -TRUE if at least one of the dataset resources is choroplethable.
 *   -FALSE if none of the dataset resources are choroplethable.
 */
function choropleth_dataset_choroplethable($node = NULL) {
  $choroplethable = FALSE;
  // Make sure $node is a dataset.
  if (!empty($node) && (is_object($node)) && ($node->type == 'dataset')) {
    // Load resources.
    $resources_to_load = field_get_items('node', (object) $node, 'field_resources');
    if (!empty($resources_to_load) && is_array($resources_to_load)) {
      foreach ($resources_to_load as $key => $value) {
        $resources_to_load[$key] = $value['target_id'];
      }
      if (!empty($resources_to_load)) {
        $resources = entity_load('node', $resources_to_load);
      }
    }
    // If resources exist, check for choropleth setting and set the flag = TRUE.
    if (!empty($resources)) {
      foreach ($resources as $resource) {
        if ($field_upload = field_get_items('node', $resource, 'field_upload')) {
          $field_upload = array_shift($field_upload);
          if (!empty($field_upload['choropleth'])) {
            // This resource has a choropleth display enabled.
            $choroplethable = TRUE;
          }
        }
      }
    }
  }
  return $choroplethable;
}
