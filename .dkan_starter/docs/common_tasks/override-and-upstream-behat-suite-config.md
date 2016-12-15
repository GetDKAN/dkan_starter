# Override an Upstream Behat Suite Configuration

Sometimes site developers may have a need to override an upstream suite configuration as is the case if they need to filter out specific dkan tests for whatever reason.  This page describes the steps needed when a developer is faced with such a task

### Step-by-step guide

1. open up config/tests/behat.custom.yml
2. If an entry for the suite you want to override is not present, then add it
3. Add filters filters or override what contexts or where contexts being loaded from.  The example below shows us adding a name: filter to the dkan suite.

**config/tests/behat.custom.yml**
```
# behat.yml
default:
  suites:
    custom:
      paths:
        - %paths.base%/../config/tests/features
      contexts:
        - CustomContext
        - FeatureContext #Temporary overrides only!
        - Drupal\DrupalExtension\Context\MinkContext
        - Drupal\DrupalExtension\Context\DrupalContext
        - Drupal\DrupalExtension\Context\MessageContext
    dkan:
      filters:
        - name: Viewing the site title
```
