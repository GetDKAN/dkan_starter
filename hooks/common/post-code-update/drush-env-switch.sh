#!/bin/bash
#
# Cloud Hook: drush-env-switch
#
set -e

site=$1
env=$2
drush_alias=$site'.'$env
env_map=(
  "local:local"
  "pro:production"
  "prod:production"
  "production:production"
  "dev:development"
  "test:test"
  "ra:test"
  "testing:test"
  "lt:test"
)

for m in "${env_map[@]}"; do
  target_key=$(echo $m | cut -d: -f1)
  echo $target_key
  if [ "$target_key" = "$env" ]; then
  echo $m
    target_env=$(echo $m | cut -d: -f2)
   break
  fi
done

echo "Target environment is $target_env"

echo "Running drush rr --no-cache-clear"
drush @$drush_alias rr --no-cache-clear
echo "Truncating cache table"
drush @$drush_alias sqlq "TRUNCATE cache;"
echo "Running database update"
drush @$drush_alias updatedb -y
echo "Clearing caches"
drush @$drush_alias cc all

STOP_EXECUTION=false
if [ -f  "config/custom-deploy.sh" ]; then
  echo "Running pre deploy hook"
  source "config/custom-deploy.sh"
  if [ $STOP_EXECUTION = true ]; then
    echo "Stopping execution after pre deploy hook"
    exit 0
  fi
else
  echo "No pre deploy hook included"
fi

echo "Checking drupal boostrap."
drupal=$(drush @$drush_alias status | grep -e "Drupal bootstrap" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')

if [[ "$drupal" =~ "Successful" ]]; then
  echo "Installation detected, running deploy script"
  drush @$drush_alias en custom_config -y
  drush @$drush_alias cc all
  drush @$drush_alias -y fr --force custom_config
  drush @$drush_alias env-switch $target_env --force
  drush @$drush_alias -y updb
  DB_BASED_SEARCH=`drush @$drush_alias pmi dkan_acquia_search_solr | egrep 'disabled|not installed'`
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

if [ "$env" == "local" ]; then
  ahoy dkan create-qa-users
fi
