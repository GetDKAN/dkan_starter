<?php
/**
 * @file
 * hhs_search.features.inc
 */

/**
 * Implements hook_default_search_api_server().
 */
function dkan_acquia_search_solr_default_search_api_server() {
  $items = array();
  $items['dkan_acquia_solr'] = entity_import('search_api_server', '{
    "name" : "DKAN Acquia Solr",
    "machine_name" : "dkan_acquia_solr",
    "description" : "",
    "class" : "acquia_search_service",
    "options" : {
      "clean_ids" : true,
      "site_hash" : true,
      "scheme" : "http",
      "host" : "useast1-c1.acquia-search.com",
      "port" : "80",
      "path" : "\\/solr\\/DHIT-87770",
      "edismax" : 0,
      "modify_acquia_connection" : false,
      "acquia_override_subscription" : {
        "acquia_override_auto_switch" : 1,
        "acquia_override_selector" : "default",
        "acquia_override_subscription_id" : "DHIT-87770",
        "acquia_override_subscription_key" : "db69f4d2259f1950b73dfcfc0f5980ee",
        "acquia_override_subscription_corename" : "DHIT-87770.dev.default"
      },
      "http_user" : "",
      "http_pass" : "",
      "excerpt" : 0,
      "retrieve_data" : 0,
      "highlight_data" : 0,
      "skip_schema_check" : 0,
      "solr_version" : "",
      "http_method" : "AUTO",
      "derived_key" : "04e73d299d2795101cab16b849f98293c848a8cd"
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  $items['local_solr_server'] = entity_import('search_api_server', '{
    "name" : "Local Solr Server",
    "machine_name" : "local_solr_server",
    "description" : "",
    "class" : "search_api_solr_service",
    "options" : {
      "clean_ids" : true,
      "site_hash" : true,
      "scheme" : "http",
      "host" : "localhost",
      "port" : "8080",
      "path" : "\\/solr-hhs",
      "http_user" : "",
      "http_pass" : "",
      "excerpt" : 0,
      "retrieve_data" : 0,
      "highlight_data" : 0,
      "skip_schema_check" : 0,
      "solr_version" : "",
      "http_method" : "AUTO"
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}
