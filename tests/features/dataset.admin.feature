
Feature: Dataset Features
  In order to realize a named business value
  As an explicit system actor
  I want to gain some beneficial outcome which furthers the goal

  Additional text...


  Background:
    Given users:
      | name    | mail             | roles                |
      | John    | john@test.com    | portal administrator |
      | Admin   | admin@test.com   | portal administrator |
      | Gabriel | gabriel@test.com | content editor       |
      | Jaz     | jaz@test.com     | data contributor     |
      | Katie   | katie@test.com   | data contributor     |
      | Martin  | martin@test.com  | data contributor     |
      | Celeste | celeste@test.com | data contributor     |
    Given groups:
      | title    | author | published |
      | Group 01 | Admin  | Yes       |
      | Group 02 | Admin  | Yes       |
      | Group 03 | Admin  | No        |
    And group memberships:
      | user    | group    | role on group        | membership status |
      | Gabriel | Group 01 | administrator member | Active            |
      | Katie   | Group 01 | member               | Active            |
      | Jaz     | Group 01 | member               | Pending           |
      | Admin   | Group 02 | administrator member | Active            |
      | Celeste | Group 02 | member               | Active            |
    And datasets:
      | title      | format | author  | published        | Date         | tags   |
      | Dataset 01 | CSV    | Gabriel | Yes              | Feb 01, 2015 | Health |
      | Dataset 02 | XLS    | Gabriel | Yes              | Mar 13, 2015 | Gov    |
      | Dataset 03 | CSV    | Katie   | Yes              | Feb 17, 2013 | Health |
      | Dataset 04 | CSV    | Celeste | No, Draft        | Dic 21, 2015 | Gov    |
      | Dataset 05 | CSV    | Katie   | No, Needs review | Dic 21, 2015 | Gov    |
    And resources:
      | title       | dataset    | published |
      | Resource 01 | Dataset 01 | Yes       |
      | Resource 02 | Dataset 01 | Yes       |
      | Resource 03 | Dataset 02 | Yes       |

  @api @wip
  Scenario: Review any dataset
    Given I am logged in as "John"
    When I am on "Needs Review" page
    Then I should see "Dataset 05"
    When I click "Change to Published" in the "Dataset 05" row
    Then I should see "Email notifications sent"
    When I am on "Needs Review" page
    Then I should not see "Dataset 05"

  @api @wip
  Scenario: Edit any dataset
    Given I am logged in as "John"
    And I am on "Dataset 03" page
    When I click "Edit"
    And I fill in "title" with "Dataset 03 edited"
    And I press "Save"
    Then I should see "Dataset Dataset 03 edited has been updated"

  @api @wip
  Scenario: Delete any dataset
    Given I am logged in as "John"
    And I am on "Dataset 03" page
    When I click "Delete"
    And I press "Delete"
    Then I should see "Dataset Dataset 03 has been deleted"

  @api @wip
  Scenario: Publish any dataset
    Given I am logged in as "John"
    And I am on "Dataset 05" page
    When I click "Edit"
    And I select "published" from "publishing options"
    And I press "Save"
    Then I should see "Dataset Dataset 05 has been updated"
