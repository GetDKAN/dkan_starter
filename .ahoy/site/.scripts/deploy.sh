eval $(ahoy parse config/config.yml)
drush --root=docroot user-password 1 --password="$private_probo_password"
drush --root=docroot user-password admin --password="$private_probo_password"

if [ "$CI" = true ]; then
  drush --root=docroot search-api-clear
  drush --root=docroot php-script .ahoy/site/.scripts/delete-all-nodes.php
  drush --root=docroot search-api-enable
fi


