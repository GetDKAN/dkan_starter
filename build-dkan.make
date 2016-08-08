api = 2
core = 7.x

includes[core] = drupal-org-core.make

; Profile

projects[dkan][type] = profile
projects[dkan][download][type] = git
projects[dkan][download][url] = https://github.com/NuCivic/dkan.git
projects[dkan][download][branch] = 7.x-1.12.6 
projects[dkan][patch][1] = https://github.com/NuCivic/dkan/compare/7.x-1.12.6...release-1-12.diff 
projects[dkan][patch][2] = https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/1283.diff

; Restore topics page fix
projects[dkan][patch][3] = https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/1305.diff

; Should be removed before 7.x-1.12.7 build
projects[dkan][patch][4] = config/patches/remove-remote-tests.diff
