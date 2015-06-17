Feature: Admin Blog

  Background:
    Given users:
      | username  | role on site         |
      | admin     | portal administrator |
      | john      | portal administrator |
    Given blog_posts:
      | title   | description | user  |
      | Post 01 | Lorem ipsum | admin |
      | Post 02 | Lorem ipsum | john  |

  @api @wip
  Scenario: Create blog entry
    Given I am logged in as "admin"
    And I am on the homepage
    When I follow "Blog"
    Then I should see "Create new blog entry"
    When I press "Create new blog entry"
    And I fill in the "post" form for "New Post"
    And I press "Save"
    Then I should see "Blog entry New Post has been created"