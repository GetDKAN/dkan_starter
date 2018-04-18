<?php

if (getenv('DB_1_ENV_MYSQL_DATABASE') != FALSE || getenv('MYSQL_DATABASE') != FALSE) {
  $databases = array(
    'default' =>
    array(
      'default' =>
      array(
        'database' => (getenv('MYSQL_DATABASE') != FALSE) ? getenv('MYSQL_DATABASE') : getenv('DB_1_ENV_MYSQL_DATABASE'),
        'username' => (getenv('MYSQL_USER') != FALSE) ? getenv('MYSQL_USER') : getenv('DB_1_ENV_MYSQL_USER'),
        'password' => (getenv('MYSQL_PASSWORD') != FALSE) ? getenv('MYSQL_PASSWORD') : getenv('DB_1_ENV_MYSQL_PASSWORD'),
        'host' => (getenv('DB_1_PORT_3306_TCP_ADDR') != FALSE) ? getenv('DB_1_PORT_3306_TCP_ADDR') : 'db',
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
    define('ENVIRONMENT', 'test');
  }
  else {
    // Everywhere else, use the local environment.
    define('ENVIRONMENT', 'local');
  }
}

if (getenv('HOSTNAME') == 'web' || getenv('MEMCACHED_PORT_11211_TCP_ADDR') != FALSE) {
  if (getenv('MEMCACHED_PORT_11211_TCP_ADDR') != FALSE) {
    $memcache_server = getenv('MEMCACHED_PORT_11211_TCP_ADDR') . ':' . getenv('MEMCACHED_PORT_11211_TCP_PORT');
  }
  else {
    $memcache_server = 'memcached:11211';
  }
  $conf['memcache_servers'] = array($memcache_server => 'default');
  $conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
  $conf['cache_default_class'] = 'MemCacheDrupal';
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
}
