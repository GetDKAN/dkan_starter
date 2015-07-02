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
;projects[] = acquia_connector
; Acquia Expire integration
;projects[] = acquia_purge
; Acquia Search modules
;projects[] = search_api_acquia
;projects[] = acquia_search_multi_subs

; Search
; ======
;projects[] = search_api_solr

; DATA WORKFLOW
; =====
projects[data_workflow][type] = module
projects[data_workflow][download][type] = git
projects[data_workflow][download][url] = git@github.com:NuCivic/data_workflow.git
projects[data_workflow][download][branch] = master



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
