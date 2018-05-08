#!/bin/bash
#
# Cloud Hook: drush-env-switch
#

env=$1
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

target_env=$target_env bash .ahoy/site/.scripts/upgrade-deploy.sh
