
Feature: User

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
      | Celeste | Group 02 | member               | Active            |
    And datasets:
      | title      | group    | resource format | tags      | author  | published |
      | Dataset 01 | Group 01 | CSV             | Health    | Katie   | Yes       |
      | Dataset 02 | Group 01 | XLS             | Health    | Katie   | No        |
      | Dataset 03 | Group 01 | XLS             | Goverment | Gabriel | Yes       |
      | Dataset 04 | Group 01 | CSV             | Health    | Katie   | Yes       |


  @api @wip
  Scenario: Edit own user account
    Given I am logged in as "Katie"
    And I am on "Katie" page
    When I follow "Edit"
    And I fill in "about" with "This is my profile"
    And I press "Save"
    Then I should see "The changes have been saved"
    When I am on "Katie" page
    And I follow "About"
    Then I should see "This is my profile" in the "about" region

  @api @wip
  Scenario: View the list of own published and unpublished datasets on profile
    Given I am logged in as "Katie"
    And I am on "Katie" page
    When I click "Datasets"
    Then I should see "3" items in the "datasets" region

  # TODO: Check if resources are shown on user profile
  @api @wip
  Scenario: View the list of own published and unpublished resources on profile
    Given I am on the homepage

  # TODO: Needs definition.
  @api @wip
  Scenario: User should be logged out automatically after N minutes
    Given I am on the homepage
