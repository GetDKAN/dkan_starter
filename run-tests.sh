#!/usr/bin/env bash

PWD=`pwd`
echo $PWD
cd tests
if [ -z "$1" ];
then
  PROFILE="default"
else
  PROFILE=$1
fi

CMD="bin/behat --colors --profile=$PROFILE features/ ${@:2}"
echo "Running $CMD"


$CMD

cd $PWD
