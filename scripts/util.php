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