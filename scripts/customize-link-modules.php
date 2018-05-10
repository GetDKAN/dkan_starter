<?php
require_once "util.php";

if (file_exists("custom/modules")) {
  `cd docroot/sites/all/modules && ln -s ../../../../custom/modules custom`;
}