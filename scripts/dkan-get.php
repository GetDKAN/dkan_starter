<?php
require_once "util.php";
require_once "vendor/autoload.php";

use DkanTools\Configuration;

echoe("Running dkan-get");

$exit_message = "";

if (isset($argv[1])) {
  $dkan_version = $argv[1];
}
else {
  $config = new Configuration();
  $dkan_version = $config->getDkanVersion();
}

if (!file_exists('docroot')) {
  throw new \Exception("Drupal needs to be present before getting DKAN.");
}

get_dkan_archive($dkan_version);

decompress_dkan_archive($dkan_version);

copy_dkan_to_drupal_profiles($dkan_version);

function get_dkan_archive($dkan_version) {
  prepare_tmp();

  $file_name = "{$dkan_version}.tar.gz";

  $archive = TMP_DIR . "/dkan-{$file_name}";
  $got_dkan = file_exists($archive);
  if ($got_dkan) {
    echoe("Got DKAN .tar.gz: " . bool_to_str($got_dkan));
    return;
  }

  $sources = [
    "https://github.com/GetDKAN/dkan/releases/download/{$dkan_version}/{$file_name}",
    "https://github.com/GetDKAN/dkan/archive/{$file_name}",
  ];

  $source = NULL;
  foreach ($sources as $s) {
    if (url_exists($s)) {
      $source = $s;
      break;
    }
  }

  if (!isset($source)) {
    throw new \Exception("Could not get DKAN at {$source}");
  }

  echoe("Getting DKAN from {$source}");
  `wget -O {$archive} {$source}`;
}

function decompress_dkan_archive($dkan_version) {
  $file_name = "{$dkan_version}.tar.gz";
  $archive = TMP_DIR . "/dkan-{$file_name}";
  $decompressed = TMP_DIR . "/dkan-{$dkan_version}";

  if (file_exists($decompressed)) {
    echoe("Got {$decompressed}");
    return;
  }

  if (file_exists($archive)) {
    $tmp = TMP_DIR;
    `tar -xzvf {$archive} -C {$tmp}`;
  }
  else {
    throw new \Exception("The DKAN archive {$archive} does not exist.");
  }
}

function copy_dkan_to_drupal_profiles($dkan_version) {
  $dkan = "docroot/profiles/dkan";
  $decompressed = TMP_DIR . "/dkan-{$dkan_version}";

  if (file_exists($dkan)) {
    echoe("Got {$dkan}");
    return;
  }

  if (file_exists($decompressed)) {
    `cp -r {$decompressed} {$dkan}`;
  }
  else {
    throw new \Exception("Drupal not found at {$decompressed}");
  }
}
