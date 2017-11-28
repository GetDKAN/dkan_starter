Add custom configuration
------------------------

By custom configuration we mean:

* A new Content Type
* A new View
* A new Page
* Whatever can be captured in features in a custom module

Whatever's you need to customise the project, you should set the feature export
to somewhere inside the **docroot/sites/all/modules/custom** folder. That way,
your module will **persist** when the site gets **remade** (Read Add a custom
module if you don't understand what this means). Please note your changes shouldn't be added directly to docroot/sites/all/modules/custom but to **config/modules/custom**, otherwise, your changes will be lost on the next DKAN Starter upgrade.

However, there's a few caveats:

1. If you add something custom to the site, you need to make sure it gets tested every time new code gets added to the project (on a Pull Request).
  1. It doesn't need to be the fanciest behat test, something like this will do ->  `tests/features/general.feature <https://github.com/GetDKAN/data_starter_private/blob/master/tests/features/general.feature>`_
  2. Create a behat feature file for your tests (name it appropriately)
  3. Look at the ``circle.yml`` recipe to see if it's setup to run that behat feature
2. **Customisation** does not mean in any way **DKAN overrides.** If you plan to introduce overrides please refer to Override a DKAN out of the box feature
