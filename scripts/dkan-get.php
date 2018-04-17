<?php
require_once "util.php";

echoe("Running dkan-get");

if (isset($argv[1])) {
  $dkan_version = $argv[1];
}
else {
  throw new \Exception("The first argument should be the dkan version.");
}

$file_name = "{$dkan_version}.tar.gz";
$destination_folder = ".";
$temp_folder = "/tmp";
$archive = "{$temp_folder}/{$file_name}";
$archive_copy = "{$destination_folder}/{$file_name}";
$archive_decompressed = "{$destination_folder}/dkan-{$dkan_version}";
$dkan = "{$destination_folder}/dkan";

$urls = [
  "https://github.com/GetDKAN/dkan/releases/download/{$dkan_version}/{$file_name}",
  "https://github.com/GetDKAN/dkan/archive/{$file_name}",
];

foreach ($urls as $url) {
  $got_dkan = file_exists($archive);
  echoe("Got DKAN .tar.gz: " . bool_to_str($got_dkan));

  if ($got_dkan) {
    break;
  }

  if (!$got_dkan && url_exists($url)) {
    echoe("Getting DKAN from {$url}");
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

if (!file_exists($dkan)) {
  `cp -r {$archive_decompressed} dkan`;
}
else {
  echoe("Got {$dkan}");
}

