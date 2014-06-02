#!/bin/sh

cd ../docroot
drush -y en nudata_disqus nudata_story
drush rr
drush -y fr --force nudata_disqus
drush -y fr --force nudata_story