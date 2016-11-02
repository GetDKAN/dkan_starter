Patching or "Overriding" DKAN
-----------------------------

DKAN overrides
^^^^^^^^^^^^^^

Sometimes you will need to make a patch directly to DKAN or a DKAN profile specific module.

These patches can go in ``confing/overrides.make``

Adding a Patch to Override DKAN
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Any patches to DKAN or modules supplied by DKAN (any code in the dkan/ folder) should be added to the ``config/overrides.make`` file as a patch linked by a URL or a local patch contained in ``config/assets/patches``.

Patches should be applied to the appropriate git repository, included in a pull request, and applied to your build as outlined below.

To test the application of the patch you can run

.. code-block:: bash

  ahoy build overrides

Overrides are applied in the build process when running 

.. code-block:: bash

  ahoy build update VERSION 
  
See Updating Data Starter to Latest Version of DKAN

Here is a step by step process:

1. Create the patch

   1. Create a PR
   
   2. Make sure the PR can be merged into the **release-1-12** (or the release number you are working off of)
   
   3. Get a diff of the PR
   
      1. Go to https://github.com/NuCivic/MODULE/compare or https://github.com/NuCivic/dkan/compare
      
      2. Select the upstream branch, ie release-1-12, and the PR you want to override with.
   
      3. Add **.diff** to the end 

.. image: https://cloud.githubusercontent.com/assets/512243/19907917/495b1638-a057-11e6-845d-c462be711f6d.png
    :alt: github view of comparison
   

2. Add the patch to the ``config/overrides.make``

.. code-block:: yml

    ---
    api: '2'
    core: 7.x
    projects:
      dkan_datastore:
        subdir: dkan
        download:
          type: git
          url: https://github.com/NuCivic/dkan_datastore.git
          tag: 7.x-1.12
        patch:
          1: "https://github.com/NuCivic/dkan_datastore/compare/release-1-12...fix-filters-pw-1.diff"

3. Test patch by running **ahoy build overrides**

   1. Patched module should appear in ``docroot/sites/all/modules/overrides``
  
Implementation Notes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
During **ahoy site remake** there is a step to append whatever is at the end of the overrides.make file to dkan/drupal-org.make file.  In effect we are introducing overrides to the make file because anything that is defined last a make file overrides whatever has been defined first.

Such overrides are put in overrides folder within ``docroot/sites/all/modules/overrides``.
