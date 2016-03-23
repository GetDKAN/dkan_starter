#!/bin/bash
#
# Cloud Hook: drush-env-switch
#

site=$1
env=$2
drush_alias=$site'.'$env
target_env=`drush @$drush_alias php-eval "echo ENVIRONMENT;"`

drupal=$(drush @$drush_alias status | grep -e "Drupal bootstrap" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')

if [[ "$drupal" =~ "Successful" ]]; then
  echo "Installation detected, running deploy script"
  drush @$drush_alias rr
  drush cc drush
  drush @$drush_alias -y fr --force custom_config
  drush @$drush_alias env-switch $target_env --force
  drush @$drush_alias -y updb
else
  echo "Installation not detected"
fi
