
echo "Target environment is $target_env"
echo "Drush alias is $drush_alias"

upgrade_version=upgrade_1_13
upgrade_status=`drush @$drush_alias vget $upgrade_version --exact`

if [ -z $upgrade_status ]; then
  upgrade_status='never upgraded';
fi

if [ "$upgrade_status" != 'upgraded' ]; then
  target_env=$target_env drush_alias=$drush_alias bash .ahoy/site/.scripts/upgrades/$upgrade_version.sh
  drush @$drush_alias vset $upgrade_version upgraded
fi

if [ "$target_env" == 'local' ]; then
  drush @$drush_alias dis memcache memcache_admin -y
fi

target_env=$target_env drush_alias=$drush_alias ruby .ahoy/site/.scripts/deploy.rb
