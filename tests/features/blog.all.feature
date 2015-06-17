
Feature: Blog

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
  Scenario: View the list of published blog posts
    Given I am on the homepage
    When I follow "Blog"
    Then I should see "2" items in the "posts" region

  @api @wip
  Scenario: View the list of blog posts associated with an user
    Given I am on the homepage
    When I follow "Blog"
    And I click "john" in the "Post 02" row
    Then I should see "1" items in the "posts" region

  @api @wip
  Scenario: View published blog post
    Given I am on the homepage
    And I follow "Blog"
    When I click "Post 01" in the "posts" region
    Then I should see the "Post 01" detail page