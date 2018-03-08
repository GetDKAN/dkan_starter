api: '2'
core: 7.x
includes:
  # Site-specific custom modules
  - config/custom.make
  # Grab DKAN make file
  - build-dkan.make
defaults:
  projects:
    subdir: contrib
projects:
  # Reset download dir for DKAN
  dkan:
    subdir: ''
    patch: https://patch-diff.githubusercontent.com/raw/GetDKAN/dkan/pull/2419.patch
  devinci:
    version: ''
  environment:
    version: ''
  environment_indicator:
    version: ''
  features_banish:
    version: ''
  features_master:
    patch:
      2775681: 'https://www.drupal.org/files/issues/2775681-4-only-revert-if-defined.patch'
  stage_file_proxy:
    version: '1.7'
  visualization_entity_maps:
    type: module
    download:
      type: git
      url: 'https://github.com/NuCivic/visualization_entity_maps.git'
      branch: master
  visualization_entity_tables:
    type: module
    download:
      type: git
      url: 'https://github.com/NuCivic/visualization_entity_tables.git'
      branch: master
  memcache:
    patch:
      2856140: 'https://www.drupal.org/files/issues/undefined-function-dmemcache_object_cluster-2856140-8.patch'
  ape:
    version: '1.1'
  clamav:
    version: 1.x-dev
  seckit:
    version: '1.9'
  dkan_acquia_expire:
    type: module
    download:
      type: git
      url: 'https://github.com/NuCivic/dkan_acquia_expire.git'
      branch: master
  dkan_acquia_search_solr:
    type: module
    download:
      type: git
      url: 'https://github.com/NuCivic/dkan_acquia_search_solr.git'
      branch: master
  dkan_health_status:
    type: module
    download:
      type: git
      url: 'https://github.com/NuCivic/dkan_health_status.git'
      branch: 7.x-1.x
  devel:
    version: ''
  maillog:
    version: ''
  shield:
    version: ''
  acquia_connector:
    version: ''
  acquia_purge:
    version: ''
  search_api_acquia:
    version: ''
  acquia_search_multi_subs:
    version: ''
  search_api_solr:
    version: ''
  expire:
    version: ''
  expire_panels:
    version: ''
  entitycache:
    version: ''
  admin_views:
    version: ''
  fast_404:
    version: ''
  securepages:
    version: ''
  role_watchdog:
    version: ''
  google_analytics:
    version: ''
  google_tag:
    version: ''
  # Additional supported, optional modules: features_override, security_review, password_policy
