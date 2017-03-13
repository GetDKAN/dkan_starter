api = 2
core = 7.x

includes[core] = dkan/drupal-org-core.make

; Profile

projects[dkan][type] = profile
projects[dkan][download][type] = git
projects[dkan][download][url] = https://github.com/NuCivic/dkan.git
projects[dkan][download][branch] = CIVIC-5746-topics-test

;projects[dkan][patch][1] = https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/1769.diff
;projects[dkan][patch][2] = https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/1761.diff
;projects[dkan][patch][3] = https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/1763.diff
;projects[dkan][patch][4] = https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/1766.diff
;projects[dkan][patch][5] = https://patch-diff.githubusercontent.com/raw/NuCivic/dkan/pull/1788.diff
