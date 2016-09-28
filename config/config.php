<?php
/**
 * @file
 * Contents generated automatically by `ahoy site config` command.
 * DO NOT EDIT.
 */
$conf = array (
  'default' => 
  array (
    'hostname' => 'localhost',
    'https_everywhere' => false,
    'https_securepages' => false,
    'clamav' => false,
    'stage_file_proxy_origin' => 'changeme',
    'fast_file' => 
    array (
      'enable' => true,
      'limit' => '10MB',
      'queue' => '50MB',
    ),
  ),
  'acquia' => 
  array (
    'subscription' => 'professional',
    'search' => 
    array (
      'host' => 'changeme',
      'id' => 'dkan_acquia_solr',
    ),
    'dev' => 
    array (
      'base_url' => 'http://devurl',
      'core_id' => 'changeme',
      'derived_key' => 'changeme',
    ),
    'test' => 
    array (
      'base_url' => 'http://testurl',
      'core_id' => 'changeme',
      'derived_key' => 'changeme',
    ),
    'ra' => 
    array (
      'base_url' => 'http://testurl',
      'core_id' => 'changeme',
      'derived_key' => 'changeme',
    ),
    'prod' => 
    array (
      'base_url' => 'http://produrl',
      'core_id' => 'changeme',
      'derived_key' => 'changeme',
    ),
  ),
);