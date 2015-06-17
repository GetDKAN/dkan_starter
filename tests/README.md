# DKAN Data Starter: Specs

## Installation

You'll need to have at least php 5.3, then to install using composer.

`composer install` which will install the dependencies. It's best to place this directory one level below the drupal root.

Make sure you have a drush alias setup in behat.yml @data_starter.local is the default. The site should be available at http://data_starter.local or again, you need to update behat.yml

Remember that all of your tests that use the drupal api need to have @api tagged to them.

Debugging Best Practices
=========================

Tag broken tests as @wip (work in progress)
-----------------------------

Adding a @wip tag to a scenario will cause the whole scenario to be skipped and not show up as a failure. It will still remind you that a test (or the underlying bug needs to be fixed). The goal is that existing broken tests will be skipped so that we get everything green first and it's more apparent when we have regressions.

Example:
```
@wip
Scenario: This is a test that is currently broken and being worked on.
  Given I have a scenario tagged as @wip
  When I run my tests
  Then any tests with @wip will be skipped.
```

Use the built in "And I break" step to pause tests and debug issues
--------------------------------------------------------------------

It's often helpful to use the tests to debug complex issues. Normally the test will run through all steps and then finally clean any users or content created during a scenario. This makes it hard to debug an issue because all the evidence of the failure has been deleted. However, you can pause behat mid-scenario by adding a step called "And I break", which will wait until the Enter key is pressed before continuing. That means you can add the "And I break" step BEFORE the step that's failing, and then browse the site to diagnose the issue in more detail. When you want behat to continue on, you just hit enter in the command line. If you put the "And I break" after a failing step then it will end up just being skipped. Also, be careful not to commit with "And I break" in any test since it will screw up the test server. (The server doesn't ever press enter)

Example:
```
Scenario: This is a test that fails because text is missing.
  Given I have a scenario that is failing.
  And I break
  And I see "missing text"
  Then behat will pause at 'And I break' and not run 'And I see "missing text"' until I press enter
```

For stepping through a whole scenario, use our custom @debugEach tag
---------------------------------------------------------------------

Sometimes it's useful to step through a failing test instead of using "And I break". We've created a custom tag that allows you to do just that. It works like "And I break" was added after each step in a scenario with the addition of "And print current URL" so that you can see what pages are being viewed at each step along the way. You can also use @debugBeforeEach if you want to pause before each step. (You should usually only use one or the other)

Example:
```
@debugEach
Scenario: This is a test that fails because text is missing.
  Given I have a scenario that is failing at - And i see "missing text"
  # Pause
  And I see "missing text"
  # Pause
  Then start WW3
  # Pause
```
