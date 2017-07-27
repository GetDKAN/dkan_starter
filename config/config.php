<?php
/**
 * @file
 * Contents generated automatically by `ahoy site config` command.
 * DO NOT EDIT.
 */
$conf = array (
  'acquia' => 
  array (
    'dev' => 
    array (
      'base_url' => 'http://devurl',
      'core_id' => 'changeme',
      'derived_key' => 'changeme',
    ),
    'prod' => 
    array (
      'base_url' => 'http://produrl',
      'core_id' => 'changeme',
      'derived_key' => 'changeme',
    ),
    'ra' => 
    array (
      'base_url' => 'http://testurl',
      'core_id' => 'changeme',
      'derived_key' => 'changeme',
    ),
    'search' => 
    array (
      'host' => 'changeme',
      'id' => 'dkan_acquia_solr',
    ),
    'subscription' => 'professional',
    'test' => 
    array (
      'base_url' => 'http://testurl',
      'core_id' => 'changeme',
      'derived_key' => 'changeme',
    ),
  ),
  'behat' => 
  array (
    'contexts' => 
    array (
      'datasets' => 
      array (
        'defaults' => 
        array (
        ),
        'fields' => 
        array (
        ),
        'labels' => 
        array (
        ),
        'sets' => 
        array (
        ),
      ),
      'services' => 
      array (
        'request_fields_map' => 
        array (
        ),
      ),
    ),
  ),
  'circle' => 
  array (
    'memory_limit' => '256M',
    'skip_features' => 
    array (
    ),
    'skip_tags' => 
    array (
      0 => 'customizable',
      1 => 'fixme',
      2 => 'testBug',
    ),
    'test_dirs' => 
    array (
      0 => 'tests/features',
      1 => 'dkan/test/features',
      2 => 'config/tests/features',
    ),
  ),
  'default' => 
  array (
    'clamav' => 
    array (
      'enable' => false,
    ),
    'dkan_workflow' => 
    array (
      'enable' => false,
    ),
    'fast_file' => 
    array (
      'enable' => true,
      'limit' => '10MB',
      'queue' => '50MB',
    ),
    'hostname' => 'www.example.com',
    'https_everywhere' => false,
    'https_securepages' => false,
    'odfe' => 
    array (
      'enable' => false,
    ),
    'stage_file_proxy_files' => 
    array (
    ),
    'stage_file_proxy_origin' => 'changeme',
  ),
  'dkan_dash' => 
  array (
  ),
  'gaClientTrackingCode' => 'UA-XXXXX-Y',
  'gaNuCivicTrackingCode' => 'UA-XXXXX-Z',
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
  'redirectDomains' => 
  array (
    0 => 'example.com',
    1 => 'oldsite.example.com',
  ),
);