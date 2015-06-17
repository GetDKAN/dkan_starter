Feature: Some terse yet descriptive text of what is desired
  In order to realize a named business value
  As an explicit system actor
  I want to gain some beneficial outcome which furthers the goal

  Additional text...

  @api @test
  Scenario: Test basic functionality.
    Given I am on the homepage

  @api @wip
  Scenario: Test the drupal drivers.
    Given I am logged in as a user with the "administrator" role

  @api @wip
  Scenario: Test addPages()
    Given pages:
      | title    | url      |
      | Content | /content |
      | User    | /user    |
    And I am on the "User" page

  @api @wip
  Scenario: Test iSearchFor()
    Given I am on the homepage
    And I search for "Datasets"

  @api @wip
  Scenario: Test addGroups()
    Given groups:
      | title    | author | published |
      | Group 01 | John   | Yes       |
      | Group 02 | Dan    | Yes       |
      | Group 03 | Jaz    | No        |
    And I am on the "Group 01" page
    Then I should see "Group 01"