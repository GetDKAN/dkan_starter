<?php

define('ENVIRONMENT', 'local');

$databases = array (
  'default' =>
  array (
    'default' =>
    array (
      'database' => '#DB_NAME',
      'username' => '#DB_USER',
      'password' => '#DB_PASS',
      'host' => '#DB_HOST',
      'port' => '#DB_PORT',
      'driver' => 'mysql',
      'prefix' => '',
    ),
  ),
);

$base_url = 'http://#HOST:#WEB_PORT';
