Overview of the dkan and dkan_starter behat suite setup

This page describes how behat profiles and suites have been configured in dkan and dkan_starter.
Behat suite overview as they apply to dkan and dkan_starter
Behat (3.1)  allows for many levels of control of the testing environment, two of which are profile and suite level controls.
Profiles:
Profile level control is over arching and at this level behat allows users to import configuration files that can be composed into new configurations. For example:
imports:
        1.  dkan is the default profile
  -  ../dkan/test/behat.yml
        1.  data_stater will override dkan
  -  behat.data_starter.yml
        1.  custom has the last say
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

In the above configuration we see contents of the `tests/behat.docker.yml` file.  Note the imports: field.  When we invoke behat using the following command:
behat --config=behat.docker.yml

Then, what is happening is that we are using the composition of merging the default profiles from ../dkan/test/behat.yml,  behat.data_starter.yml and ../config/tests/behat.custom.yml in that order.  This is the approach we are currently taking in our dkan_starter setup.  This approach allows us to use the behat configuration in dkan/test/behat.yml as an upstream configuration that does not need to be copied and maintained outside of dkan as the case used to be.
Suites:
Although profiles are uniquely suited for deriving and composing appropriate configurations from a reusable configuration files subset, they do not allow for nuanced level configuration at the testing specification level.  We have assessed three distinct subsets of tests; there are dkan specific tests, there are tests that are specific to dkan_starter and finally there are tests that are specific to site level features that are not part of dkan oob.
in the past we have maintained completely separate configurations to be able to handle the testing needs at each of these levels.  By adopting the use of behat suites, we now can control our testing from one origin and supply the needed configuration and configuration overrides via the use of suite configurations and profile compositions as described in the profile section above.
We have implemented three defaults called custom, data_starter, and dkan.
Below see the example of the data_starter suite as defined in tests/behat.data_starter.yml:
1.  this default label refers to a profile level configuration
default:
        1.  autoloads can only be handled in behat using PS-0 at the profile level
        1.  otherwise use composer.json and PS-3
  autoload:
                1.  PS-0 forces us to use a symlink inside of the profile level bootstrap
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
      1.  handle dkan path here to maintain backwards compatibility
      paths:
        - %paths.base%/../dkan/test/features

