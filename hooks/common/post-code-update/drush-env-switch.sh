#!/bin/bash
#
# Cloud Hook: drush-env-switch
#

site=$1
env=$2
drush_alias=$site'.'$env
target_env=$env

echo "Target environment is $target_env"

echo "Run pre deploy steps..."
(drush @$drush_alias cc all && drush @$drush_alias rr) || (drush @$drush_alias updb -y 2>/dev/null && drush @$drush_alias rr)

echo "Checking drupal boostrap."
drupal=$(drush @$drush_alias status | grep -e "Drupal bootstrap" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')

if [[ "$drupal" =~ "Successful" ]]; then
  echo "Installation detected, running deploy script"
  drush @$drush_alias cc all
  drush @$drush_alias -y fr --force custom_config
  drush @$drush_alias env-switch $target_env --force
  drush @$drush_alias -y updb
  DB_BASED_SEARCH=`drush @$drush_alias pmi dkan_acquia_search_solr | grep disabled`
  if [ -z "$DB_BASED_SEARCH" ]; then
    echo "SOLR Search, avoiding indexing data"
  else
    echo "DB Search, indexing data"
    drush @$drush_alias search-api-index datasets
    drush @$drush_alias search-api-index groups_di
    drush @$drush_alias search-api-index stories_index
  fi
else
  echo "Installation not detected"
fi
