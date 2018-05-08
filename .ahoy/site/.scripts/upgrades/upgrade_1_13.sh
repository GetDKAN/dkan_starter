# Upgrade path script for 1.12 to 1.13 sites.


$drush_cmd rr

$drush_cmd dis -y menu_token remote_file_source rdf
if [ "$CI" = "true" ]; then
  $drush_cmd sql-query "truncate watchdog;"
fi

$drush_cmd fr dkan_dataset_content_types -y
$drush_cmd fr dkan_permissions -y

odfe_status=`$drush_cmd pmi --fields=status  --format=csv open_data_federal_extras`

if [ "$odfe_status" = "enabled" ]; then
  $drush_cmd fr open_data_schema_map_dkan -y
fi

$drush_cmd updatedb -y
$drush_cmd en dkan_ipe -y

$drush_cmd en dkan_harvest_dashboard -y
$drush_cmd en menu_admin_per_menu -y

$drush_cmd php-eval "dkan_sitewide_convert_panel_page('front_page', TRUE);"

$drush_cmd fra -y
$drush_cmd rr
$drush_cmd vset upgrade_1_13 upgraded

