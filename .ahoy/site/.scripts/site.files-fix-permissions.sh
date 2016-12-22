#!/usr/bin/env bash

mkdir -p docroot/sites/default/files
chmod 777 assets/sites/default

if [ -d docroot/sites/default/files ]; then
  find docroot/sites/default/files/ -type d -exec chmod o+rwx {} \;
  find docroot/sites/default/files/ -type f -exec chmod o+rw {} \;
fi

mkdir -p docroot/sites/default/private

if [ -d docroot/sites/default/private]; then
  find docroot/sites/default/private/ -type d -exec chmod o+rwx {} \;
  find docroot/sites/default/private/ -type f -exec chmod o+rw {} \;
fi