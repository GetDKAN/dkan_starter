core = 7.x
api = 2

projects[geo_file_entity][subdir] = nucivic
projects[geo_file_entity][download][type] = git
projects[geo_file_entity][download][url] = https://github.com/NuCivic/geo_file_entity.git
projects[geo_file_entity][download][branch] = master

includes[geo_file_entity_make] = https://raw.githubusercontent.com/NuCivic/geo_file_entity/master/geo_file_entity.make 

; Libraries
libraries[chroma][download][type] = "file"
libraries[chroma][download][url] = "https://github.com/gka/chroma.js/zipball/master"

libraries[numeral][download][type] = "file"
libraries[numeral][download][url] = "https://github.com/adamwdraper/Numeral-js/zipball/master"

libraries[recline_choropleth][download][type] = "file"
libraries[recline_choropleth][download][url] = "https://github.com/NuCivic/choropleth/zipball/integration_branch"

libraries[recline_choropleth_dataset][download][type] = "file"
libraries[recline_choropleth_dataset][download][url] = "https://github.com/NuCivic/choropleth_dataset/zipball/fix_accessing_state_variable_globally"