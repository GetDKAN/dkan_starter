<?php

/**
 * Prunes taxonomy terms saving a default of 5 from each vocabulary.
 */
function prune_terms($number = 5) {
  $terms_to_save = array();
  // Ensures we save terms that are being used.
  $topics = db_query("SELECT DISTINCT field_topic_tid as tid FROM {field_data_field_topic} LIMIT $number")->fetchAll();
  foreach ($topics as $topic) {
    $terms_to_save[] = $topic->tid;
  }
  $tags = db_query("SELECT DISTINCT field_tags_tid as tid FROM {field_data_field_tags} LIMIT $number")->fetchAll();
  foreach ($tags as $tag) {
    $terms_to_save[] = $tag->tid;
  }
  $all_terms = array();
  // Leave all formats.
  $format_vid = 1;
  $terms = db_query("SELECT DISTINCT tid FROM {taxonomy_term_data} WHERE vid != $format_vid")->fetchAll();

  if ($number = 0)
  {
    $terms_to_save = array();
  }
  foreach ($terms as $term) {
    $all_terms[] = $term->tid;
  }
  foreach ($all_terms as $tid) {

    if (!in_array($tid,$terms_to_save)) {
      taxonomy_term_delete($tid);
    }
  }
}

/**
 * Prunes nodes.
 */
function prune_nodes($number = 25) {
  if ($number == 0) {
    db_delete('node')
      ->condition('type', array(
        'dataset', 'resource',
        'dkan_data_story', 'group',
        'page',
      ))
      ->execute();
    return;
  }

  $query = db_select('node', 'n');
  $query
    ->fields('n', array('nid'))
    ->condition('type', 'dataset');

  if ($number > 0) {
    $query->range(0, $number);
    $query->condition('status', 1);
  }

  $query->orderBy('created', 'DESC');
  
  $records = $query->execute();

  $keep_nodes = [];
  foreach($records as $record) {
    $keep_nodes[] = $record->nid;
  }

  $query = db_select('node', 'n');
  $nodes = $query
    ->fields('n', array('nid'))
    ->condition('type', array('dataset'))
    ->condition('n.nid', $keep_nodes, 'NOT IN')
    ->execute();

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
}

function truncate_db_index() {
  $query = "SELECT concat('TRUNCATE TABLE `', TABLE_NAME, '`;') as query
    FROM INFORMATION_SCHEMA.TABLES
    WHERE TABLE_NAME LIKE 'search_api_db%'";

  $records = db_query($query)->fetchAll();
  foreach($records as $record) {
    db_query($record->query)->execute();
  }
}
