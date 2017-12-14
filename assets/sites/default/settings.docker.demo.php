<?php

if (getenv('MYSQL_DATABASE')) {
  $databases = array(
    'default' =>
    array(
      'default' =>
      array(
        'database' => getenv('MYSQL_DATABASE'),
        'username' => getenv('MYSQL_USER'),
        'password' => getenv('MYSQL_PASSWORD'),
        'host' => 'db',
        'port' => '',
        'driver' => 'mysql',
        'prefix' => '',
      ),
    ),
  );

  // Workaround for permission issues with NFS shares in Vagrant.
  $conf['file_chmod_directory'] = 0777;
  $conf['file_chmod_file'] = 0666;

  // Define our environment to use.
  if (getenv('CI')) {
    // On circleCI, use the testing environment.
    define('ENVIRONMENT', 'testing');
  }
  else {
    // Everywhere else, use the local environment.
    define('ENVIRONMENT', 'local');
  }
}

if (getenv('HOSTNAME') == 'web') {
  $memcache_server = 'memcached:11211';
  $conf['memcache_servers'] = array($memcache_server => 'default');
  $conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
  $conf['cache_default_class'] = 'MemCacheDrupal';
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
}
