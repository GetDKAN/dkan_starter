<?php

/**
 * Implements hook_dkan_health_status_monitors().
 */
function dkan_health_status_dkan_health_status_monitors() {
  // Make sure user mail addresses are valid.
  $monitors['check_solr_index'] = array(
    'name' => t('Check Solr Index'),
    'description' => t('Ensures solr index matches dataset count.'),
  );
  return $monitors;
}

/**
 * Checks Solr count vs node count.
 */
function dkan_health_status_dkan_health_status_monitor_check_solr_index_count() {
  $index = search_api_index_load('datasets');
  if ($index) {
    $query = $index->query()
      ->fields(array())
      ->range(0, 0);
    $search_count = 0;
    
    $filter = $query->createFilter('AND', array('facet:type'));
    $filter->condition('type', 'dataset');
    $filter->condition('status', '1');
    $query->filter($filter);
    $response = $index->server()->search($query);
    $search_count = $response['result count'];
  }
  else {
    return FALSE;
  }
  $node_count = db_query("SELECT count(*) FROM {node} WHERE type = 'dataset' AND status = 1")->fetchField();
  return array('search_count' => $search_count, 'node_count' => $node_count); 

}

/**
 * Displays status based on node counts.
 */
function dkan_health_status_dkan_health_status_monitor_check_solr_index() {
  $count = dkan_health_status_dkan_health_status_monitor_check_solr_index_count();
  if (!$count) {
    return dkan_health_status_message(DKAN_HEALTH_ERROR, t("Dataset index could not be found."));
  }
  if ($count['node_count'] == $count['search_count']) {
    return dkan_health_status_message(DKAN_HEALTH_OKAY, t("Dataset count: !node_count matches Search count: !search_count.",
      array("!node_count" => $count['node_count'], "!search_count" => $count['search_count'])));
  }
  else {
    return dkan_health_status_message(DKAN_HEALTH_ERROR, t("Dataset count: !node_count does not match Search count: !search_count.",
      array("!node_count" => $count['node_count'], "!search_count" => $count['search_count'])));
  }
}
