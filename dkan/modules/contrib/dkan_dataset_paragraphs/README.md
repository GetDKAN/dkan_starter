# About this module

This module adds the "Extended Metadata" field to the dataset content type.

## Extending Available Metadata Bundles

Utilizing the [Paragraphs](https://www.drupal.org/project/paragraphs) module, this module's feature holds the **"field_metadata_extended"** field base and creates the field instance on the **Dataset** content type when the module is enabled. This allows admin UI overridding of the field settings without conflicting with dkan feature modules.

Additional "Paragraph" bundles can be added to provide additional metadata fields and the Dataset content type's "Extended Metadata" field settings can be edited to select which bundles are available when adding new Datasets. By default, all bundles are available.

The "[Paragraphs Default](http://drupal.org/project/paragraphs_defaults)" module allows you to configure which bundles of fields are automatically open when adding/editing Datasets at `/admin/structure/paragraphs/defaults/manage/node/field_metadata_extended/dataset`.

The configuration for new "Paragraph" bundles of **"Extended Metadata"** can be set in the Admin UI and saved in a new Feature module if you wish.

