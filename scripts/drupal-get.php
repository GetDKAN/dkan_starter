<?php
require_once "util.php";
require_once "vendor/autoload.php";

echoe("Running drupal-get");

if (isset($argv[1])) {
  $drupal_version = $argv[1];
}
else {
  $config_file = "dktl.yaml";
  if (file_exists($config_file)) {
    $config = Symfony\Component\Yaml\Yaml::parse(file_get_contents($config_file));
    if (isset($config['Drupal Version'])) {
      $drupal_version = $config['Drupal Version'];
    }
    else {
      $exit_message = "Drupal Version is not set in {$config_file}";
    }
  }
  else {
    $exit_message = "The first argument should be the Drupal Version.";
  }
}

if (!empty($exit_message)) {
  throw new \Exception($exit_message);
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

