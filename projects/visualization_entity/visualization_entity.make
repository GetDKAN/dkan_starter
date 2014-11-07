core = 7.x
api = 2

projects[geo_file_entity][subdir] = nucivic
projects[geo_file_entity][download][type] = git
projects[geo_file_entity][download][url] = https://github.com/NuCivic/geo_file_entity.git
projects[geo_file_entity][download][branch] = master
projects[geo_file_entity][type] = module

;includes[geo_file_entity_make] = https://raw.githubusercontent.com/NuCivic/geo_file_entity/master/geo_file_entity.make 

;projects[dkan_dataset][subdir] = dkan
;projects[dkan_dataset][download][type] = git
;projects[dkan_dataset][download][url] = https://github.com/NuCivic/dkan_dataset.git
;projects[dkan_dataset][download][branch] = 7.x-1.x

;includes[dkan_dataset_make] = https://raw.githubusercontent.com/NuCivic/dkan_dataset/7.x-1.x/dkan_dataset.make

; Libraries
libraries[chroma][download][type] = "file"
libraries[chroma][download][url] = "https://github.com/gka/chroma.js/zipball/master"

libraries[numeral][download][type] = "file"
libraries[numeral][download][url] = "https://github.com/adamwdraper/Numeral-js/zipball/master"

libraries[recline_choropleth][download][type] = "file"
libraries[recline_choropleth][download][url] = "https://github.com/NuCivic/recline.view.choroplethmap.js/archive/master.zip"
