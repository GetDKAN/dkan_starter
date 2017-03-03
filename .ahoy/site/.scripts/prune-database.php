<?php
/**
 * The purpose of this script is to delete all but N datasets and their 
 * resources in order to prune the size of a DKAN site database for development 
 * purposes.
 */

include_once( dirname(__FILE__) . '/utils-prune.php');

echo "Pruning nodes.";
prune_nodes();

echo "Pruning terms.";
prune_terms();

echo "Deleting Acquia search indexes.";
db_query("DELETE FROM search_api_index where server = 'dkan_acquia_solr'");
db_query("DELETE FROM search_api_index where server = 'local_solr_server'");
db_query("DELETE FROM search_api_server where machine_name = 'dkan_acquia_solr';");
db_query("DELETE FROM search_api_server where machine_name = 'local_solr_server';");
db_query("DELETE FROM search_api_index where server IS NULL");


