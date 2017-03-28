api = 2
core = 7.x

includes[core] = dkan/drupal-org-core.make

; Profile

projects[dkan][type] = profile
projects[dkan][download][type] = git
projects[dkan][download][url] = https://github.com/NuCivic/dkan.git
projects[dkan][download][branch] = harvest-fields-dataset

;projects[dkan][patch][1703] = 'https://github.com/NuCivic/dkan/pull/1703.diff'
