#!/bin/sh
#
# Cloud Hook: drush-env-switch
#

site=$1
env=$2
drush_alias=$site'.'$env
target_env=`drush @$drush_alias php-eval "echo ENVIRONMENT;"`

drush @$drush_alias env-switch $target_env --force 
drush @$drush_alias -y updb
