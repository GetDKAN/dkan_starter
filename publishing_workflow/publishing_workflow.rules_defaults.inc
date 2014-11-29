<?php
/**
 * @file
 * publishing_workflow.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function publishing_workflow_default_rules_configuration() {
  $items = array();
  $items['rules_author_saves_dataset_or_resource'] = entity_import('rules_config', '{ "rules_author_saves_dataset_or_resource" : {
      "LABEL" : "NUCIVIC DATA -Author saves Dataset or Resource",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "author", "dataset", "resource" ],
      "REQUIRES" : [ "rules", "publishing_workflow" ],
      "ON" : [ "node_insert", "node_update" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "dataset" : "dataset", "resource" : "resource" } }
          }
        },
        { "NOT node_is_published" : { "node" : [ "node" ] } },
        { "user_has_role" : {
            "account" : [ "node:author" ],
            "roles" : { "value" : { "226931607" : "226931607" } }
          }
        }
      ],
      "DO" : [
        { "publishing_workflow_reviewers" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "reviewers" : { "reviewers" : "List of reviewers" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "reviewers" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "mail" : {
                  "to" : [ "list-item:mail" ],
                  "subject" : "[node:author] has just updated \\u0022[node:title]\\u0022",
                  "message" : "Please review the recent update at -\\u003E [node:edit-url]",
                  "from" : "[node:author:mail]",
                  "language" : [ "" ]
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_editor_publish_dataset_or_resource'] = entity_import('rules_config', '{ "rules_editor_publish_dataset_or_resource" : {
      "LABEL" : "NUCIVIC DATA -Editor Publish Dataset or Resource",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "dataset", "editor", "publish", "resource" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_update" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "dataset" : "dataset", "resource" : "resource" } }
          }
        },
        { "NOT user_has_role" : {
            "account" : [ "node:author" ],
            "roles" : { "value" : { "254633039" : "254633039", "27274083" : "27274083" } }
          }
        },
        { "node_is_published" : { "node" : [ "node" ] } }
      ],
      "DO" : [
        { "mail" : {
            "to" : "[node:author:mail]",
            "subject" : "The \\u0022[node:title]\\u0022 [node:content-type] was just published",
            "message" : "You can review it here -\\u003E [node:edit-url]",
            "from" : "[site:mail]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
