<?php

/**
 * @file
 * Modifies docker configuration files using config.yml.
 */

include './.ahoy/site/vendor/autoload.php';

use Symfony\Component\Yaml\Parser;

$mysql_cnf_file = 'dkan/.ahoy/.docker/etc/mysql/my.cnf';

try {
  // Parse yaml.
  $yaml = new Parser();
  $config = $yaml->parse(file_get_contents('./config/config.yml'));

  // Update my.cnf file from config.yml
  if (isset($config['docker']) && isset($config['docker']['mysql_cnf'])) {
    $ini_file = parse_ini_file($mysql_cnf_file, TRUE);
    foreach ($config['docker']['mysql_cnf'] as $section => $key_values) {
      foreach ($key_values as $key_value) {
        list($key, $value) = explode('=', $key_value);
        $ini_file[$section][$key] = $value;
      }
    }
    write_php_ini($ini_file, $mysql_cnf_file);
  }
}
catch (Exception $e) {
  echo "An error happened trying to load the config.yml file:\n{$e->getMessage()}\n";
}

/**
 * Writes an INI file from an array.
 *
 * @param array $array
 *   Input array
 * @param string $file
 *   File to write to.
 */
function write_php_ini($array, $file)
{
  $res = array();
  foreach ($array as $key => $val) {
    if (is_array($val)) {
      $res[] = "[$key]";
      foreach ($val as $skey => $sval) {
        $res[] = "$skey = " . (is_numeric($sval) ? $sval : '"' . $sval . '"');
      }
    }
    else {
      $res[] = "$key = " . (is_numeric($val) ? $val : '"' . $val . '"');
    }
  }
  file_put_contents($file, implode("\n", $res) . "\n");
}
