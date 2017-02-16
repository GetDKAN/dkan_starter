#!/bin/bash

# This is a custom deployment script. It will add deployment steps that are not
# covered by hooks/common/post-code-update/drush-env-switch.sh
# 
# If STOP_EXECUTION is set to true then the deployment script will exit and
# this script will be the only one to run.
#
# 
STOP_EXECUTION=false

echo "Running custom deployment steps"
