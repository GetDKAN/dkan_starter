
Feature: Workbench

  # If this is just dataset moderation, move it to the dataset features.
  # TODO: The Workbench functionality is not part of DKAN. Needs definition.

  ##################################################################
  # AUTHENTICATED ( PORTAL ADMINISTRATORS + CONTENT EDITORS + DATA CONTRIBUTORS )
  ##################################################################

  @api
  Scenario Outline: View 'My workbench' page
    Given I am logged in as a user with the "<role name>" role
    Then I should see the link "My Workbench" in the navigation region
    When I follow "My Workbench"
    Then I should see "My Content"
    And I should see "Create content"
    And I should see "My drafts"
    And I should see "Needs review"

  Examples:
    | role name                 |
    | portal administrator      |
    | content editor            |

  @api
  Scenario: View 'My workbench' page for "data contributor" role
    Given I am logged in as a user with the "data contributor" role
    Then I should see the link "My Workbench" in the navigation region
    When I follow "My Workbench"
    Then I should see "My Content"
    And I should see "Create content"
    And I should see "My drafts"
    And I should not see "Needs review"


  @api @wip
  Scenario: View basic profile information
    Given I am on the homepage

  @api @wip
  Scenario: Access to full profile
    Given I am on the homepage

  @api @wip
  Scenario: Access to edit profile page
    Given I am on the homepage

  @api @wip
  Scenario: Access to create content page
    Given I am on the homepage

  ##################################################################
  # DATA CONTRIBUTOR
  ##################################################################

  @api @wip
  Scenario: View my latest N updated contents
    Given I am on the homepage

  @api @wip
  Scenario: View all content created by me
    Given I am on the homepage

  @api @wip
  Scenario: View list of own content with 'Draft' status
    Given I am on the homepage

  @api @wip
  Scenario: View list of own content with 'Needs review' status
    Given I am on the homepage

  @api @wip
  Scenario: Search/filter own content
    Given I am on the homepage

  ##################################################################
  # CONTENT EDITOR
  ##################################################################

  @api @wip
  Scenario: View last N updated contents associated with the groups I belong to
    Given I am on the homepage

  @api @wip
  Scenario: View all content associated with the groups I belong to
    Given I am on the homepage

  @api @wip
  Scenario: View list of content with 'Draft' status associated with the groups I belong to
    Given I am on the homepage

  @api @wip
  Scenario: View list of content with 'Needs review' status associated with the groups I belong to
    Given I am on the homepage

  @api @wip
  Scenario: Search/filter content associated with the groups I belong to
    Given I am on the homepage

  @api
  Scenario: As a Content Editor I want to Publish datasets posted by a Data Contributor
    Given pages:
      | title        | url                          |
      | Datasets     | dataset                      |
      | Needs Review | admin/workbench/needs-review |
    Given users:
      | name    | mail             | roles                |
      | Jaz     | jaz@test.com     | data contributor     |
      | Gabriel | gabriel@test.com | content editor       |
    And "tags" terms:
      | name   |
      | Health |
    And datasets:
      | title      | author  | moderation   | date         | tags   |
      | Dataset 01 | Gabriel | needs_review | Feb 01, 2015 | Health |
    And "Format" terms:
      | name |
      | csv |
    And resources:
      | title       | format | dataset    |
      | Resource 01 | csv    | Dataset 01 |
    Given I am logged in as "Gabriel"
    And I am on "Needs Review" page
    Then I should see "Dataset 01"
    Given I click "Change to Published" in the "Dataset 01" row
    Then I should not see "Dataset 01"
    Given I am on "Dataset 01" page
    Then I should see "Revision state: Published"
    Given I am an anonymous user
    And I am on "Dataset 01" page
    Given I should not see the error message "Access denied. You must log in to view this page."

  @api @wip
  Scenario: Publish multiple content associated with the groups I belong to at the same time
    Given I am on the homepage

  ##################################################################
  # PORTAL ADMINISTRATOR
  ##################################################################

  @api @wip
  Scenario: View last N updated contents
    Given I am on the homepage

  @api @wip
  Scenario: View all content
    Given I am on the homepage

  @api @wip
  Scenario: View list of all content with 'Draft' status
    Given I am on the homepage

  @api @wip
  Scenario: View list of all content with 'Needs review' status
    Given I am on the homepage

  @api @wip
  Scenario: Search/filter content
    Given I am on the homepage

  @api @wip
  Scenario: Publish multiple content at the same time
    Given I am on the homepage

  @api @wip
  Scenario: View a report on drafts that haven't moved to published for more than XX days
    Given I am on the homepage

  @api @wip
  Scenario: View a reports on drafts that haven't moved to published for more than 48 hours
    Given I am on the homepage
