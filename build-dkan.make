api: '2'
core: 7.x
includes:
  - dkan/drupal-org-core.make
projects:
  dkan:
    type: profile
    download:
      type: git
      url: https://github.com/GetDKAN/dkan.git
      tag: 7.x-1.14-RC1
    patch:
      - https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/2121.diff
