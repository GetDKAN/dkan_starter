<?php

if (getenv('DB_1_ENV_MYSQL_DATABASE')) {
  $databases = array (
    'default' =>
    array (
      'default' =>
      array (
        'database' => getenv('DB_1_ENV_MYSQL_DATABASE'),
        'username' => getenv('DB_1_ENV_MYSQL_USER'),
        'password' => getenv('DB_1_ENV_MYSQL_PASSWORD'),
        'host' => getenv('DB_1_PORT_3306_TCP_ADDR'),
        'port' => '',
        'driver' => 'mysql',
        'prefix' => '',
      ),
    ),
  );

  # Workaround for permission issues with NFS shares in Vagrant
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

if (getenv('MEMCACHED_PORT_11211_TCP_ADDR')) {
  $memcache_server = getenv('MEMCACHED_PORT_11211_TCP_ADDR') . ':' . getenv('MEMCACHED_PORT_11211_TCP_PORT');
  $conf['memcache_servers'] = array($memcache_server => 'default');
  $conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
  $conf['cache_default_class'] = 'MemCacheDrupal';
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
}
