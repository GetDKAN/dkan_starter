<?php
require_once "util.php";

echoe("Running drupal-get");

if (isset($argv[1])) {
  $drupal_version = $argv[1];
}
else {
  throw new \Exception("The first argument should be the drupal version.");
}

$file_name = "drupal-{$drupal_version}.tar.gz";
$destination_folder = ".";
$temp_folder = "/tmp";
$archive = "{$temp_folder}/{$file_name}";
$archive_copy = "{$destination_folder}/{$file_name}";
$archive_decompressed = "{$destination_folder}/drupal-{$drupal_version}";
$drupal = "{$destination_folder}/docroot";

$urls = [
  "https://ftp.drupal.org/files/projects/{$file_name}",
];

foreach ($urls as $url) {
  $got_drupal = file_exists($archive);
  echoe("Got Drupal .tar.gz: " . bool_to_str($got_drupal));

  if ($got_drupal) {
    break;
  }

  if (!$got_drupal && url_exists($url)) {
    echoe("Getting Drupal from {$url}");
    `wget -O {$archive} {$url}`;
    break;
  }
  else {
    echoe("{$url} does not exist.");
  }
}

if (!file_exists($archive_copy)) {
  `cp {$archive} {$destination_folder}`;
}
else {
  echoe("Got {$archive_copy}");
}

if (!file_exists($archive_decompressed)) {
  `tar -xzvf {$archive_copy}`;
}
else {
  echoe("Got {$archive_decompressed}");
}

if (!file_exists($drupal)) {
  `cp -r {$archive_decompressed} docroot`;
}
else {
  echoe("Got {$drupal}");
}

