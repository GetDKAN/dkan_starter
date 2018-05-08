<?php
require_once "vendor/autoload.php";
require_once "util.php";

use Symfony\Component\Yaml\Yaml;

$contrib_modules_location = "docroot/sites/all/modules/contrib";
$contrib_libraries_location = "docroot/sites/all/libraries";

if (!file_exists("docroot")) {
  throw new \Exception("Drupal is not present at docroot");
}

if (file_exists($contrib_modules_location)) {
  throw new \Exception("Drupal contrib has already been made.");
}

$cache_modules = TMP_DIR . "/drupal_contrib";
$cache_libs = TMP_DIR . "/drupal_libs";

if (file_exists($cache_modules) && file_exists($cache_libs)) {
  echoe("Using the cache: {$cache_modules}");
  `cp -r {$cache_modules} {$contrib_modules_location}`;
  `cp -r {$cache_libs}/* {$contrib_libraries_location}/`;
  run_drush_make_without_cached();
}
else {
  `drush --root=docroot -y make --strict=0 --cache --no-core --contrib-destination=docroot/sites/all custom/custom.make --no-recursion --verbose`;
  `cp -r {$contrib_modules_location} {$cache_modules}`;
  `cp -r {$contrib_libraries_location} {$cache_libs}`;
}

function run_drush_make_without_cached() {
  $drush_make_config = Yaml::parse(file_get_contents("custom/custom.make"));

  $contrib_modules = [];
  exec("ls -lA docroot/sites/all/modules/contrib | awk '{print $9;}'", $contrib_modules);
  foreach ($contrib_modules as $contrib_module) {
    if (isset($drush_make_config['projects'][$contrib_module])) {
      unset($drush_make_config['projects'][$contrib_module]);
    }
  }

  $libs = [];
  exec("ls -lA docroot/sites/all/libraries | awk '{print $9;}'", $libs);
  foreach ($libs as $lib) {
    if (isset($drush_make_config['libraries'][$lib])) {
      unset($drush_make_config['libraries'][$lib]);
    }
  }

  $cache_make = TMP_DIR . "/drupal.make";
  file_put_contents($cache_make, Yaml::dump($drush_make_config));
  `drush --root=docroot -y make --strict=0 --cache --no-core --contrib-destination=docroot/sites/all {$cache_make} --no-recursion --verbose`;
}
