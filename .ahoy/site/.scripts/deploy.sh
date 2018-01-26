echo "Running drush rr --no-cache-clear"
drush @$drush_alias rr --no-cache-clear
echo "Truncating cache table"
drush @$drush_alias sqlq "TRUNCATE cache;"
echo "Running database update"
drush @$drush_alias updatedb -y
echo "Clearing caches"
drush @$drush_alias cc all

echo "Checking drupal boostrap."
drupal=$(drush @$drush_alias status | grep -e "Drupal bootstrap" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')

if [[ "$drupal" =~ "Successful" ]]; then
  echo "Installation detected, running deploy script"
  drush @$drush_alias en custom_config -y
  drush @$drush_alias cc all
  drush @$drush_alias -y fr --force custom_config
  drush @$drush_alias env-switch $target_env --force
  drush @$drush_alias -y updb
  DB_BASED_SEARCH=`drush @$drush_alias pmi dkan_acquia_search_solr | grep disabled`
  if [ -z "$DB_BASED_SEARCH" ]; then
    echo "SOLR Search, avoiding indexing data"
  else
    echo "DB Search, indexing data"
    drush @$drush_alias search-api-index groups_di
    drush @$drush_alias search-api-index stories_index
  fi
else
  echo "Installation not detected"
fi

# Extra non-acquia steps.
if [ "$target_env" == "local" ]; then
  eval $(ahoy parse config/config.yml)

  ahoy dkan create-qa-users


  if [ "$CI" = "true" ]; then
    private_probo_password=admin
  fi

  drush --root=docroot user-password 1 --password="$private_probo_password"
  drush --root=docroot user-password admin --password="$private_probo_password"
fi
