
Feature: Workbench

Background:
  Given pages:
    | title        | url                          |
    | Datasets     | dataset                      |
    | Needs Review | admin/workbench/needs-review |
    | My drafts    | admin/workbench/drafts       |
  Given users:
    | name    | mail             | roles                |
    | Katie   | katie@test.com   | data contributor     |
    | Celeste | celeste@test.com | data contributor     |
    | Gabriel | gabriel@test.com | content editor       |
    | Jaz     | jaz@test.com     | data contributor     |
  And "Tags" terms:
    | name   |
    | Health |
    | Gov    |
  And datasets:
    | title      | author  | moderation   | date         | tags   |
    | Dataset 01 | Gabriel | published    | Feb 01, 2015 | Health |
    | Dataset 02 | Gabriel | published    | Mar 13, 2015 | Gov    |
    | Dataset 03 | Katie   | published    | Feb 17, 2013 | Health |
    | Dataset 04 | Celeste | draft        | Jun 21, 2015 | Gov    |
    | Dataset 05 | Katie   | needs_review | Jun 21, 2015 | Gov    |
  And "Format" terms:
    | name |
    | csv  |
  And resources:
    | title       | dataset    | moderation | format |
    | Resource 01 | Dataset 01 | published  | csv    |
    | Resource 02 | Dataset 01 | published  | csv    |
    | Resource 03 | Dataset 02 | published  | csv    |

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
    And I should see "Needs review"

  @api
  Scenario: View 'Stale drafts' menu item for "portal administrator" role
    Given I am logged in as a user with the "portal administrator" role
    Then I should see the link "My Workbench" in the navigation region
    When I follow "My Workbench"
    Then I should see "Stale drafts"

  @api
  Scenario: View 'Stale reviews' menu item for "portal administrator" role
    Given I am logged in as a user with the "portal administrator" role
    Then I should see the link "My Workbench" in the navigation region
    When I follow "My Workbench"
    Then I should see "Stale reviews"

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
    And I am on "Dataset 01" page
    When I follow "Moderate"
    Then I should see "Published" in the "#edit-state" element
    When I press "Apply"
    Then I should see "Needs Review --> Published"
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

  ##################################################################
  # EMAIL NOTIFICATION
  ##################################################################

  @api @mail
  Scenario: As a Content Editor I want to receive an email notification when "Data Contributor" add a Dataset that "Needs Review".
    Given I am logged in as "Katie"
    And I am on "Datasets" page
    When I click "Add Dataset"
    And I fill in the following:
      | Title                     | Dataset That Needs Review |
      | Description               | Test Behat Dataset 06     |
      | autocomplete-deluxe-input | Health                    |
    And I press "Next: Add data"
    And I fill in the following:
      | Title                     | Resource 061            |
      | Description               | Test Behat Resource 061 |
      | autocomplete-deluxe-input | CSV                     |
    And I press "Save"
    Then I should see the success message "Resource Resource 061 has been created."
    And I click "Back to dataset"
    Then I follow "Moderate"
    Then I should see "Needs Review" in the "#edit-state" element
    And I should not see "Published" in the "#edit-state" element
    And I press "Apply"
    And I should see "Draft --> Needs Review"
    And user Gabriel should receive an email containing "Please review the recent update at"

  @api @mail
  Scenario: Request dataset review (Change dataset status from 'Draft' to 'Needs review')
    Given I am logged in as "Celeste"
    And I am on "My drafts" page
    And I should see "Change to Needs Review" in the "Dataset 04" row
    When I click "Change to Needs Review" in the "Dataset 04" row
    Then I should see "Needs Review" in the "Dataset 04" row
    And user Gabriel should receive an email containing "Please review the recent update at"
