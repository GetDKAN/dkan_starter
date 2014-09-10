Feature: Testing storyteller role and permissions

  @api
  Scenario: Can see the administration menu
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And I am logged in as "storyteller"
    When I am on the homepage
    Then I should see the administration menu

  @api
  Scenario: Can see administration pages
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And I am logged in as "storyteller"
    When I am on "/admin"
    Then I should see "Content"

  @api
  Scenario: Access content overview
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And I am logged in as "storyteller"
    When I am on "/admin/content"
    Then I should see "District Names"

  @api
  Scenario: Create Blog Content
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And I am logged in as "storyteller"
    When I am on "/node/add/blog"
      And I fill in "edit-title" with "Test Blog Post"
      And I fill in "body[und][0][value]" with "Test description"
      And I press "Save"
    Then I should see "Blog entry Test Blog Post has been created"

  @api
  Scenario: Delete own blog content
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And "blog" nodes:
        | title          | author      | status   |
        | test Blog Post | storyteller | 1        |
      And I am logged in as "storyteller"
    When I am on "admin/content"
      And I click "delete"
      And I press "Delete"
    Then I should see "Blog entry test Blog Post has been deleted"

  @api
  Scenario: Edit own blog content
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And "blog" nodes:
        | title          | author      | status   |
        | test Blog Post | storyteller | 0        |
      And I am logged in as "storyteller"
      And I am on "/admin/content"
    When I click "edit"
      And I fill in "body[und][0][value]" with "Test description Update"
      And I press "Save"
    Then I should see "Blog entry Test Blog Post has been updated"

  @api @javascript
  Scenario: Use text format filtered_html
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And I am logged in as "storyteller"
    When I am on "/node/add/blog"
    Then I should have an "html" text format option 

  @api @javascript
  Scenario: Use text format php_code
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And I am logged in as "storyteller"
    When I am on "/node/add/blog"
    Then I should have an "php_code" text format option 

  @api
  Scenario: View own unpublished content
    Given users:
      | name         | mail                  | status     | roles     |
      | storyteller  | storyteller@test.com  | 1          | 132006037 |
      And "blog" nodes:
        | title          | author      | status   |
        | test Blog Post | storyteller | 0        |
      And I am logged in as "storyteller"
    When I am on "/admin/content"
    Then I should see "test Blog Post"
