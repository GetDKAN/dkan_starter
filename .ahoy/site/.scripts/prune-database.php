<?php
/**
 * The purpose of this script is to delete all but N datasets and their 
 * resources in order to prune the size of a DKAN site database for development 
 * purposes.
 */

include_once( dirname(__FILE__) . '/utils-prune.php');

db_query("DELETE FROM search_api_index where server = 'dkan_acquia_solr'");
db_query("DELETE FROM search_api_index where server = 'local_solr_server'");
db_query("DELETE FROM search_api_server where machine_name = 'dkan_acquia_solr';");
db_query("DELETE FROM search_api_server where machine_name = 'local_solr_server';");
db_query("DELETE FROM search_api_index where server IS NULL");

module_load_include('inc', 'search_api', 'search_api.drush');
drush_search_api_disable();

prune_nodes();
prune_terms();

