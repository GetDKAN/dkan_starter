api = 2
core = 7.x

includes[core] = dkan/drupal-org-core.make

; Profile

projects[dkan][type] = profile
projects[dkan][download][type] = git
projects[dkan][download][url] = https://github.com/NuCivic/dkan.git
projects[dkan][download][tag] = 7.x-1.13-beta3
;projects[dkan][download][branch] = 7.x-1.x

; Un-comment if diff is not empty.
;projects[dkan][patch][1] = https://github.com/NuCivic/dkan/compare/7.x-1.12.13...release-1-12.diff

