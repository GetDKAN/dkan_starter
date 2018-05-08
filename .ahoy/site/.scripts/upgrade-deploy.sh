echo "running upgrade deploy script: .ahoy/site/.scripts/upgrade-deploy.sh"
drush_cmd="drush"
if [ "$target_env" == "local" ]; then
  drush_cmd="ahoy drush"
fi
echo "Target environment is $target_env"

upgrade_version="upgrade_1_14"
upgrade_status=`${drush_cmd} vget $upgrade_version --exact |tr -d '\n'`

echo "The upgrade status is $upgrade_status"

if [ -z "$upgrade_status" ]; then
  upgrade_status='never upgraded';
fi


if [ "$upgrade_status" != 'upgraded' ]; then
  echo "The site was not upgraded. Running $upgrade_version.sh"
  drush_cmd=$drush_cmd bash .ahoy/site/.scripts/upgrades/$upgrade_version.sh
  ${drush_cmd} vset $upgrade_version upgraded
fi

if [ "$target_env" == 'local' ]; then
  $drush_cmd dis memcache memcache_admin -y
fi

target_env=$target_env bash .ahoy/site/.scripts/deploy.sh
