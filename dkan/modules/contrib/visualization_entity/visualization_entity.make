core: 7.x
api: '2'
projects:
  eck:
    version: 2.0-rc8
    subdir: contrib
  geo_file_entity:
    subdir: contrib
    download:
      type: git
      url: 'https://github.com/NuCivic/geo_file_entity.git'
      branch: master
    type: module
  uuidreference:
    subdir: contrib
    version: 1.x-dev
    patch:
      238875: 'https://www.drupal.org/files/issues/uuidreference-alternative_to_module_invoke_all_implementation_for_query_alter_hook-238875-0.patch'
libraries:
  chroma:
    download:
      type: file
      url: 'https://github.com/gka/chroma.js/zipball/master'
  numeral:
    download:
      type: file
      url: 'https://github.com/adamwdraper/Numeral-js/zipball/master'
  recline_choropleth:
    download:
      type: file
      url: 'https://github.com/NuCivic/recline.view.choroplethmap.js/archive/master.zip'
  leaflet_zoomtogeometries:
    download:
      type: file
      url: 'https://github.com/NuCivic/leaflet.map.zoomToGeometries.js/zipball/master'
  nvd3:
    download:
      type: git
      url: 'https://github.com/novus/nvd3.git'
      tag: v1.8.5
  d3:
    download:
      type: git
      url: 'https://github.com/d3/d3.git'
      tag: v3.5.17
  gdocs:
    download:
      type: git
      url: 'https://github.com/okfn/recline.backend.gdocs.git'
      revision: e81bb237759353932834a38a0ec810441e0ada10
  lodash_data:
    download:
      type: git
      url: 'https://github.com/NuCivic/lodash.data.git'
      branch: master
  spectrum:
    download:
      type: git
      url: 'https://github.com/bgrins/spectrum.git'
      tag: 1.8.0
    destination: libraries
    directory_name: bgrins-spectrum
  reclineViewNvd3:
    download:
      type: git
      url: 'https://github.com/NuCivic/recline.view.nvd3.js.git'
      branch: CIVIC-6000
