#!/usr/bin/env bash

set -e

if [ ! -f ~/.s3curl ]; then
  echo "
%awsSecretAccessKeys = (
  local => {
    id => '$AWS_ID',
    key => '$AWS_KEY',
  }
);" > ~/.s3curl

chmod 600 ~/.s3curl
fi


