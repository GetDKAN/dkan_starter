; This make file helps package DKAN as well as desired modules, themes, and
; libraries. Use in conjunction with drush subtree or build-manager to track
; and contribute to upstream code.

; Grab DKAN make file
includes[dkan] = build-dkan.make

; Set the default subdirectory for projects so we don't have to specifically set it each time, but make sure dkan stays in /profiles
defaults[projects][subdir] = contrib
projects[dkan][subdir] = ""

; Default Projects
; ================
projects[] = features_master
projects[] = features_banish

; Optional Projects
; =================
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
projects[] = acquia_connector
; Acquia Expire integration
projects[] = acquia_purge
; Acquia Search modules
projects[] = search_api_acquia
projects[] = acquia_search_multi_subs

; Search
; ======
projects[] = search_api_solr

; DKAN Migrate Base
; =====
projects[dkan_migrate_base][type] = module
projects[dkan_migrate_base][download][type] = git
projects[dkan_migrate_base][download][url] = git@github.com:NuCivic/dkan_migrate_base.git
projects[dkan_migrate_base][download][branch] = master

; DKAN Harvest
; =====
projects[dkan_harvest][type] = module
projects[dkan_harvest][download][type] = git
projects[dkan_harvest][download][url] = git@github.com:NuCivic/dkan_harvest.git
projects[dkan_harvest][download][branch] = master

; Additional Visualization Entity Components
; =====
projects[visualization_entity_maps][type] = module
projects[visualization_entity_maps][download][type] = git
projects[visualization_entity_maps][download][url] = git@github.com:NuCivic/visualization_entity_maps.git
projects[visualization_entity_maps][download][branch] = master

projects[visualization_entity_tables][type] = module
projects[visualization_entity_tables][download][type] = git
projects[visualization_entity_tables][download][url] = git@github.com:NuCivic/visualization_entity_tables.git
projects[visualization_entity_tables][download][branch] = master

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
