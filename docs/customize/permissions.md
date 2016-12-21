# Override core permissions

Don't.

# Custom permissions and roles

Don't

# Extend core permissions

* Capture the additional set of needed permissions in a complementary user role name after the core user role (If you intend to extend editor, then name it after editor)
* Tie them up as **DKAN WORKFLOW** does it, using a `hook_form_alter` implementation. See [this link](https://github.com/NuCivic/dkan/blob/7.x-1.x/modules/dkan/dkan_workflow/modules/dkan_workflow_permissions/dkan_workflow_permissions.module#L36).
