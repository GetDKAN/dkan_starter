; This make file helps package DKAN as well as desired modules, themes, and
; libraries. Use in conjunction with drush subtree or build-manager to track
; and contribute to upstream code.

; Grab DKAN make file
includes[dkan] = projects/dkan/build-dkan.make
includes[data_story] = projects/data_story/data_story.make
includes[data_disqus] = projects/data_disqus/data_disqus.make
includes[data_workflow] = projects/data_workflow/data_workflow.make
includes[visualization_entity] = projects/visualization_entity/visualization_entity.make

; Include desired modules, themes, or libraries here.
projects[devel][version] = 1.x
projects[devel][subdir] = contrib
