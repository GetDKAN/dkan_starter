; This make file helps package DKAN as well as desired modules, themes, and
; libraries. Use in conjunction with drush subtree or build-manager to track
; and contribute to upstream code.

; Set the default subdirectory for projects so we don't have to specifically set it each time, but make sure dkan stays in /profiles
defaults[projects][subdir] = contrib
projects[dkan][subdir] = ""

; Site specific custom modules
includes[] = config/custom.make

; Grab DKAN make file
includes[] = build-dkan.make

; Default Projects
; ================
projects[] = devinci
projects[] = environment
projects[] = environment_indicator
projects[] = features_banish
projects[features_master] = 1.1
projects[features_master][patch][2775681] = https://www.drupal.org/files/issues/2775681-4-only-revert-if-defined.patch

; Optional Projects
; =================
; These start as all commented out, but you'll probably want to enable almost all of them depending on the project.

; Development
; ===========
projects[] = devel
projects[] = environment
projects[] = environment_indicator
projects[] = maillog
projects[] = shield
;projects[] = features_override
;projects[] = security_review
projects[stage_file_proxy][version] = 1.7
projects[stage_file_proxy][subdir] = contrib

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

; DKAN Migrate
; ============
projects[] = migrate
; DKAN Migrate Base
projects[dkan_migrate_base][type] = module
projects[dkan_migrate_base][download][type] = git
projects[dkan_migrate_base][download][url] = https://github.com/NuCivic/dkan_migrate_base.git
projects[dkan_migrate_base][download][branch] = 7.x-1.x

; DKAN Harvest
projects[dkan_harvest][type] = module
projects[dkan_harvest][download][type] = git
projects[dkan_harvest][download][url] = https://github.com/NuCivic/dkan_harvest.git
projects[dkan_harvest][download][branch] = 7.x-1.x

; Additional Visualization Entity Components
; ==========================================
projects[visualization_entity_maps][type] = module
projects[visualization_entity_maps][download][type] = git
projects[visualization_entity_maps][download][url] = https://github.com/NuCivic/visualization_entity_maps.git
projects[visualization_entity_maps][download][branch] = master

projects[visualization_entity_tables][type] = module
projects[visualization_entity_tables][download][type] = git
projects[visualization_entity_tables][download][url] = https://github.com/NuCivic/visualization_entity_tables.git
projects[visualization_entity_tables][download][branch] = master

; Federal
; ==========================================
;projects[] = password_policy
;projects[] = open_data_federal_extras
;projects[open_data_federal_extras][type] = module
;projects[open_data_federal_extras][download][type] = git
;projects[open_data_federal_extras][download][url] = https://github.com/NuCivic/open_data_federal_extras.git
;projects[open_data_federal_extras][download][branch] = master

; Performance
; ===========
projects[] = expire
projects[] = memcache
projects[] = entitycache
projects[] = admin_views
projects[] = fast_404

projects[ape][version] = 1.1
projects[ape][subdir] = contrib

; Security
; ==========
projects[] = securepages
projects[clamav][version] = 1.x-dev
projects[clamav][subdir] = contrib

; Other
; ======
projects[] = google_analytics
projects[] = google_tag

; NuCivic
; =======
projects[dkan_acquia_expire][type] = module
projects[dkan_acquia_expire][download][type] = git
projects[dkan_acquia_expire][download][url] = https://github.com/NuCivic/dkan_acquia_expire.git
projects[dkan_acquia_expire][download][branch] = master

projects[dkan_acquia_search_solr][type] = module
projects[dkan_acquia_search_solr][download][type] = git
projects[dkan_acquia_search_solr][download][url] = https://github.com/NuCivic/dkan_acquia_search_solr.git
projects[dkan_acquia_search_solr][download][branch] = master

projects[dkan_health_status][type] = module
projects[dkan_health_status][download][type] = git
projects[dkan_health_status][download][url] = https://github.com/NuCivic/dkan_health_status.git
projects[dkan_health_status][download][branch] = 7.x-1.x

;projects[nucivic_data_devops][type] = module
;projects[nucivic_data_devops][download][type] = git
;projects[nucivic_data_devops][download][url] = https://github.com/NuCivic/nucivic_data_devops.git
;projects[nucivic_data_devops][download][branch] = master
