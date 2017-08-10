Overview of the dkan and dkan_starter behat suite setup
===========================

This page describes how behat profiles and suites have been configured in dkan and dkan_starter.

Behat suite overview as they apply to dkan and dkan_starter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Behat (3.1)  allows for many levels of control of the testing environment, two of which are profile and suite level controls.

Profiles:
---------

Profile level control is over arching and at this level behat allows users to import configuration files that can be composed into new configurations. For example:

.. code-block:: yaml

  // tests/behat.docker.yml
  imports:
    # dkan is the default profile
    -  ../dkan/test/behat.yml
    # data_stater will override dkan
    -  behat.data_starter.yml
    # custom has the last say
    -  ../config/tests/behat.custom.yml
  default:
    extensions:
      Behat\MinkExtension:
        base_url: http://web
        selenium2:
          wd_host: http://browser:4444/wd/hub
          browser: chrome
      Drupal\DrupalExtension:
        drupal:
          drupal_root: '/var/www/docroot'

In the above configuration we see contents of the ``tests/behat.docker.yml`` file.  Note the imports: field.  When we invoke behat using the following command:

  .. cone-block::bash

	behat --config=behat.docker.yml

Then, what is happening is that we are using the composition of merging the default profiles from ``../dkan/test/behat.yml``,  ``behat.data_starter.yml`` and ``../config/tests/behat.custom.yml`` in that order.  This is the approach we are currently taking in our dkan_starter setup.  This approach allows us to use the behat configuration in dkan/test/behat.yml as an upstream configuration that does not need to be copied and maintained outside of dkan as the case used to be.

Suites:
--------
Although profiles are uniquely suited for deriving and composing appropriate configurations from a reusable configuration files subset, they do not allow for nuanced level configuration at the testing specification level.  We have assessed three distinct subsets of tests; there are dkan specific tests, there are tests that are specific to dkan_starter and finally there are tests that are specific to site level features that are not part of dkan oob.

In the past we have maintained completely separate configurations to be able to handle the testing needs at each of these levels.  By adopting the use of behat suites, we now can control our testing from one origin and supply the needed configuration and configuration overrides via the use of suite configurations and profile compositions as described in the profile section above.

We have implemented three defaults called custom, data_starter, and dkan.
Below see the example of the data_starter suite as defined in ``tests/behat.data_starter.yml``:

# this default label refers to a profile level configuration

.. code-block:: yaml

  default:
    # autoloads can only be handled in behat using PS-0 at the profile level
    # otherwise use composer.json and PS-3
    autoload:
      # PS-0 forces us to use a symlink inside of the profile level bootstrap
      - %paths.base%/features/bootstrap/custom
      - %paths.base%/features/bootstrap/dkan
      - %paths.base%/features/bootstrap
    suites:
      data_starter:
        paths:
          - %paths.base%/features
        contexts:
          - FeatureContext #Temporary overrides only!
          - Drupal\DrupalExtension\Context\MinkContext
          - Drupal\DrupalExtension\Context\DrupalContext
          - Drupal\DrupalExtension\Context\MessageContext
      dkan:
        # handle dkan path here to maintain backwards compatibility
        paths:
          - %paths.base%/../dkan/test/features

Context Attributes:
___________________

Behat allows for the passing of attributes to contexts via configuration.  For example in the below code snippet we are passing two attributes to an underlying constructor for the context:

.. code-block:: yaml

      - Drupal\DKANExtension\Context\DatasetContext:
        - fields:
            title: title
        - labels:
            title: Title

Indeed, when the above configuration gets processed it will be used by a behat Factory class to generate an instance of the Context class.  Each value in the above array is passed as an argument:

.. code-block:: php

  /**
   * Defines application features from the specific context.
   */
  class DatasetContext extends RawDKANEntityContext {

    public function __construct($fields, $labels = array(), $sets = array(), $defaults = array()) {
      $this->datasetFieldLabels = $labels['labels'];
      $this->datasetFieldSets = $sets['sets'];

It probably goes without saying (but I'll say it anyway) that this feature can be leveraged to make a failing test in a site context pass if the underlying cause of the failure is do to a change to any of the above attribute values.  For example let's imagine that we make the title of a dataset a required field and that because of this change the title of that label is "Title \*" and not "Title".  If there is a test that depends on the value to be exaclty "Title" then by updating thie value via the behat context attribute we can avoid that test failure.


How to reconfigure Behat Context attributes:
~~~~~~~~~~~~~~~
So, we know that via behat context attributes we are provided with a way to update site changes to avoid test failures.  But how can we do this in a way that does not get lost and is easy to maintain?

We now have updated the `ahoy build config` command to apply any changes made to the following config.yml attributes to the behat.yml, behat.dkan_starter.yml, and behat.custom.yml files:

.. code-block:: yaml

  behat:
    contexts:
      datasets:
        defaults: {}
        fields: {}
        labels: {}
        sets: {}
      services:
        request_fields_map: {}

So for example let's say that instead of "Groups" we decide to call them "Agencies" then we would make the following change and apply it by runn `ahoy buld config`:

.. code-block:: yaml

  behat:
    contexts:
      datasets:
        defaults: {}
        fields: {}
        labels:
          og_group_ref: Agencies
        sets: {}
      services:
        request_fields_map: {}

Caveats:
~~~
This process can add or update values in the behat.yml files but it will not remove values.  If a site requires custom field (adding a value) that later needs to be removed `ahoy build confi` will not remove that for you even if you remove it from the config.yml file.  You will need to manually remove that item. 
