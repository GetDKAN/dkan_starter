Feature: Testing portal administrator role and permissions

  @api @javascript
  Scenario: Add Editor to group and test for og role assignment rule
    Given users:
      | name         | mail            | status | roles              |
      | editor       | editor@test.com | 1      | 254633039          |
      | portal admin | admin@test.com  | 1      | 27274083,161863194 |
      And "group" nodes:
      | title      | status | body       | group_group |
      | test group | 1      | Test Group | 1           |
    When I am logged in as "portal admin"
      And I am on "/group/test-group"
    When I click "Group" in the "toolbar" region
      And I click "Add people"
      And I wait for "3" seconds
    When I fill in "name" with "editor"
      And I wait for "3" seconds
      And I press "edit-submit"
      And I wait for "3" seconds
    Then I should see "editor has been added to the group"
    When I am on "/group/test-group"
      And I click "Group" in the "toolbar" region
      And I click "People"
    Then I should see "content editor"

  @api @javascript
  Scenario: Remove Editor from group
    Given users:
      | name         | mail            | status | roles              |
      | editor       | editor@test.com | 1      | 254633039          |
      | portal admin | admin@test.com  | 1      | 27274083,161863194 |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |
      And groups memberships:
        | group      | members |
        | test group | editor  |
      When I am logged in as "portal admin"
        And I am on "/group/test-group"
      When I click "Group" in the "toolbar" region
        And I click "People"
        And I check the box "edit-views-bulk-operations-0"
        And I click "remove"
        And I press "Remove"
      Then I should see "The membership was removed."
        And I should not see "editor"

  @api
  Scenario: Create group
    Given users:
      | name         | mail           | status | roles              |
      | portal admin | admin@test.com | 1      | 27274083,161863194 |
      And I am logged in as "portal admin"
      And I am on "/node/add/group"
    When I fill in "title" with "Test Group"
      And I fill in "body[und][0][value]" with "Test Group Body"
      And I press "Save"
    Then I should see "Test Group has been created"

  @api
  Scenario: Add an User
    Given users:
      | name         | mail           | status | roles              |
      | portal admin | admin@test.com | 1      | 27274083,161863194 |
      And I am logged in as "portal admin"
      And I am on "/admin/people/create"
    When I fill in "edit-name" with "new content editor"
      And I fill in "edit-mail" with "newcontenteditor@editor.com"
      And I fill in "edit-pass-pass1" with "HM1092.secure_policy"
      And I fill in "edit-pass-pass2" with "HM1092.secure_policy"
      And I press "Create new account"
    Then I should see "Created a new user account for new content editor."

  @api @javascript
  Scenario: Remove an User
    Given users:
      | name         | mail           | status | roles                        |
      | portal admin | admin@test.com | 1      | 27274083,161863194,161863194 |
      And I am logged in as "portal admin"
      And I am on "/users/new-content-editor"
    When I click "Edit" in the "toolbar" region
      And I press "Cancel account"
      And I check the box "edit-user-cancel-method--4"
      And I press "Cancel account"
    Then I should see "new content editor has been deleted."

  @api @javascript
  Scenario: Give an authenticated user the data contributor role
    Given users:
      | name          | mail                   | status | roles              |
      | authenticated | authenticated@test.com | 1      | 2                  |
      | portal admin  | admin@test.com         | 1      | 27274083,161863194 |
      And I am logged in as "portal admin"
      And I am on "/users/authenticated"
    When I click "Edit" in the "toolbar" region
      And I check the box "edit-roles-change-226931607"
      And I press "Save"
    Then I should see "The changes have been saved."
    
  @api @javascript
  Scenario: Give a data contributor the content editor role
    Given users:
      | name             | mail                      | status | roles              |
      | data contributor | data_contributor@test.com | 1      | 226931607          |
      | portal admin     | admin@test.com            | 1      | 27274083,161863194 |
      And I am logged in as "portal admin"
      And I am on "/users/data-contributor"
    When I click "Edit" in the "toolbar" region
      And I check the box "edit-roles-change-254633039"
      And I uncheck the box "edit-roles-change-226931607"
      And I press "Save"
    Then I should see "The changes have been saved."
      
  @api @javascript
  Scenario: Move a content editor to the data contributor role
    Given users:
      | name         | mail            | status | roles              |
      | editor       | editor@test.com | 1      | 226931607          |
      | portal admin | admin@test.com  | 1      | 27274083,161863194 |
      And I am logged in as "portal admin"
      And I am on "/users/editor"
    When I click "Edit" in the "toolbar" region
      And I uncheck the box "edit-roles-change-254633039"
      And I check the box "edit-roles-change-226931607"
      And I press "Save"
    Then I should see "The changes have been saved."

  @api
  Scenario: Add a menu link
    Given users:
      | name         | mail           | status | roles              |
      | portal admin | admin@test.com | 1      | 27274083,161863194 |
      And I am logged in as "portal admin"
      And I am on "/admin/structure/menu"
    When I click "add link"
      And I fill in "edit-link-title" with "Testing Menu Link to Home page"
      And I fill in "edit-link-path" with "<front>"
      And I press "Save"
    Then I should see "Your configuration has been saved."

  @api @javascript
  Scenario: Can delete past revisions
    Given users:
      | name             | mail                      | status | roles              |
      | data_contributor | data_contributor@test.com | 1      | 226931607          |
      | editor           | editor@test.com           | 1      | 226931607          |
      | portal admin     | admin@test.com            | 1      | 27274083,161863194 |
      And "dataset" nodes:
        | title | author | status |
        | test  | author | 1      |
      And "group" nodes:
        | title      | status | body       | group_group |
        | test group | 1      | Test Group | 1           |
      And groups memberships:
        | group      | members       | nodes |
        | test group | author,editor | test  |
      And I am logged in as "portal admin"
      And I am on "/dataset/test"
      And I click "Edit" in the "toolbar" region
      And I press "Finish"
      And I am on "/dataset/test"
    When I click "Revisions" in the "toolbar" region
      And I click "Delete"
      And I press "Delete"
    Then  I should see "Revision from"
      And I should see "has been deleted"
