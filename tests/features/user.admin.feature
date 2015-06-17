
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
  Scenario: Edit any user account
    Given I am logged in as "John"
    And I am on "Users" page
    When I click "edit" in the "Katie" row
    And I fill in "about" with "This is Katie!"
    And I press "Save"
    Then I should see "The changes have been saved"
    When I am on "Katie" page
    And I follow "About"
    Then I should see "This is Katie!" in the "about" region

  @api @wip
  Scenario: Create user
    Given I am logged in as "John"
    And I am on "Users" page
    When I follow "Add user"
    And I fill in the "user" form for "Micaela"
    And I press "Create new account"
    Then I should see "Created a new user account for Micaela."

  @api @wip
  Scenario: Block user
    Given I am logged in as "John"
    And I am on "Users" page
    When I click "edit" in the "Katie" row
    And I check "Blocked"
    And I press "Save"
    Then I should see "The changes have been saved"
    When I am on "Users" page
    Then I should see "blocked" as "status" in the "Katie" row

  @api @wip
  Scenario: Disable user
    Given I am logged in as "John"
    And I am on "Users" page
    When I click "edit" in the "Katie" row
    And I press "Cancel account"
    And I check "Disable the account and keep its content."
    And I press "Cancel account"
    Then I should see "dan has been disabled"

  @api @wip
  Scenario: Modify user roles
    Given I am logged in as "John"
    And I am on "Users" page
    When I click "edit" in the "Katie" row
    And I uncheck "data contributor"
    And I check "content editor"
    And I press "Save"
    Then I should see "The changes has been saved"
    When I am on "Users" page
    Then I should see "content editor" as "Roles" in the "Katie" row

  # TODO: Needs definition

  @api @wip
  Scenario: Search users
    Given I am on the homepage






