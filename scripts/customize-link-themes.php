<?php
require_once "util.php";

if (file_exists("custom/themes")) {
  `cd docroot/sites/all/themes && ln -s ../../../../custom/themes custom`;
}