# Change log
## [1.13.4.0]
- Update to dkan version 1.13.4 
- Add check for resources to avoid errors when pruning #279
- Switch to yaml format for default makefiles #258
- Patched memcache to fix call to undefined object.

## [1.13.3.2]
- Support htaccess in public file directory  #232

## [1.13.3.1]
- Fix fast import tests when fast import is disabled in config.yml #233
- Fix notice about undefined constant CI. #233

## [1.13.3.0]
- Upgrade DKAN to version 1.13.3  PR#220

## [1.13.2.0]
- Upgrade DKAN to version 1.13.2  PR#136

## [1.13.1.5]
- Added patch on DKAN to include modifications made on PR#1795.

## [1.13.1.4]
- Update dkan acquia cache settings #118

## [1.13.1.3]
- Added patches on DKAN to include modifications made on PR#1766 and PR#1788.

## [1.13.1.2]
- Fix pass NULL in data_config_enabled_modules() #109
- Do not disable dkan_sitewide_demo_front via script #110
- Added 'no-main-menu' tag on tests #112
- Added patches on DKAN to include modifications made on PR#1769, PR#1761 and PR#1763

## [1.13.1.1]
- Update the 1.13 upgrade script #107

## [1.13.1.0]
- Remake with dkan 7.x-1.131 #106
- Add example.config.yml on update. #103
- Fix `ahoy drush custom-libs` #102
- Fix backup database reuse #98
- Fixed homepage conversion call on upgrade script #100

## [1.13.0.0]
- Remake with dkan 7.x-1.13 #96
- Make skip_tags configurable. #90
- Remove INSTALL and README files from sites. #94
- Upgrade dkan to 1.13.beta #57
- Enable dkan-workflow via config.yml #92
- Fix broken search index brakes DB pruning script #91
- Only fail the build on circle (not probo) #75
- Make git ignore asset files folder #80
- Add registry_rebuild to CircleCi environment #89
- Fix extra spaces in `ahoy dkan uli` output #86
- Fix circle.rb script - #74
- Add sql-drop step before database import #72

## [1.12.13.2]
- Fix broken template #69

## [1.12.13.1]
- Fix `ahoy build new` #63
- Update remote.ahoy.yml #64
- Fix http to https redirects #65
- Clean up db when vis_entity modules are disabled #66
- Add drupal extension patch for some custom tests. #67
- Make circle.yml configurable via config.yml #68
- Always allow cli access #56

## [1.12.13.0]
- Update dkan version to 7.x-1.12.13 https://github.com/NuCivic/dkan/releases/tag/7.x-1.12.13
- Recolection of customize dkan docs #47
- Adds support for private files if available in s3 backup #49
- Fix ahoy build update. #50
- Typo in ahoy utils files-link #51
- Add troubleshooting steps when container doesn't start #52
- Fix `ahoy build custom libs` not working. #53

## [1.12.12.0]
### Changed
- Disable shield module on local instance #23 
- Fix QA password not set #24 
- Refactor `ahoy build update` for re-use #27
- Relink `ahoy custom`  commands #28
- Fix data_config_modules_disabled() exception when module missing #32
- Fix `ahoy build config` compatibility bug with Linux #39
- Fix `ahoy build custom` not work for single modules #43
- Allow config for Google Analytic #44 
- Avoid indexing when dkan_acquia_search_solr is not installed. #46
- Update dkan version to 7.x-1.12.12 #45
  
