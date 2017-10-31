---
core: 7.x
api: '2'
projects:
  eck:
    version: 2.0-rc9
    subdir: contrib
  geo_file_entity:
    subdir: contrib
    download:
      type: git
      url: https:///geo_file_entity.git
      revision: 
    type: module
  uuidreference:
    subdir: contrib
    version: 1.x-dev
    patch:
      238875: https://www.drupal.org/files/issues/uuidreference-alternative_to_module_invoke_all_implementation_for_query_alter_hook-238875-0.patch
libraries:
  chroma:
    download:
      type: file
      url: https://github.com/gka/chroma.js/zipball/11ef08f6922900f2e7aa04c3058808a39f1317ca
  numeral:
    download:
      type: file
      url: https://github.com/adamwdraper/Numeral-js/zipball/7de892ffb438af6e63b9c4f6aff0c9bc3932f09f
  recline_choropleth:
    download:
      type: file
      url: https:///recline.view.choroplethmap.js/archive/.zip
  leaflet_zoomtogeometries:
    download:
      type: file
      url: https:///leaflet.map.zoomToGeometries.js/zipball/
  nvd3:
    download:
      type: git
      url: https://github.com/novus/nvd3.git
      tag: v1.8.5
  d3:
    download:
      type: git
      url: https://github.com/d3/d3.git
      tag: v3.5.17
  gdocs:
    download:
      type: git
      url: https://github.com/okfn/recline.backend.gdocs.git
      revision: e81bb237759353932834a38a0ec810441e0ada10
  lodash_data:
    download:
      type: git
      url: https:///lodash.data.git
      revision: 
  spectrum:
    download:
      type: git
      url: https://github.com/bgrins/spectrum.git
      tag: 1.8.0
      revision: 9e04e5882de98cb9f909300b035d0f38c058c2fb
    destination: libraries
    directory_name: bgrins-spectrum
  reclineViewNvd3:
    download:
      type: git
      url: https:///recline.view.nvd3.js.git
      revision: 
