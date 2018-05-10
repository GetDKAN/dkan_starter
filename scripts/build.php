<?php
require_once "util.php";

if (file_exists("docroot")) {
  echoe("Removing docroot");
  `rm -rf docroot`;
}
passthru("php /dktl/drupal-get.php");
passthru("php /dktl/dkan-get.php");
passthru("php /dktl/drupal-dkan-connect.php");
passthru("php /dktl/dkan-make.php");
passthru("php /dktl/drupal-contrib-make.php");
passthru("php /dktl/customize-link-modules.php");
passthru("php /dktl/customize-link-themes.php");
passthru("php /dktl/customize-copy.php");
passthru("php /dktl/customize-patch.php");
passthru("php /dktl/drupal-clean.php");


