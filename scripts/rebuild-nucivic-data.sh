#!/bin/sh

cd ../docroot
drush -y en nudata_disqus nudata_story nucivic_data
drush vset theme_default nucivic_data
drush rr
drush -y fr --force nudata_disqusw
drush -y fr --force nudata_story
drush cc all
