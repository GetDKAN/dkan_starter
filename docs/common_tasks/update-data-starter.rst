Update DKAN Starter
-------------------

When a new version of DKAN is released, the core dkan_starter repo needs to be updated. The following instructions are primarily for the core DKAN team that maintains the [dkan_starter repo](https://github.com/GetDKAN/dkan_starter), but if you maintain a fork of dkan_starter as an "upstream" to build your own organization's sites from, you may need to do this as well. 

1. `git checkout master`
2. `git checkout -b [branch-name]`
3. If upgrading DKAN, update line 11 in **build-dkan.make** with the latest tag from `DKAN <https://github.com/GetDKAN/dkan/releases>`_
4. `ahoy build remake`
5. Commit your changes
6. `git push origin [branch-name]` and create the PR
7. Merge if all tests pass
8. Create new tag for data_starter

### Reminders
- Adding or removing a module? Make sure to update the $features_master list in `assest/modules/data_config/data_config.module`
