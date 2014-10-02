; This make file helps package DKAN as well as desired modules, themes, and
; libraries. Use in conjunction with drush subtree or build-manager to track
; and contribute to upstream code.

; Grab DKAN make file
includes[dkan] = projects/dkan/build-dkan.make
includes[data_story] = projects/data_story/data_story.make
includes[data_disqus] = projects/data_disqus/data_disqus.make
includes[data_workflow] = projects/data_workflow/data_workflow.make


; Viz entitiy dependencies and libraries (less recursion this way)
projects[eck][version] = 2.0-rc3
projects[eck][subdir] = contrib

libraries[chroma][download][type] = "file"
libraries[chroma][download][url] = "https://github.com/gka/chroma.js/zipball/master"

libraries[numeral][download][type] = "file"
libraries[numeral][download][url] = "https://github.com/adamwdraper/Numeral-js/zipball/master"

libraries[recline_choropleth][download][type] = "file"
libraries[recline_choropleth][download][url] = "https://github.com/NuCivic/choropleth/zipball/integration_branch"

libraries[recline_choropleth_dataset][download][type] = "file"
libraries[recline_choropleth_dataset][download][url] = "https://github.com/NuCivic/choropleth_dataset/zipball/fix_accessing_state_variable_globally"

; Include desired modules, themes, or libraries here.
projects[devel][version] = 1.x
projects[devel][subdir] = contrib

