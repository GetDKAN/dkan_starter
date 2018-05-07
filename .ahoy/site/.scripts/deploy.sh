drush_cmd="drush"
if [ "$target_env" == "local" ]; then
  drush_cmd="ahoy cmd-proxy drush"
fi
echo "Running drush rr --no-cache-clear"
$drush_cmd drush rr --no-cache-clear
echo "Truncating cache table"
$drush_cmd drush sqlq "TRUNCATE cache;"
echo "Running database update"
$drush_cmd drush updatedb -y
echo "Clearing caches"
$drush_cmd drush cc all

echo "Checking drupal boostrap."
drupal=$($drush_cmd drush status | grep -e "Drupal bootstrap" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')

if [[ "$drupal" =~ "Successful" ]]; then
  echo "Installation detected, running deploy script"
  $drush_cmd drush en custom_config -y
  $drush_cmd drush cc all
  $drush_cmd drush -y fr --force custom_config
  $drush_cmd drush env-switch $target_env --force
  $drush_cmd drush -y updb
  DB_BASED_SEARCH=`$drush_cmd drush pmi dkan_acquia_search_solr | grep disabled`
  if [ -z "$DB_BASED_SEARCH" ]; then
    echo "SOLR Search, avoiding indexing data"
  else
    echo "DB Search, indexing data"
    $drush_cmd drush search-api-index datasets
    $drush_cmd drush search-api-index groups_di
    $drush_cmd drush search-api-index stories_index
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
