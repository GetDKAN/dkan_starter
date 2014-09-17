<?php
/**
 * @file
 * og_permissions_settings.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function og_permissions_settings_default_rules_configuration() {
  $items = array();
  $items['rules_content_editor_joins_group'] = entity_import('rules_config', '{ "rules_content_editor_joins_group" : {
      "LABEL" : "NUCIVIC DATA - Content Editor joins group",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "editor", "group", "joins" ],
      "REQUIRES" : [ "rules", "og" ],
      "ON" : [ "og_user_insert" ],
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "254633039" : "254633039" } }
          }
        }
      ],
      "DO" : [
        { "og_grant_og_role" : {
            "account" : [ "account" ],
            "group" : [ "site:og-context--node" ],
            "roles" : { "value" : { "content editor" : "content editor" } }
          }
        }
      ]
    }
  }');
  $items['rules_portal_administrator_gets_group_organizer_role'] = entity_import('rules_config', '{ "rules_portal_administrator_gets_group_organizer_role" : {
      "LABEL" : "NUCIVIC DATA - Portal Administrator gets the Group Organizer Role",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_insert", "user_update" ],
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "27274083" : "27274083" } }
          }
        }
      ],
      "DO" : [
        { "user_add_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "161863194" : "161863194" } }
          }
        }
      ]
    }
  }');
  return $items;
}
