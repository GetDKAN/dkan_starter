Update DKAN Starter
-------------------

1. `git checkout master`
2. `git checkout -b [branch-name]`
2. Update line 11 in **build-dkan.make** with the latest tag from `DKAN <https://github.com/GetDKAN/dkan/releases>`_
3. `ahoy build remake`
4. Commit your changes
5. `git push origin [branch-name]` and create the PR
6. Merge if all tests pass
7. Create new tag for data_starter

### Reminders
- Adding or removing a module? Make sure to update the $features_master list in `assest/modules/data_config/data_config.module`
