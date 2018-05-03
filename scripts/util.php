<?php

function bool_to_str($bool) {
  return ($bool) ? 'true' : 'false';
}

function url_exists($url) {
  $headers = @get_headers($url);
  return (substr_count($headers[0], "404") > 0) ? FALSE : TRUE;
}

function echoe($string) {
  echo $string . PHP_EOL;
}

function get_all_files_with_extension($path, $ext) {
  $files_with_extension = [];
  $subs = get_all_subdirectories($path);
  foreach ($subs as $sub) {
    $files = get_files_with_extension($sub, $ext);
    $files_with_extension = array_merge($files_with_extension, $files);
  }
  return $files_with_extension;
}

function get_files_with_extension($path, $ext) {
  $files_with_extension = [];
  $files = get_files($path);
  foreach ($files as $file) {
    $e = pathinfo($file, PATHINFO_EXTENSION);
    if ($ext == $e) {
      $files_with_extension[] = $file;
    }
  }
  return $files_with_extension;
}

function get_all_subdirectories($path) {
  $all_subs = [];
  $stack = [$path];
  while (!empty($stack)) {
    $sub = array_shift($stack);
    $all_subs[] = $sub;
    $subs = get_subdirectories($sub);
    $stack = array_merge($stack, $subs);
  }
  return $all_subs;
}


function get_subdirectories($path) {
  $directories_info = shell_table_to_array(`ls {$path} -lha | grep '^dr'`);
  $subs = [];
  foreach ($directories_info as $di) {
    if (isset($di[8])) {
      $dir = trim($di[8]);
      if ($dir != "." && $dir != "..") {
        $subs[] = "{$path}/{$dir}";
      }
    }
  }
  return $subs;
}

function get_files($path) {
  $files_info = shell_table_to_array(`ls {$path} -lha | grep -v '^dr'`);
  $files = [];
  foreach ($files_info as $fi) {
    if (isset($fi[8])) {
      $file = trim($fi[8]);
      $files[] = "{$path}/{$file}";
    }
  }
  return $files;
}


function shell_table_to_array($shell_table) {
  $final = [];
  $lines = explode(PHP_EOL, $shell_table);

  foreach ($lines as $line) {
    $parts = preg_split('/\s+/', $line);
    if (!empty($parts)) {
      $final[] = $parts;
    }
  }

  return $final;
}