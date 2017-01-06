# Upgrade path script for 1.12 to 1.13 sites.


drush rr
drush @$drush_alias fr dkan_dataset_content_types -y
drush @$drush_alias fr dkan_permissions -y

odfe_status=`drush pmi --fields=status  --format=csv open_data_federal_extras`

if [ "$odfe_status" = "enabled" ]; then
  drush @$drush_alias fr open_data_schema_map_dkan -y
fi

drush @$drush_alias updatedb -y
drush @$drush_alias en dkan_ipe -y
drush @$drush_alias en dkan_harvest_dashboard -y
drush @$drush_alias en menu_admin_per_menu -y
drush @$drush_alias fra -y
drush @$drush_alias rr

drush @$drush_alias vset upgrade_1_13 upgraded

