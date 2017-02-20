DKAN Starter Annotated
----------------------


DKAN Starter Structure
^^^^^^^^^^^^^^^^^^^^^^

The following is the root structure for DKAN Starter:

.. code-block:: bash

  .ahoy/                       [Ahoy files. See note below.]
  CHANGELOG.md                 [Changelog for DKAN Starter. All releases include an entry.]
  README.md                    [Description of DKAN Starter.]
  build-dkan.make              [Drupal Make file for DKAN version.]
  circle.yml                   [CircleCI test file.]
  docroot/                     [Built docroot. See note below.]
  drupal-org-core.make         [Drupal Make file for Drupal version.]
  OWNERS.md                    [Owners file for DKAN Starter.]
  assets/                      [DKAN Starter assets. See note below.]
  build.make                   [Drupal make file for DKAN Starter modules. These are useful modules not included in DKAN.]
  config/                      [The only place you should touch. See note below.]
  dkan/                        [Fully-built DKAN profile directory. This is simlinked from the ``docroot/profiles/dkan`` folder.]
  docs/                        [Documentation folder.]
  hooks/                       [Deployment hooks for changing environments (ie dev to test, test to production)]
  tests/                       [Tests]


``docroot/``
~~~~~~~~~~~~

This is the docroot that the webserver on your environments will point to. The docroot is never edited directly and is always rebuilt from a combination of sources. Everything except for:

* ``sites/all/modules/custom``
* ``sites/all/themes/custom``
* ``sites/all/modules/overrides``

come from the upstream version of DKAN Starter.

The docroot is rebuilt by running *ahoy build update* command.

``.ahoy/``
~~~~~~~~~~

This contains our ahoy scripts. All parts of the DKAN Starter workflow run through the ahoy commands.

``assets/``
~~~~~~~~~~~

This folder includes DKAN Starter modules, patches, and other miscellaneous assets.

``config/``
~~~~~~~~~~~

In an effort to simplify how we configure and customize projects we have  added `./config` to capture all configurations and customizations to a dkan_starter project.
The idea with this folder is to capture all customizations across our sites in one place  as well as to separate the logic that uses custom configuration from the configuration itself.
The current structure of the config folder is:

.. code-block:: bash

  config/
    aliases.local.php
    config.php
    config.yml
    custom.ahoy.yml
    custom.make
    custom_libs.make
    overrides.make
    modules/
        custom/
            README.md
            custom_config/
    patches/
      README.md
    settings.custom.php
    tests/
       features/
          general.feature

Most of the files in the above structure are self explanatory, and as you can see in most cases we have simply moved existing files from the legacy structure to this new folder.  Moving these files here satisfies the first condition of this change: to capture all parts that are customizable in one place.
The new files that have not been seen in previous setups are the config.yml, config.php, and custom.settings.php.

config.php
~~~~~~~~~~~

You should forget about it.  This file is created automatically by running `ahoy build config` and it is derived in part by what you add to `./config/config.yml`.

config.yml
~~~~~~~~~~~

This is where we will now keep all of the site specific configurations.  This file is a yaml formatted file that will not contain any logic (by definition) and thus simplifies understanding how sites differ from each other.

settings.custom.php
~~~~~~~~~~~~~~~~~~~

This is where settings.php logic that is custom to a site will live, so that it will be much easier to see how, if at all, a site's settings logic is different from another.  Currently we use the devinci along with the environment module to automatically run changes between environments.  Often there are site specific differences that happen between the different installations.  This is where we can capture these logic difference.  Note, that we may move away from how we run deployments (a la devinci style) so this file may become unnecessary.

custom.make
~~~~~~~~~~~

This is where contributed modules are added. Contributed modules are defined as modules that live outside of your project. This make file gets rebuilt when your site is updated.

aliases.local.php
~~~~~~~~~~~~~~~~~

Houses local aliases for your project. This file is set by running ``ahoy utils name``.

custom.ahoy.yml
~~~~~~~~~~~~~~~

Custom ahoy commands are added here.

custom_libs.make
~~~~~~~~~~~~~~~~~~~~

3rd-party libraries are added to this make file.

``modules/custom/``
~~~~~~~~~~~~~~~~~~~

Custom modules built for this project are added here. Any modules added to this folder are added to ``docroot/sites/all/modules/custom`` through a symlink.

``modules/custom/custom_config/``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This module is for custom configuration of DKAN. Implementers can use this module or another custom module for customizations.

custom_config.features.features_master.inc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``modules/custom/custom_config/custom_config.features.features_master.inc`` file contains a list of the modules that you want enabled on your site.
