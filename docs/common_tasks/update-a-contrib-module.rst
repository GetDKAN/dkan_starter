Update a contributed module
---------------------------

Please read Add a contributed module first.

Edit the module entry in the ``build.make`` file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Look for the module in the ``build.make``. If it looks like this:

.. code-block:: bash

projects[] = sharethis


Then you can just remake since it'll grab the latest stable version for the module. Instead, if it's set to a specific version:

.. code-block:: bash
 projects[sharethis][version] = 2.12

Then make the edit to the desired version and then remake

Remake the project
^^^^^^^^^^^^^^^^^^

Run:

.. code-block:: bash
 
 ahoy build remake

That should update the **sharethis** module at ``docroot/sites/all/modules/contrib``. 
