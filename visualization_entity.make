core = 7.x
api = 2

projects[geo_file_entity][subdir] = nucivic
projects[geo_file_entity][download][type] = git
projects[geo_file_entity][download][url] = https://github.com/NuCivic/geo_file_entity.git
projects[geo_file_entity][download][branch] = master
projects[geo_file_entity][type] = module

; Libraries
libraries[chroma][download][type] = "file"
libraries[chroma][download][url] = "https://github.com/gka/chroma.js/zipball/master"

libraries[numeral][download][type] = "file"
libraries[numeral][download][url] = "https://github.com/adamwdraper/Numeral-js/zipball/master"

libraries[recline_choropleth][download][type] = "file"
libraries[recline_choropleth][download][url] = "https://github.com/NuCivic/recline.view.choroplethmap.js/archive/master.zip"
