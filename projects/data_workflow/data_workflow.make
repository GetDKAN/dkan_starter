core = 7.x
api = 2

; Adds the ability to add og roles to user with rules
projects[og][version] = 2.7
projects[og][subdir] = contrib

projects[entityreference_unpublished_node][download][type] = git
projects[entityreference_unpublished_node][download][url] = "http://git.drupal.org/sandbox/Ayrmax/1977458.git"
projects[entityreference_unpublished_node][subdir] = contrib
projects[entityreference_unpublished_node][type] = module

projects[features_roles_permissions][version] = 1.0
projects[features_roles_permissions][subdir] = contrib

projects[roleassign][version] = 1.0
projects[roleassign][subdir] = contrib

projects[role_delegation][version] = 1.1
; Forbid editing of accounts with higher permission: https://drupal.org/node/1156414
projects[role_delegation][patch][1156414] = https://drupal.org/files/issues/1156414-prevent-editing-of-certain-users-16.patch
projects[role_delegation][subdir] = contrib

projects[role_export][version] = 1.0
projects[role_export][subdir] = contrib

projects[view_unpublished][version] = 1.x-dev
projects[view_unpublished][subdir] = contrib