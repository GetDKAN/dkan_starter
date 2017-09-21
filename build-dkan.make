api = 2
core = 7.x

includes[core] = dkan/drupal-org-core.make

; Profile

projects[dkan][type] = profile
projects[dkan][download][type] = git
projects[dkan][download][url] = https://github.com/NuCivic/dkan.git
projects[dkan][download][tag] = 7.x-1.14-RC1

projects[dkan][patch][] = https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/2121.diff

