eval $(ahoy parse config/config.yml)

if [ "$CI" = "true" ]; then
  private_probo_password=admin
fi
drush --root=docroot user-password 1 --password="$private_probo_password"
drush --root=docroot user-password admin --password="$private_probo_password"
