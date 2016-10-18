# Update Data Starter

There is a new command **"ahoy build update VERSION".**

**VERSION** is the latest data_starter release here: https://github.com/NuCivic/data_starter_private/releases

This performs the following:

* **ahoy data-starter-update VERSION**  downloads data_starter_private and updates everything outside of the 'config' folder.
* **ahoy build custom updates** the custom modules in custom.make
* **ahoy build overrides** patches dkan with items from overrides.make
* **ahoy build config** adds config in config.yml to various items

Full command: https://github.com/NuCivic/nucivic-ahoy/

This removes everything outside of the config/ folder with the data_starter updates and then applies the local overrides and settings that are contained in the config folder.

If a module is removed from sites/all/modules/contrib after running **ahoy build update VERSION** then it was not defined in custom.make.
