
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
  Scenario: Login
    Given I am on the homepage
    When I follow "Log in"
    And I fill in "username" with "john"
    And I fill in "password" with "johnpass"
    Then I should see the "John" page

  @api @wip
  Scenario: Logout
    Given I am logged in as "John"
    And I am on the homepage
    When I follow "Log out"
    Then I should see "Log in"
    When I am on "John" page
    Then I should see "Page not found"

  @api @wip
  Scenario: Register
    Given I am on the homepage
    When I follow "Register"
    And I fill in "username" with "newuser"
    And I fill in "email" with "newuser@email.com"
    And I press "Create new account"
    Then I should see "Thank you for applying for an account."
    And I should see "Your account is currently pending approval by the site administrator."

  @api @wip
  Scenario: Request new password
    Given I am on the homepage
    When I follow "Log in"
    And I follow "Request new password"
    And I fill in "username or email" with "john@test.com"
    And I press "E-mail new password"
    Then a notification should be sent to "John"
    #TODO: Follow reset password link on email?

  @api @wip
  Scenario: View user profile
    Given I am on "Group 01" page
    And I follow "members"
    When I click "Katie"
    Then I should see "Katie" page

  @api @wip
  Scenario: View list of published datasets created by user on user profile
    Given I am on "Katie" page
    And I click "Datasets"
    Then I should see "2" items in the "datasets" region

  @api @wip
  Scenario: Search datasets created by user on user profile
    Given I am on "Katie" page
    And I click "Datasets"
    When I fill in "search" with "Dataset 01"
    And I press "Apply"
    Then I should see "1 datasets" in the "datasets" region
    And I should see "1" items in the "datasets" region

  @api @wip
  Scenario: See list of user memberships on user profile
    Given I am on "Katie" page
    And I click "Groups"
    Then I should see "Group membership:"
    And I should see "1" items in the "groups" region
    And I should see "Group 01" in the "groups" region
