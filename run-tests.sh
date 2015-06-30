#!/usr/bin/env bash
set -e

if [ -z "$1" ];
then
  echo "Usage: run-tests.sh http://127.0.0.1:8888 [--name='Name of scenario'] [--additional-behat-arg=something]"
  exit 1
else
  URL=$1
fi

echo "RUN_TESTS =============>"

# Store the working directory so we can switch back.
PWD=`pwd`
echo "  > $PWD"
cd tests
echo  "  > changed directory to "`pwd`

# USE ENV params so that we can set the base URL via command line, which bin/beheat doesn't support in a nice way.
ENV_PARAMS='BEHAT_PARAMS='\''{"extensions":{"Behat\\MinkExtension":{"base_url":"'$URL'"}}}'\'
# --expand doesn't work / was removed in behat 3.x See https://github.com/Behat/Behat/issues/723
FORMAT_SETTINGS='--format-settings='\''{"expand": true}'\'
CMD="bin/behat --colors features/ -v"

RUN="$ENV_PARAMS $CMD $FORMAT_SETTINGS ${@:2}"
echo "  > Running Command: $RUN\n"

set -v
eval $RUN
set +v
cd $PWD
