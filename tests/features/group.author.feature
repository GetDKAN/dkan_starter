
Feature: Portal Administrators administer groups
  In order to manage site organization
  As a Portal Administrator
  I want to administer groups

  Portal administrators needs to be able to create, edit, and delete
  groups. They need to be able to set group membership by adding and removing
  users and setting group roles and permissions.


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

  @api @wip
  Scenario: Request group membership
    Given I am logged in as "Kathie"
    And I am on "Group 02" page
    When I click "Request group membership"
    And I fill in "request message" with "Please let me join!"
    And I press "Join"
    Then I should see "Your membership is pending approval." in the "group information" region
    And I should see "Remove pending membership request" in the "group information" region

  @api @wip
  Scenario: Cancel membership request
    Given I am logged in as "Kathie"
    And I am on "Group 02" page
    When I click "Request group membership"
    And I fill in "request message" with "Please let me join!"
    And I press "Join"
    Then I should see "Remove pending membership request" in the "group information" region
    When I click "Remove pending membership request"
    And I press "Remove"
    Then I should see the "Group 02" detail page
    And I should see "Request group membership" in the "group information" region

  @api @wip
  Scenario: Leave group
    Given I am logged in as "Katie"
    And I am on "Group 01" page
    When I click "Unsubscribe from group"
    And I press "Remove"
    Then I should see the "Group 01" detail page
    And I should see "Request group membership" in the "group information" region

  @api @wip
  Scenario: I should not be able to edit groups
    Given I am logged in as "Katie"
    When I am on "Group 01" page
    Then I should not see the link "Edit"
    And I should not see the link "Group"