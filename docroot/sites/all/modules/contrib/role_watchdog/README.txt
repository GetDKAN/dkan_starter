
Description
---------------
Role watchdog automatically logs all role changes made through the user profile
or the User List in its own table. A record of these changes is shown in a Role
history tab on each user's page. Role watchdog can optionally monitor one or 
more specific roles for changes and notify members of selected roles via email 
whenever a change occurs.

This module might be useful when there are multiple administrators for a site, 
and you need auditing or alerting of manual role changes.


Dependencies
---------------
None. Database logging is a Core (optional) module.

Tested compatible with:
* user edit (e.g. http://example.com/user/3/edit)
* user list (e.g. http://example.com/admin/user/user)
* Views Bulk Operations (VBO) 7.x-3.x-dev
* Role Delegation 7.x-1.0

Related Modules
---------------
Role Delegation (http://drupal.org/project/role_delegation), 
RoleAssign (http://drupal.org/project/roleassign),
Administer Users by Role (http://drupal.org/project/administerusersbyrole)
  modules that enable user access to assign roles to other users where the
  auditing of Role watchdog is a nice fit.

Role Change Notify (http://drupal.org/project/role_change_notify)
  the mirror functionality of Role watchdog, notifying the user when a role is
  added to their account.


Usage
---------------
Role watchdog will automatically start recording all role changes. No further 
configuration is necessary for this functionality, the module will do this "out
of the box". A record of these changes is shown in a Role history tab on each
user's page and optionally in the Watchdog log if enabled. Users will need 
either "View role history" or "View own role history" access permissions to 
view the tab. 

Role watchdog can optionally email members of selected Notify roles when 
selected Monitor roles are added or removed. This was specifically added to 
keep a closer eye on certain role changes, such as an Administrator role. At 
least one Monitor role and one Notify role must be selected for this 
functionality.


Author
---------------
John Money
ossemble LLC.
http://ossemble.com

Module development originally sponsored by LifeWire, a service of About.com, a 
part of The New York Times Company.
http://www.lifewire.com
