#!/bin/bash
#
# Cloud Hook: drush-env-switch
#

env=$1
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

echo "The environment is $env"
for m in "${env_map[@]}"; do
  target_key=$(echo $m | cut -d: -f1)
  if [ "$target_key" = "$env" ]; then
    echo $m
    target_env=$(echo $m | cut -d: -f2)
   break
  fi
done

acquia=`env | grep AH_REALM`

if [ "$acquia" != '' ]; then
  echo "Acquia environment detected. Moving to correct directory. /var/www/html/$1'.'$2"
  cd /var/www/html/$1'.'$2
  pwd 
fi

echo "target_env is $target_env"
target_env=$target_env bash .ahoy/site/.scripts/upgrade-deploy.sh
