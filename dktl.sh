#!/bin/bash

dktl_symlink_location=$(which dktl)
# echo $dktl_symlink_location

dktl_executable_location=$(readlink -f $dktl_symlink_location)
# echo $dktl_executable_location

blank=""
DKTL_DIRECTORY=$(echo "${dktl_executable_location///dktl.sh/$blank}")
# echo $DKTL_DIRECTORY
export DKTL_DIRECTORY

DKTL_CURRENT_DIRECTORY=$(pwd)
# echo $DKTL_CURRENT_DIRECTORY
export DKTL_CURRENT_DIRECTORY

SLUG=${PWD##*/}
SLUG=${SLUG//-/}
SLUG=${SLUG//_/}
SLUG=$(echo ${SLUG} | tr -d '[:space:]' | tr "[A-Z]" "[a-z]") # Mixed case dirs cause issue with docker image names
#echo $SLUG
export SLUG

which docker || (echo "you don't seem to have docker installed. Exiting."; exit 1)
which docker-compose || (echo "you don't seem to have docker-compose installed. Exiting."; exit 1)
export AHOY_CMD_PROXY=DOCKER

DOCKER_COMPOSE_COMMON_CONF="$DKTL_DIRECTORY/docker/docker-compose.common.yml"
PROXY_CONF="-f $DKTL_DIRECTORY/docker/docker-compose.noproxy.yml"
VOLUME_CONF="$DKTL_DIRECTORY/docker/docker-compose.nosync.yml"

#Check for proxy container, get domain from that.
PROXY_DOMAIN=`docker inspect proxy 2> /dev/null | grep docker.domain | tr -d ' ",-' | cut -d \= -f 2 | head -1`

#If no proxy is running, use the overridden or default proxy domain
if [ -z "$PROXY_DOMAIN" ]; then
  [ "$AHOY_WEB_DOMAIN" ] && WEB_DOMAIN=$AHOY_WEB_DOMAIN || WEB_DOMAIN="localtest.me"
  PROXY_DOMAIN=${WEB_DOMAIN}
fi

export PROXY_DOMAIN=$PROXY_DOMAIN


if [ $1 = "docker-compose" ]
then
    docker-compose -f $DOCKER_COMPOSE_COMMON_CONF -f $VOLUME_CONF $PROXY_CONF -p "${SLUG}" --project-directory $DKTL_CURRENT_DIRECTORY ${@:2}
else
    script_file=$1.php
    script_file_exists=$(docker-compose -f $DOCKER_COMPOSE_COMMON_CONF -f $VOLUME_CONF $PROXY_CONF -p "${SLUG}" --project-directory $DKTL_CURRENT_DIRECTORY exec cli ls /dktl | grep $script_file)
    echo $script_file_exists

    docker-compose -f $DOCKER_COMPOSE_COMMON_CONF -f $VOLUME_CONF $PROXY_CONF -p "${SLUG}" --project-directory $DKTL_CURRENT_DIRECTORY up -d
    if [ -n "$script_file_exists" ]
    then
        docker-compose -f $DOCKER_COMPOSE_COMMON_CONF -f $VOLUME_CONF $PROXY_CONF -p "${SLUG}" --project-directory $DKTL_CURRENT_DIRECTORY exec cli php /dktl/$script_file ${@:2}
    else
        docker-compose -f $DOCKER_COMPOSE_COMMON_CONF -f $VOLUME_CONF $PROXY_CONF -p "${SLUG}" --project-directory $DKTL_CURRENT_DIRECTORY exec cli $@
    fi
fi

