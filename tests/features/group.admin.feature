
Feature: Portal Administrators administer groups
  In order to manage site organization
  As a Portal Administrator
  I want to administer groups

  Portal administrators needs to be able to create, edit, and delete
  groups. They need to be able to set group membership by adding and removing
  users and setting group roles and permissions.


  Background:
    Given pages:
      | title  | url    |
      | Groups | /groups |
    Given users:
      | name    | mail             | roles                |
      | John    | john@test.com    | administrator        |
      | Badmin  | admin@test.com   | administrator        |
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
    And "Tags" terms:
      | name       |
      | Health     |
      | Government |
    And datasets:
      | title      | publisher | resource format | tags       | author  | published |
      | Dataset 01 | Group 01  | CSV             | Health     | Katie   | Yes       |
      | Dataset 02 | Group 01  | XLS             | Health     | Katie   | No        |
      | Dataset 03 | Group 01  | XLS             | Government | Gabriel | Yes       |

  @api @wip
  Scenario: Create group
    Given I am logged in as "John"
    And I am on "Groups" page
    And I follow "Add Group"
    When I fill in the following:
      | Title  |  My group      |
      | Body   | This is a body |
    And I press "Save"
    Then I should see the success message "Group My group has been created"
    And I should see the heading "My group"
    And I should see "This is a body"

  @api @wip
  Scenario: Add a group member on any group
    Given I am logged in as "John"
    And I am on "Group 02" page
    And I press "Group"
    And I click "Add people"
    When I fill in the "member" form for "Katie"
    And I press "Add users"
    Then I should see "Katie has been added to the group Group 02"
    When I am on "Group 02" page
    And I click "Members" in the "group information" region
    Then I should see "Katie" in the "groups information" region

  @api @wip
  Scenario: Remove a group member from any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    And I press "Group"
    And I click "People"
    And I click "remove" in the "Katie" row
    And I press "Remove"
    Then I should see "The membership was removed"
    When I am on "Group 01" page
    And I click "Members" in the "group information" region
    Then I should not see "Katie" in the "group information" region

  @api @wip
  Scenario: Delete any group
    Given I am logged in as "John"
    And I am on "Group 02" page
    When I press "Edit"
    Then I should see the button "Delete"
    When I press "Delete"
    Then I should see "Are you sure you want to delete"
    When I press "Delete"
    Then I should see "Group Group 02 has been deleted"

  @api @wip
  Scenario: Edit any group
    Given I am logged in as "John"
    And I am on "Group 02" page
    When I press "Edit"
    And I fill in "title" with "Goup 02 edited"
    And I press "Save"
    Then I should see "Group Goup 02 edited has been updated"
    And I should see the "Goup 02 edited" detail page

  @api @wip
  Scenario: Edit membership status of group member on any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    And I press "Group"
    And I click "People"
    And I click "edit" in the "Katie" row
    When I select "Blocked" from "status"
    And I press "Update membership"
    Then I should see "The membership has been updated"

  @api @wip
  Scenario: Edit group roles of group member on any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    And I press "Group"
    And I click "People"
    And I click "edit" in the "Katie" row
    When I check "administrator member"
    And I press "Update membership"
    Then I should see "The membership has been updated"

  @api @wip
  Scenario: View permissions of any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    And I press "Group"
    When I click "Permissions (read-only)"
    Then I should see the list of permissions for the group

  @api @wip
  Scenario: View group roles of any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    And I press "Group"
    When I click "Roles (read-only)"
    Then I should see the list of roles for the group

  @api @wip
  Scenario Outline: View group role permissions of any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    And I press "Group"
    And I click "Roles (read-only)"
    When I click "view permissions" in the "<role name>" row
    Then I should see the list of permissions for "<role name>" role

    Examples:
      | role name            |
      | non-member           |
      | member               |
      | administrator member |

  @api @wip
  Scenario: View the number of members on any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    And I press "Group"
    When I click "People"
    Then I should see "Total members: 2"

  @api @wip
  Scenario: View the number of content on any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    And I press "Group"
    When I click "People"
    Then I should see "Total content: 3"

  @api @wip
  Scenario: Add a sub-group on any group
    Given I am logged in as "John"
    And I am on "Group 01" page
    When I press "Edit"
    Then I should see "Parent group" field
    When I fill in "Parent group" with "Group 02"
    And I press "Update"
    Then I should see "Group Group 01 has been updated"
    When I am on "Group 02" page
    Then I should see "Group 01" in the "sub-groups" region

  # TODO: Change to use Workbench instead of /content

  @api @wip
  Scenario: View list of unpublished groups
    Given I am logged in as "John"
    And I am on "Content" page
    When I select "not published" from "status"
    And I select "group" from "type"
    And I press "Filter"
    Then I should see "Group 03" in the "search content results" region
    And I should see "1" items in the "search content results" region

  @api @wip
  Scenario: View the details of an unpublished group
    Given I am logged in as "John"
    When I am on "Group 03" page
    Then I should see the "Group 03" detail page


