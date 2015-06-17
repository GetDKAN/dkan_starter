; This make file helps package DKAN as well as desired modules, themes, and
; libraries. Use in conjunction with drush subtree or build-manager to track
; and contribute to upstream code.

; Grab DKAN make file
includes[dkan] = build-dkan.make

; Set the default subdirectory for projects so we don't have to specifically set it each time, but make sure dkan stays in /profiles
defaults[projects][subdir] = contrib
projects[dkan][subdir] = ""

; These start as all commented out, but you'll probably want to enable almost all of them depending on the project.

; Development
; ===========
;projects[] = devel
;projects[] = environment
;projects[] = maillog
;projects[] = shield
;projects[] = features_override
;projects[] = security_review

; Acquia
; ======
;projects[] = acquia_connector
; Acquia Expire integration
;projects[] = acquia_purge
; Acquia Search modules
;projects[] = search_api_acquia
;projects[] = acquia_search_multi_subs

; Search
; ======
;projects[] = search_api_solr

; DKAN
; =====
; override nuboot_radix, temporary till merged
;projects[nuboot_radix][download][type] = git
;projects[nuboot_radix][download][url] = https://github.com/NuCivic/nuboot_radix.git
;projects[nuboot_radix][download][branch] = 7.x-1.x
;projects[nuboot_radix][download][revision] = "137ae6ac98298b16c1f746626bd0821dbae7e6f4"
;projects[nuboot_radix][type] = theme

; override open_data_schema_map, temporary till merged.
; @todo also remove rm command from buildmanager
;projects[open_data_schema_map][type] = module
;projects[open_data_schema_map][download][type] = git
;projects[open_data_schema_map][download][url] = https://github.com/NuCivic/open_data_schema_map.git
;projects[open_data_schema_map][download][branch] = draft_for_file_caching


;projects[dkan_migrate_base][type] = module
;projects[dkan_migrate_base][download][type] = git
;projects[dkan_migrate_base][download][url] = https://github.com/NuCivic/dkan_migrate_base.git
;projects[dkan_migrate_base][download][branch] = single_migration_class

; Performance
; ===========
;projects[] = expire
;projects[] = memcache
;projects[] = entitycache
;projects[] = admin_views

; Security
; ==========
;projects[] = securepages

; Other
; ======
;projects[] = google_analytics
;projects[] = migrate
;projects[] = entity
