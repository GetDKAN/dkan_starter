<?php
require_once "vendor/autoload.php";
require_once "util.php";

use Symfony\Component\Yaml\Yaml;

$contrib_modules_location = "docroot/profiles/dkan/modules/contrib";
$contrib_libraries_location = "docroot/profiles/dkan/libraries";

if (!file_exists("docroot/profiles/dkan")) {
  throw new \Exception("DKAN is not present in Drupal at docroot/profiles/dkan");
}

if (file_exists($contrib_modules_location)) {
  throw new \Exception("DKAN is already made.");
}

$cache_modules = TMP_DIR . "/dkan_contrib";
$cache_libs = TMP_DIR . "/dkan_libs";

if (file_exists($cache_modules) && file_exists($cache_libs)) {
  echoe("Using the cache: {$cache_modules}");
  run_drush_make_without_cached();

  `cp -r {$cache_modules} ../{$contrib_modules_location}`;
  `cp -r {$cache_libs} {$contrib_libraries_location}`;
}
else {
  `drush --root=docroot -y make --strict=0 --cache --no-core --contrib-destination=./ docroot/profiles/dkan/drupal-org.make --no-recursion --verbose docroot/profiles/dkan`;
  `cp -r {$contrib_modules_location} {$cache_modules}`;
  `cp -r {$contrib_libraries_location} {$cache_libs}`;
}

function run_drush_make_without_cached() {
  $drush_make_config = Yaml::parse(file_get_contents("docroot/profiles/dkan/drupal-org.make"));
  unset($drush_make_config['includes']);

  $contrib_modules = [];
  exec("ls -lA docroot/profiles/dkan/modules/contrib | awk '{print $9;}'", $contrib_modules);
  foreach ($contrib_modules as $contrib_module) {
    if (isset($drush_make_config['projects'][$contrib_module])) {
      unset($drush_make_config['projects'][$contrib_module]);
    }
  }

  $libs = [];
  exec("ls -lA docroot/profiles/dkan/libraries | awk '{print $9;}'", $libs);
  foreach ($libs as $lib) {
    if (isset($drush_make_config['libraries'][$lib])) {
      unset($drush_make_config['libraries'][$lib]);
    }
  }

  $cache_make = "docroot/profiles/dkan/dkan.make";
  file_put_contents($cache_make, Yaml::dump($drush_make_config));
  `drush --root=docroot -y make --strict=0 --cache --no-core --contrib-destination=./ {$cache_make} --no-recursion --verbose docroot/profiles/dkan`;
}
