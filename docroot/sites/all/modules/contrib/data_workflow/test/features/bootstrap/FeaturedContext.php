<?php

use Drupal\DrupalExtension\Context\DrupalContext;
use Behat\Behat\Context\Step\Given;
use Behat\Behat\Context\Step\Then;
use Symfony\Component\Process\Process;
use Behat\Gherkin\Node\TableNode;
use Behat\Mink\Driver\Selenium2Driver;
// use Cocur\Slugify\Slugify;

require 'vendor/autoload.php';

class FeatureContext extends DrupalContext
{

  /**
   * @Given /^groups memberships:$/
   */
  public function groupsMemberships(TableNode $table) {
    $memberships = $table->getHash();
    foreach ($memberships as $membership) {
      // Find group node.
      $group_node = $membership['group'];
      foreach($this->nodes as $node) {
        if($node->type == 'group' && $node->title == $group_node) {
          $group_node = $node;
        }
      }

      // Subscribe nodes and users to group.
      if (isset($membership['members'])) {
        $members = explode(",", $membership['members']);
        foreach ($this->users as $user) {
          if (in_array($user->name, $members)) {
            og_group(
              'node',
              $group_node->nid,
              array(
                'entity' => $user,
                'entity_type' => 'user',
                "membership type"   => OG_MEMBERSHIP_TYPE_DEFAULT,
              )
            );
            // Patch till i figure out why rules are not firing.
            if ($user->name == 'editor') {
              $group_roles = db_select('og_role', 'ogr')
                ->fields('ogr', array('rid', 'name'))
                ->condition('group_type', 'node', '=')
                ->condition('name', 'content editor', '=')
                ->execute()
                ->fetchAllkeyed();
              $group_roles = array_keys($group_roles);
              if (count($group_roles)) {
                og_role_grant('node', $group_node->nid, $user->uid, $group_roles[0]);  
              }
            }
          }
        }
      }
      
      if (isset($membership['nodes'])) {
        $content = explode(",", $membership['nodes']); 
        foreach ($this->nodes as $node) {
          if ($node->type != 'group' && in_array($node->title, $content)) {
            og_group(
              'node',
              $group_node->nid,
              array(
                'entity' => $node,
                'entity_type' => 'node',
                'state' => OG_STATE_ACTIVE,
              )
            );
          }
        }
      }
    }
  }

  /**
   * @Given /^I wait for "([^"]*)" seconds$/
   */
  public function iWaitForSeconds($seconds) {
    $session = $this->getSession();
    $session->wait($seconds * 1000); 
  }

  /**
   * @Then /^I should see the administration menu$/
   */
  public function iShouldSeeTheAdministrationMenu() {
    $xpath = "//div[@id='admin-menu']";
    // grab the element
    $element = $this->getXPathElement($xpath);
  }

  /**
   * Returns an element from an xpath string
   * @param  string $xpath
   *   String representing the xpath
   * @return object
   *   A mink html element
   */
  protected function getXPathElement($xpath) {
    // get the mink session
    $session = $this->getSession();
    // runs the actual query and returns the element
    $element = $session->getPage()->find(
      'xpath',
      $session->getSelectorsHandler()->selectorToXpath('xpath', $xpath)
    );
    // errors must not pass silently
    if (null === $element) {
      throw new \InvalidArgumentException(sprintf('Could not evaluate XPath: "%s"', $xpath));
    }
    return $element;
  }

  /**
   * Determine if the a user is already logged in.
   */
  public function loggedIn() {
    $session = $this->getSession();
    $session->visit($this->locatePath('/'));
    $driver = $this->getSession()->getDriver();
    if ($driver instanceof Selenium2Driver) {
      $session->wait(2000);
    }
    // If a logout link is found, we are logged in. While not perfect, this is
    // how Drupal SimpleTests currently work as well.
    $element = $session->getPage();
    return $element->findLink($this->getDrupalText('log_out'));
  }

  /**
   * Sets php error reporting to just errors.
   *
   * @BeforeScenario
   */
  public function setErrorReportingBeforeScenario($event) {
    error_reporting(E_ERROR | E_PARSE);
  }

  /**
   * Take screenshot when step fails.
   * Works only with Selenium2Driver.
   * 
   * @AfterStep
   */
  public function takeScreenshotAfterStep($event)
  {
    if (4 === $event->getResult()) {
      $driver = $this->getSession()->getDriver();
      if (!($driver instanceof Selenium2Driver)) {
        // throw new UnsupportedDriverActionException('Taking screenshots is not supported by %s, use Selenium2Driver instead.', $driver);
        return;
      }
      $screenshot = $driver->getScreenshot();
      // $slugify = new Slugify();
      $file = 'screens/' . time() . ' ' . $event->getLogicalParent()->getTitle();
      // $file = $slugify->slugify($file) . '.png';
      $file = $file . '.png';
      file_put_contents($file, $screenshot);
    }
  }
}
