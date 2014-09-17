#!/bin/sh

cd ../docroot
drush -y en data_disqus data_story nucivic_data publishing_workflow og_permissions_settings
drush -y dis dkan_sitewide_roles_perms
drush vset theme_default nucivic_data
drush rr
drush -y fr --force data_disqus data_story publishing_workflow
drush cc all
