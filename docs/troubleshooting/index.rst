Troubleshooting FAQ
-------------------

These are FAQ issues that DKAN Starter or DKAN users may run into while using the docker and ahoy tools.

Please submit tickets to the `DKAN Issue Queue <http://github.com/GetDKAN/dkan/issues>`_.

Frequently Asked Questions
~~~~~~~~~~~~~~~~~~~~~~~~~~

"ENVIRONMENT set to , but not mapped in settings.php"
=====================================================

When trying to connect to local docker machine:

.. warning::
    ENVIRONMENT set to , but not mapped in settings.php

* Your "settings.docker.php" file is not set. Run **ahoy site up**
* Can also be caused by the MySQL container missing:

.. code-block:: bash

    ahoy diagnose all
    ahoy docker up

Container not running
=====================================================

If you get an error when running ``ahoy diagnose all`` like:

.. code-block:: bash

    [Error] The "db" container is not running: mysite_db_1          /entrypoint.sh mysqld            Exit 1

then you can try:

1. Restart the containers ``ahoy docker stop; ahoy docker up``
2. Destroy the containers (WARNING: YOU WILL LOSE DATA) ``ahoy docker destroy; ahoy site up``
3. Restart the docker maching ``docker-machine stop default; docker-machine start default; eval "$(docker-machine env default)"; ahoy site up``

Could not open connection: Curl error
=====================================

When running tests:

.. warning::
    Could not open connection: Curl error thrown for http POST to http://browser:4444/wd/hub/session

* Your browser container is not running. Run **ahoy diagnose all** to troubelshoot

.. code-block:: bash

    ERROR 1064 (42000) at line 1: You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near 'env: drush9: No such file or directory' at line 1
    [error] The command could not be executed successfully (returned: env: drush9: No such file or directory, code: 127)

* Make sure you are using drush version 8.0.2 (not 9).

ahoy dkan test... stalls on a step or very slow
===================================================

* Do a complete cleanup:

.. code-block:: bash

    ahoy docker cleanup
    ahoy docker stop
    docker-machine stop default
    docker-machine start default
    ahoy docker up
