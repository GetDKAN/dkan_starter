echo "running: .ahoy/site/.scripts/deploy.sh"
drush_cmd="drush"
if [ "$target_env" == "local" ]; then
  drush_cmd="ahoy drush"
fi
echo "Running drush rr --no-cache-clear"
${drush_cmd} rr --no-cache-clear
echo "Truncating cache table"
${drush_cmd} sqlq "TRUNCATE cache;"
echo "Running database update"
$drush_cmd updatedb -y
echo "Clearing caches"
$drush_cmd cc all

echo "Checking drupal boostrap."
drupal=$($drush_cmd status | grep -e "Drupal bootstrap" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')

if [[ "$drupal" =~ "Successful" ]]; then
  echo "Installation detected, running deploy script"
  $drush_cmd en custom_config -y
  $drush_cmd cc all
  $drush_cmd -y fr --force custom_config
  $drush_cmd env-switch $target_env --force
  $drush_cmd -y updb
  DB_BASED_SEARCH=`$drush_cmd drush pmi dkan_acquia_search_solr | grep disabled`
  if [ -z "$DB_BASED_SEARCH" ]; then
    echo "SOLR Search, avoiding indexing data"
  else
    echo "DB Search, indexing data"
    $drush_cmd search-api-index datasets
    $drush_cmd search-api-index groups_di
    $drush_cmd search-api-index stories_index
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

  $drush_cmd user-password 1 --password="$private_probo_password"
  $drush_cmd user-password admin --password="$private_probo_password"
fi
