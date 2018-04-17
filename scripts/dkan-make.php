<?php
require_once "util.php";

if (file_exists("./docroot/profiles/dkan")) {
  `drush --root=docroot -y make --no-core --contrib-destination=./ dkan/drupal-org.make --no-recursion --no-cache --verbose docroot/profiles/dkan`;
}
else {
  echoe("DKAN is not present in Drupal at docroot/profiles/dkan");
}