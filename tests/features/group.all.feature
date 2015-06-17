
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
  Scenario: View the list of published groups
    Given I am on the homepage
    When I follow "Groups"
    Then I should see "2" items in the "groups" region
    And I should not see "Group 03"

  @api @wip
  Scenario: View the details of a published group
    Given I am on "Groups" page
    When I follow "Group 01"
    Then I should see the "Group 01" detail page

  @api @wip
  Scenario: View the list of datasets on a group
    Given I am on "Group 01" page
    When I click "Datasets" in the "group information" region
    Then I should see "2" items in the "group datasets" region

  @api @wip
  Scenario: View the number of datasets on group
    Given I am on "Group 01" page
    When I click "Datasets" in the "group information" region
    Then I should see "2 datasets" in the "group datasets" region

  @api @wip
  Scenario: View the list of group members
    Given I am on "Group 01" page
    When I click "Members" in the "group information" region
    Then I should see "Gabriel" in the "group members" region
    And I should see "Katie" in the "group members" region
    And I should see "Jaz" in the "group members" region
    And I should not see "John" in the "group members" region

  @api @wip
  Scenario: Search datasets on group
    Given I am on "Group 01" page
    And I click "Datasets" in the "group information" region
    When I fill in "search" with "Dataset 01"
    And I press "Apply"
    Then I should see "1 datasets" in the "group datasets" region
    And I should see "1" items in the "groups datasets" region

  @api @wip
  Scenario: View available "resource format" filters after search
    Given I am on "Group 01" page
    And I click "Datasets" in the "group information" region
    When I fill in "search" with "Dataset"
    And I press "Apply"
    Then I should see "CSV (1)" in the "filter by resource format" region
    And I should see "XLS (1)" in the "filter by resource format" region

  @api @wip
  Scenario: View available "author" filters after search
    Given I am on "Group 01" page
    And I click "Datasets" in the "group information" region
    When I fill in "search" with "Dataset"
    And I press "Apply"
    Then I should see "Katie (1)" in the "filter by author" region
    And I should see "Gabriel (1)" in the "filter by author" region

  @api @wip
  Scenario: View available "tag" filters after search
    Given I am on "Group 01" page
    And I click "Datasets" in the "group information" region
    When I fill in "search" with "Dataset"
    And I press "Apply"
    Then I should see "Health (1)" in the "filter by tag" region
    And I should see "Government (1)" in the "filter by tag" region

  @api @wip
  Scenario: Filter datasets on group by resource format
    Given I am on "Group 01" page
    And I click "Datasets" in the "group information" region
    And I fill in "search" with "Dataset"
    And I press "Apply"
    When I click "CSV (1)" in the "filter by resource format" region
    Then I should see "1 datasets" in the "group datasets" region
    And I should see "1" items in the "groups datasets" region

  @api @wip
  Scenario: Filter datasets on group by author
    Given I am on "Group 01" page
    And I click "Datasets" in the "group information" region
    And I fill in "search" with "Dataset"
    And I press "Apply"
    When I click "Katie" in the "filter by author" region
    Then I should see "1 datasets" in the "group datasets" region
    And I should see "1" items in the "groups datasets" region

  @api @wip
  Scenario: Filter datasets on group by tags
    Given I am on "Group 01" page
    And I click "Datasets" in the "group information" region
    And I fill in "search" with "Dataset"
    And I press "Apply"
    When I click "Health" in the "filter by tag" region
    Then I should see "1 datasets" in the "group datasets" region
    And I should see "1" items in the "groups datasets" region