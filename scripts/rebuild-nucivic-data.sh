#!/bin/bash

set -e

cd ../docroot

# Disable DKAN Modules we don't want
drush -y dis dkan_sitewide_roles_perms
drush -y dis dkan_sitewide_demo_front

# Enable components
drush -y en data_disqus data_story data_story_storyteller_role publishing_workflow og_permissions_settings
drush -y en visualization_entity_choropleth_bundle visualization_entity_geojson_bundle

# Enable NUCIVIC Data Front
drush -y en nucivic_data_demo_front

# Enable theme and set default
drush -y en nucivic_data
drush vset theme_default nucivic_data

# Revert
drush -y fr --force data_disqus data_story publishing_workflow
drush -y fr --force visualization_entity_choropleth_bundle visualization_entity_geojson_bundle

# Rebuild Permissions, Registry and clear Cache
drush php-eval 'node_access_rebuild();'
drush rr
drush cc all
