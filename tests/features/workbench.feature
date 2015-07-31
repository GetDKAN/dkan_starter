Feature: Workbench

  Background:
    Given pages:
      | title        | url                          |
      | Datasets     | dataset                      |
      | Needs Review | admin/workbench/needs-review |
      | My drafts    | admin/workbench/drafts       |
    Given users:
      | name                    | mail                              | roles                     |
      | datacontributor1        | datacontributor1@test.com         | data contributor          |
      | contenteditor1          | contenteditor1@test.com           | content editor            |
      | portaladministrator1    | portaladministrator1@test.com     | portal administrator      |
      | Katie                   | katie@test.com                    | data contributor          |
      | Celeste                 | celeste@test.com                  | data contributor          |
      | Gabriel                 | gabriel@test.com                  | content editor            |
      | Jaz                     | jaz@test.com                      | data contributor          |
    And "tags" terms:
      | name   |
      | Health |
      | Gov    |
    And datasets:
      | title      | author                 | moderation    | date         | tags   |
      | Dataset 01 | datacontributor1       | draft         | Feb 01, 2015 | Health |
      | Dataset 02 | Gabriel                | published     | Mar 13, 2015 | Gov    |
      | Dataset 03 | Katie                  | published     | Feb 17, 2013 | Health |
      | Dataset 04 | Celeste                | draft         | Jun 21, 2015 | Gov    |
      | Dataset 05 | Katie                  | needs_review  | Jun 21, 2015 | Gov    |
    And "Format" terms:
      | name  |
      | csv   |
    And resources:
    | title       | dataset    | moderation | format |
    | Resource 01 | Dataset 01 | published  | csv    |
    | Resource 02 | Dataset 01 | published  | csv    |
    | Resource 03 | Dataset 02 | published  | csv    |

  @api
  Scenario: As a Data Contributor I want to moderate my own Datasets
    Given I am logged in as "datacontributor1"
    And I am on "Dataset 01" page
    When I follow "Moderate"
    Then I should see "Needs Review" in the "#edit-state" element
    And I should not see "Published" in the "#edit-state" element
    And I press "Apply"
    And I should see "Draft --> Needs Review"

  @api
  Scenario: As a Content Editor I want to Publish datasets posted by a Data Contributor
    Given I am logged in as "contenteditor1"
    And I am on "Dataset 01" page
    When I follow "Moderate"
    Then I should see "Needs Review" in the "#edit-state" element
    When I press "Apply"
    Then I should see "Draft --> Needs Review"
    And I should see "Published" in the "#edit-state" element
    When I press "Apply"
    Then I should see "Needs Review --> Published"
    Given I am an anonymous user
    And I am on "Dataset 01" page
    Given I should not see the error message "Access denied. You must log in to view this page."

  @api
  Scenario: As a Portal Administrator I want to moderate all content
    Given I am logged in as "portaladministrator1"
    And I am on "Dataset 01" page
    When I follow "Moderate"
    Then I should see "Needs Review" in the "#edit-state" element
    And I should see "Published" in the "#edit-state" element
    When I follow "Edit draft"
    And I fill in "Description" with "Dataset 01 edited"
    And I press "Finish"
    Then I should see "Dataset Dataset 01 has been updated"
    Given I am an anonymous user
    And I am on "Dataset 01" page
    Given I should not see the error message "Access denied. You must log in to view this page."

  @api
  Scenario Outline: View 'My workbench' page
    Given I am logged in as a user with the "<role name>" role
    Then I should see the link "My Workbench" in the navigation region
    When I follow "My Workbench"
    Then I should see "My Content"
    And I should see "Create content"
    And I should see "My drafts"
    And I should see an ".link-badge" element
    And I should see "Needs review"
    And I should see an ".link-badge" element

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
    And I should see an ".link-badge" element

  @api
  Scenario: View 'Stale reviews' menu item for "portal administrator" role
    Given I am logged in as a user with the "portal administrator" role
    Then I should see the link "My Workbench" in the navigation region
    When I follow "My Workbench"
    Then I should see "Stale reviews"
    And I should see an ".link-badge" element


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
