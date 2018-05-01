<?php
require_once "util.php";

if (file_exists("./docroot") && file_exists("./custom/custom.make")) {
  `drush --root=docroot -y make --no-core --contrib-destination=docroot/sites/all custom/custom.make --no-recursion --no-cache --verbose`;
}
else {
  echoe("We do not have Drupal yet, or there is no custom/custom.make file");
}