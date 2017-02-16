<?php
/**
 * @file
 * Contents generated automatically by `ahoy site config` command.
 * DO NOT EDIT.
 */
$conf = array (
  'default' => 
  array (
    'hostname' => 'www.example.com',
    'https_everywhere' => false,
    'https_securepages' => false,
    'clamav' => 
    array (
      'enable' => false,
    ),
    'dkan_workflow' => 
    array (
      'enabled' => false,
    ),
    'stage_file_proxy_origin' => 'changeme',
    'fast_file' => 
    array (
      'enable' => true,
      'limit' => '10MB',
      'queue' => '50MB',
    ),
  ),
  'redirectDomains' => 
  array (
    0 => 'example.com',
    1 => 'oldsite.example.com',
  ),
  'private' => 
  array (
    'aws' => 
    array (
      'scrubbed_data_url' => 'CHANGE ME',
    ),
    'probo' => 
    array (
      'password' => 'CHANGE ME',
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
  'gaClientTrackingCode' => 'UA-XXXXX-Y',
  'gaNuCivicTrackingCode' => 'UA-XXXXX-Z',
  'circle' => 
  array (
    'test_dirs' => 
    array (
      0 => 'tests/features',
      1 => 'dkan/test/features',
      2 => 'config/tests/features',
    ),
    'skip_tags' => 
    array (
      0 => 'customizable',
      1 => 'fixme',
      2 => 'testBug',
    ),
    'memory_limit' => '256M',
  ),
);