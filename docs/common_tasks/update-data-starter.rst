Update DKAN Starter
-------------------

There is a new command 

.. code-block:: bash
  
  ahoy build update VERSION

**VERSION** is the latest dkan_starter release here: `https://github.com/NuCivic/dkan_starter/releases <https://github.com/NuCivic/dkan_starter/releases>`_.

This performs the following:

* **ahoy build update-dkan-starter VERSION**  downloads dkan_starter and updates everything outside of the 'config' folder.
* **ahoy build custom updates** the custom modules in custom.make
* **ahoy build overrides** patches dkan with items from overrides.make
* **ahoy build config** adds config in config.yml to various items

This removes everything outside of the config/ folder with the dkan_starter updates and then applies the local overrides and settings that are contained in the config folder.

If a module is removed from sites/all/modules/contrib after running ``ahoy build update VERSION`` then it was not defined in ``custom.make``.
