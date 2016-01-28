<?php

define('ENVIRONMENT', 'local');

$databases = array (
  'default' => 
  array (
    'default' => 
    array (
      'database' => 'drupal',
      'username' => 'root',
      'password' => 'admin123',
      'host' => '#HOST',
      'port' => '#DB_PORT',
      'driver' => 'mysql',
      'prefix' => '',
    ),
  ),
);

$base_url = 'http://#HOST:#WEB_PORT';
