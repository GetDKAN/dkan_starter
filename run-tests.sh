#!/usr/bin/env bash

# Store the working directory so we can switch back.
PWD=`pwd`
echo $PWD
cd tests

if [ -z "$1" ];
then
  echo "Usage: run-tests.sh http://127.0.0.1:8888 [--name='Name of scenario'] [--additional-behat-arg=something]"
  exit 1
else
  URL=$1
fi

# USE
ENV_PARAMS='BEHAT_PARAMS='\''{"extensions":{"Behat\\MinkExtension":{"base_url":"'$URL'"}}}'\'
# --expand doesn't work / was removed in behat 3.x See https://github.com/Behat/Behat/issues/723
FORMAT_SETTINGS='--format-settings='\''{"expand": true}'\'
CMD="bin/behat --colors features/ -v"
echo "Running $ENV_PARAMS $CMD $FORMAT_SETTINGS ${@:2}"

eval $ENV_PARAMS $CMD

cd $PWD