## [1.12.10.3]
### Changed
- Remake dkan (#146)
- Fix data_config deployment issue (#146)
- Change CircleCI settings (#146)

## [1.12.10.3]
### Changed
- Add "lt" environment map #144
- Update nucivic-ahoy #143

## [1.12.10.2]
### Changed
- Disabled clamav by default. REF #140
### Added
- New config/config.yml param: `default: clamav`

## [1.12.10.1]
### Changed
- Set cron_safe_threshold to 0 so it only runs from Jenkins

## [1.12.10.0]
### Changed
- Remove new relic automatic settings #97
- Allow multiple tests via `ahoy dkan test` #126, nucivic/dkan#1351
- Remove .ahoy unnecessary deploy steps: nucivic/nucivic-ahoy#92
- Update dkan to version 1.12.10
  - Fix broken dkan update: nucivic/dkan#1283

## [1.12.9.0]
### Changed
- Updated dkan version to 1.12.9
- Added fast_file configuration setup via 'fast_file' config/config.yml
- Added 'https_everywhere' configuration
- Added stage_file_proxy configuration

## [1.12.8.0]
### Added
- Add new id for search servers

### Changed
- Remake dkan at 1.12.8 with release-1-12 patch
- Added securepages configuration

### Fixed
- Acquia solr search not using correct server.

## [1.12.7.0]
### Added
- config.yml (root level to store ata_starter specific metadata/and defaults)

### Changed
- Remake dkan at 1.12.7 with release-1-12 patch
- Versioning scheme (drop 7.x)

### Removed
- Unnecessary custom_config deps and info section

### Fixed
- `ahoy build custom` breaks if invalid custom.make

## [7.x-1.12.6.9]

### Fixed
- `ahoy build custom` breaks if invalid custom.make

## [7.x-1.12.6.8]

### Changed
- Skip more custom tests. 

## [7.x-1.12.6.7]

### Fixed
- Patches broken panalizer update. 

## [7.x-1.12.6.6]

### Fixed
- Updated DKAN to fix topics issue.

### Changed
- Updated settings.acquia.php to include correct solr server name.
- Updated ahoy tools.

### Removed
- assets/.htacess
- Symlink from docroot/.htacess to config/.htaccess

## [7.x-1.12.6.5]

### Added
- added default `config/.htaccess`

### Fixed
- Fixed more failing tests in DKAN.

### Changed
- Updated ahoy tools. `ahoy build new` does not throw ambiguous error when no
  argument is passed. 

### Deprecated

### Removed

### Security

## [7.x-1.12.6.4]

### Added

### Fixed
- Fixed upgraded hooks. 


### Changed
- Updated ahoy tools. `ahoy site config` now does not clobber custom .htaccess.

### Deprecated

### Removed

### Security

## [7.x-1.12.6.2]

### Added

### Fixed
- Patches `dkan 7.x-1.12.6`, skips custom tests.
- Patches `dkan 7.x-1.12.6`, fix tests break because configured not installed.


### Changed
- `ahoy site setup` now builds dynamic drush alias using `ahoy site name`

### Deprecated

### Removed

### Security

## [7.x-1.12.3.1] 2016-07-11
## [Unreleased]

### Added

### Fixed

### Changed
- `site rebuild` now uses `site update-data-starter`
- `site remake` only does a full remake if it runs within a Data Starter working copy
- `custom.make` no longer included from `build.make`
- `site custom` recipe to run drush make against `custom.make`
- `site new` now is only available if it runs within a Data Starter working copy
- `site dkan` only updates dkan if it runs within a Data Starter working copy
- `site update-data-starter` now places a working copy at `.data_starter_private` to speed up the update process (.gitignore is updated to ignore the folder)
- `site update-data-starter` prunes the working copy in `.data_starter_private` in advance to simplify the rsync copy

### Deprecated

### Removed

### Security

## [7.x-1.12.3.1] 2016-07-11
### Added
- `ahoy site config` takes configuration in `config/config.yml` and generates `config/config.php`
- Sample `config/config.yml` added:
```yaml
default:
  hostname: localhost
  https_everywhere: FALSE
acquia:
  subscription: 'professional'
  dev:
    base_url: http://devurl
    core_id: changeme
    derived_key: changeme
  test:
    base_url: http://testurl
    core_id: changeme
    derived_key: changeme
  ra:
    base_url: http://testurl
    core_id: changeme
    derived_key: changeme
  prod:
    base_url: http://produrl
    core_id: changeme
    derived_key: changeme
```
- A symlink has been added from `hooks/common/post-code-deploy/deploy.sh -> drush-env-switch.sh` for backwards compatibility.

### Changed
- `drush/aliases.local.php` now defines an alias for data_starter that's used on cirle ci
- `ahoy site setup` creates a symlink to `/var/www/docroot` if it's run on circle
- `config/config.php` is now part of the data_starter codebase
- `devinci_custom_environment_switch` function has been moved from `settins.php` to `settings.nucivic.php`
- `drush-env-switch.sh` deployment hook script now uses passed in `site` and `env` variables to run deployment.
- `ahoy custom setup/deploy` now call `ahoy ci setup/deploy` oob.
- `ahoy ci deploy` now calls the acquia hook deployment script and the deployment script runs a environment force switch
- `ahoy site update-data-starter` has been modified so that it excludes any changes to `projects/modules/custom` or to `assets/sites/default/settings.php`
- `ahoy site config` will trigger not just the config.php generation but potentially any site specific changes that can be applied via the templates `config/template` do to `config.yml` 

## [7.x-1.12.3.0] 2016-07-05
### Changed
- Updates dkan to 7.x-1.12.3.0

## [7.x-1.12.2.4] 2016-06-18
### Changed
- `ahoy custom setup` and `ahoy custom deploy` now trigger ci commands
### Fixed
- `ahoy site setup` to fix `find: unknown predicate -exec=cp'`
- Only makes acquia solr available in solr environments

## [7.x-1.12.2.3] 2016-06-17
### Changed
- Updates ahoy tools

## [7.x-1.12.2.2] 2016-06-16
### Added
- Adds dkan health status module to codebase
### Fixed
- Features master array related errors
### Changed
- Better Solr handled logic

## [7.x-1.12.2.1] 2016-06-10
### Added
- Better memcache integration handling in `assets/sites/default/settings.php`

## [7.x-1.12.2.0] 2016-06-10
### Changed
- Updates dkan to 7.x-1.12.2
