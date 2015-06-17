
Feature: General


  ##################################################################
  # ALL ( ANONYMOUS + AUTHENTICATED )
  ##################################################################

  @api @wip
  Scenario: See about page
    Given I am on the homepage
    When I follow "about"
    Then I should see the "about" page

  @api @wip
  Scenario: See tools and resources for developers page
    Given I am on the homepage
    When I follow "developers"
    Then I should see the "developers" page

  # TODO: Needs definition

  @api @wip
  Scenario: Access JSON representation of data via /data.json
    Given I am on the homepage

  # TODO: Needs definition

  @api @wip
  Scenario: View a list of available HHS APIs
    Given I am on the homepage

  ##################################################################
  # PORTAL ADMINISTRATOR
  ##################################################################

  @api @wip
  Scenario: Add link on main menu
    Given I am logged in as a user with the "portal administrator" role
    And I am on "Main menu" page
    When I press "Add link"
    And I fill in "Menu link title" with "Google"
    And I fill in "path" with "http://www.google.com"
    And I press "Save"
    Then I should see "Your configuration has been saved"
    And I should see "Google" in the "main menu" region

  @api @wip
  Scenario: Remove link from main menu
    Given I am logged in as a user with the "portal administrator" role
    And I am on "Main menu" page
    When I click "delete" in the "About" row
    And I press "Confirm"
    Then I should see "The menu link About has been deleted"

  @api @wip
  Scenario: Edit pages
    Given I am logged in as a user with the "portal administrator" role
    And I am on the homepage
    When I follow "about"
    Then I should see "Edit"
    When I press "Edit"
    And I fill in "description" with "Lorem ipsum."
    And I press "Save"
    Then I should see "Page about has been updated"
    When I am on "about" page
    Then I should see "Lorem ipsum." in the "content" region
