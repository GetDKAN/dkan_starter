Setting up Your Local Docker
----------------------------

The steps to setup the DKAN development environment are as following:

1. Docker
^^^^^^^^^
Install `docker on your local environment <https://www.docker.com/community-edition>`_.

Ahoy utilizes docker-compose and requires a functioning Docker installation. Verify ``docker info`` is working prior to continuing.

2. Ahoy
~~~~~~~
Install `ahoy on your local environment <https://www.docker.com/community-edition>`_ (Currently requires version 1.X, 2.X will not work).

For Mac OS, use:

.. code-block:: bash

  brew tap devinci-code/tap
  brew install ahoy

For Linux, use:

.. code-block:: bash

  sudo wget -q https://github.com/DevinciHQ/ahoy/releases/download/1.1.0/ahoy-`uname -s`-amd64 -O /usr/local/bin/ahoy && sudo chown $USER /usr/local/bin/ahoy && chmod +x /usr/local/bin/ahoy


3. Getting started with DKAN development
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To get started with DKAN development you need to initially follow these steps:

.. code-block:: none

  cd ~/workspace
  git clone git@github.com:NuCivic/dkan.git
  cd dkan
  bash dkan-init.sh dkan
  ahoy docker env (Follow instructions)
  ahoy dkan drupal-rebuild mysql://drupal:123@db/drupal
  ahoy dkan remake
  ahoy dkan reinstall (Can take 15-30 mins)
  ahoy uli

To run Behat automated test suite:

.. code-block:: bash

  cd ~/workspace/dkan
  ahoy dkan test

Once project is built, to run Docker environment again later:

.. code-block:: bash

  cd ~/workspace/dkan
  eval "$(ahoy docker env)"

Other Available Commands:

.. code-block:: bash

  ahoy drush {args} - run drush command in container
  ahoy uli - login as admin via drush uli
  ahoy dkan - lists available dkan commands
  ahoy docker - lists docker commands available

Local Site URL: http://dkan.docker

In the dkan folder you can see the following directories:

  - ``dkan/``: The git repository of dkan profile
  - ``docroot/``: The docroot with a fresh Drupal installation and a symlink from docroot/profiles/dkan to dkan (git repository)
  - ``backups/``: Contains SQL backup/dump file (last_install.sql) for the last DKAN site reinstall.

Make changes to dkan, add, commit and push.
When done developing for this project execute the following command: ``ahoy docker stop``

4. Troubleshooting
^^^^^^^^^^^^^^^^^^

Verify dkan docker containers are running:

.. code-block:: bash

  docker ps

  Output should look like:
  CONTAINER ID        IMAGE                                     COMMAND                  CREATED             STATUS              PORTS                                                              NAMES
  f83a1eb95730        nuams/drupal-cli:2016-10-16               "/opt/startup.sh /..."   2 hours ago         Up 2 hours                                                                             dkan_cli_1
  0ca4da28f0ae        selenium/standalone-chrome-debug:2.53.1   "/opt/bin/entry_po..."   2 hours ago         Up 2 hours          4444/tcp, 5900/tcp                                                 dkan_browser_1
  d5f864808585        nuams/drupal-apache-php:1.0-5.6           "/bin/sh -c '/opt/..."   2 hours ago         Up 2 hours          80/tcp, 443/tcp                                                    dkan_web_1
  4116abdb824c        blinkreaction/mysql:5.5                   "/entrypoint.sh my..."   3 days ago          Up 3 hours          0.0.0.0:32768->3306/tcp                                            dkan_db_1
  4580f9c04f83        jwilder/nginx-proxy                       "/app/docker-entry..."   3 days ago          Up 3 hours          0.0.0.0:80->80/tcp, 0.0.0.0:443->443/tcp, 0.0.0.0:5959->5900/tcp   dkan_proxy
  242ef176e340        memcached                                 "docker-entrypoint..."   3 days ago          Up 3 hours          11211/tcp                                                          dkan_memcached_1
  2f8106a4ce55        devinci/drupal-solr:3.x                   "/bin/sh -c /opt/s..."   3 days ago          Up 3 hours          8983/tcp                                                           dkan_solr_1


If a container is not running, you can look at logs via ``docker logs dkan_web_1`` for example.

5. Getting started with DKAN Starter development
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
See: `Setting up a project locally <../common_tasks/setting-up-local-project>`_
