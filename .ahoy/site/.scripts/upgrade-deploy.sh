drush_cmd="drush"
if [ "$target_env" == "local" ]; then
  drush_cmd="ahoy cmd-proxy drush"
fi
echo "Target environment is $target_env"

upgrade_version=upgrade_1_14
upgrade_status=`$drush_cmd drush vget $upgrade_version --exact |tr -d '\n'`

echo "The upgrade status is $upgrade_status"

if [ -z "$upgrade_status" ]; then
  upgrade_status='never upgraded';
fi


if [ "$upgrade_status" != 'upgraded' ]; then
  echo "The site was not upgraded. Running $upgrade_version.sh"
  drush_cmd=$drush_cmd bash .ahoy/site/.scripts/upgrades/$upgrade_version.sh
  $drush_cmd drush vset $upgrade_version upgraded
fi

if [ "$target_env" == 'local' ]; then
  $drush_cmd drush dis memcache memcache_admin -y
fi

target_env=$target_env drush_alias=$drush_alias ruby .ahoy/site/.scripts/deploy.sh
