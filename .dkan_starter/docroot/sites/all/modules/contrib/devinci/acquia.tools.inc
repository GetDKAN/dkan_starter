<?php

// Initialize the acquia platform

// On Acquia Cloud, this include file configures Drupal to use the correct
// database in each site environment (Dev, Stage, or Prod). To use this
// settings.php for development on your local workstation, set $db_url
// (Drupal 5 or 6) or $databases (Drupal 7) as described in comments above.
if (file_exists('/var/www/site-php')) {
  $sitegroup = getenv('AH_SITE_GROUP');
  require "/var/www/site-php/$sitegroup/$sitegroup-settings.inc";
}
