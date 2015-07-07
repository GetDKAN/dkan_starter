
Feature: Workbench

  Background:
    Given users:
      | name                    | mail                              | roles                     |
      | datacontributor1        | datacontributor1@test.com         | data contributor          |
      | contenteditor1          | contenteditor1@test.com           | content editor            |
      | portaladministrator1    | portaladministrator1@test.com     | portal administrator      |
      | name    				| mail             					| roles               	 	|
      | Katie  					| katie@test.com   					| data contributor     		|
      | Celeste 				| celeste@test.com 					| data contributor     		|
      | Gabriel                 | gabriel@test.com 					| content editor       		|
      | Jaz        				| jaz@test.com     					| data contributor     		|
    And "tags" terms:
      | name   |
      | Health |
	  | Gov    |
    And datasets:
      | title      | author                 | moderation    | date         | tags   |
      | Dataset 01 | datacontributor1       | draft         | Feb 01, 2015 | Health |
      | Dataset 02 | Gabriel 				| published    	| Mar 13, 2015 | Gov    |
      | Dataset 03 | Katie   				| published    	| Feb 17, 2013 | Health |
      | Dataset 04 | Celeste 				| draft        	| Jun 21, 2015 | Gov    |
      | Dataset 05 | Katie   				| needs_review 	| Jun 21, 2015 | Gov    |
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
    Given users:
      | name                    | mail                              | roles                     |
      | datacontributor1        | datacontributor1@test.com         | data contributor          |
      | contenteditor1          | contenteditor1@test.com           | content editor            |
      | portaladministrator1    | portaladministrator1@test.com     | portal administrator      |
    And "tags" terms:
      | name   |
      | Health |
    And datasets:
      | title      | author                 | moderation            | date         | tags   |
      | Dataset 01 | datacontributor1       | needs_review          | Feb 01, 2015 | Health |
    And "Format" terms:
      | name  |
      | csv   |
    And resources:
      | title       | format | dataset    |
      | Resource 01 | csv    | Dataset 01 |
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
    Given users:
      | name                    | mail                              | roles                     |
      | datacontributor1        | datacontributor1@test.com         | data contributor          |
      | contenteditor1          | contenteditor1@test.com           | content editor            |
      | portaladministrator1    | portaladministrator1@test.com     | portal administrator      |
    And "tags" terms:
      | name   |
      | Health |
    And datasets:
      | title      | author                 | moderation    | date         | tags   |
      | Dataset 01 | datacontributor1       | draft         | Feb 01, 2015 | Health |
    And "Format" terms:
      | name  |
      | csv   |
    And resources:
      | title       | format | dataset    |
      | Resource 01 | csv    | Dataset 01 |
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
