#!/bin/bash

set -e
cd ../docroot
drush -y en data_disqus data_story nucivic_data publishing_workflow og_permissions_settings
drush -y dis dkan_sitewide_roles_perms
drush vset theme_default nucivic_data
drush -y fr --force data_disqus data_story publishing_workflow
drush php-eval 'node_access_rebuild();'
drush rr
drush cc all
