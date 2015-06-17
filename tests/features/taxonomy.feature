
Feature: Taxonomy

  Background:
    Given users:
      | username  | role on site         |
      | admin     | portal administrator |
    Given vocabularies:
      | name          |
      | Vocabulary 01 |
    Given terms:
      | name    | vocabulary    |
      | Term 01 | Vocabulary 01 |
      | Term 02 | Vocabulary 01 |

  ##################################################################
  # PORTAL ADMINISTRATOR
  ##################################################################

  @api @wip
  Scenario: Add vocabulary
    Given I am logged in as "admin"
    And I am on "/admin/structure/taxonomy" page
    When I follow "Add vocabulary"
    And I fill in the "vocabulary" form for "New Vocabulary"
    And I press "Save"
    Then I should see "Created new vocabulary New Vocabulary"
    And I should see "New Vocabulary" in the "vocabularies" region

  @api @wip
  Scenario: Edit vocabulary
    Given I am logged in as "admin"
    And I am on "/admin/structure/taxonomy" page
    When I click "edit vocabulary" in the "Vocabulary 01" row
    And I fill in "name" with "Vocabulary 01 edited"
    And I press "Save"
    Then I should see "Updated vocabulary Vocabulary 01 edited"
    And I should see "Vocabulary 01 edited" in the "vocabularies" region

  @api @wip
  Scenario: View the list of terms in a vocabulary
    Given I am logged in as "admin"
    And I am on "/admin/structure/taxonomy" page
    When I click "list terms" in the "Vocabulary 01" row
    Then I should see "2" items in the "terms" region

  @api @wip
  Scenario: Add vocabulary item
    Given I am logged in as "admin"
    And I am on "/admin/structure/taxonomy" page
    When I click "add terms" in the "Vocabulary 01" row
    And I fill in the "term" form for "New term"
    And I press "Save"
    Then I should see "Created new term New term"

  @api @wip
  Scenario: Edit vocabulary item
    Given I am logged in as "admin"
    And I am on "/admin/structure/taxonomy" page
    And I click "list terms" in the "Vocabulary 01" row
    When I click "edit" in the "Term 01" row
    And I fill in "name" with "Term 01 edited"
    And I press "Save"
    Then I should see "Updated term Term 01 edited"
    And I should see "Term 01 edited" in the "terms" region

  @api @wip
  Scenario: Remove vocabulary item
    Given I am logged in as "admin"
    And I am on "/admin/structure/taxonomy" page
    And I click "list terms" in the "Vocabulary 01" row
    And I click "edit" in the "Term 01" row
    When I press "Delete"
    Then I should see "This action cannot be undone."
    And I press "Delete"
    Then I should see "Deleted term Term 01"
    And I should not see "Term 01" in the "terms" region