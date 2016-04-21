<?php
/**
 * The purpose of this script is to delete all but N datasets and their 
 * resources in order to prune the size of a DKAN site database for development 
 * purposes.
 */
module_load_include('inc', 'search_api', 'search_api.drush');
drush_search_api_disable();

$query = db_select('node', 'n');
$query->range(0, 25);
$records = $query
  ->fields('n', array('nid'))
  ->condition('type', 'dataset')
  ->condition('status', 1)
  ->orderBy('created', 'DESC')
  ->execute();

$keep_nodes = [];
foreach($records as $record) {
  $keep_nodes[] = $record->nid;
}

$query = db_select('node', 'n');
$nodes = $query
  ->fields('n', array('nid'))
  ->condition('type', 'dataset')
  ->condition('n.nid', $keep_nodes, 'NOT IN')
  ->execute();

$delete_nodes = [];

foreach($nodes as $node) {
  $dataset = node_load($node->nid);
  $resources = $dataset->field_resources['und'];
  try {
    node_delete($node->nid);
  }
  catch(Exception $e) {
    print "Skipping dataset $node->nid do to error\n";
  }
  foreach($resources as $resource) {
    try {
      node_delete($resource['target_id']);
    }
    catch(Exception $e) {
      print "Skipping resource $resource[target_id] do to error\n";
    }
  }
}
