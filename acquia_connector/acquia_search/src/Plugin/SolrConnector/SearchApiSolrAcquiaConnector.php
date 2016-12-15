<?php

namespace Drupal\acquia_search\Plugin\SolrConnector;

use Drupal\Core\Url;
use Drupal\search_api_solr\Annotation\SolrConnector;
use Drupal\search_api_solr\SolrConnector\SolrConnectorPluginBase;
use Drupal\Core\Annotation\Translation;
use Drupal\Core\Form\FormStateInterface;
use Drupal\acquia_search\EventSubscriber\SearchSubscriber;
use Solarium\Client;

/**
 * Class SearchApiSolrAcquiaConnector.
 *
 * @package Drupal\acquia_search\Plugin\SolrConnector
 *
 * @SolrConnector(
 *   id = "solr_acquia_connector",
 *   label = @Translation("Acquia"),
 *   description = @Translation("Index items using an Acquia Apache Solr search server.")
 * )
 */
class SearchApiSolrAcquiaConnector extends SolrConnectorPluginBase {

  protected $eventDispatcher = FALSE;

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    $configuration = parent::defaultConfiguration();
    unset($configuration['host']);
    unset($configuration['port']);
    unset($configuration['path']);
    unset($configuration['core']);
    return $configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form = parent::buildConfigurationForm($form, $form_state);
    unset($form['host']);
    unset($form['port']);
    unset($form['path']);
    unset($form['core']);
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateConfigurationForm(array &$form, FormStateInterface $form_state) {
    // Turn off connection check of parent class.
  }

  /**
   * {@inheritdoc}
   */
  protected function connect() {
    if (!$this->solr) {
      $this->solr = new Client();
      $this->solr->createEndpoint($this->configuration + [
          'key' => 'core',
          'host' => acquia_search_get_search_host(),
          'path' => '/solr/' . \Drupal::config('acquia_connector.settings')->get('identifier'),
          'port' => ($this->configuration['scheme'] == 'https') ? 443 : 80,
        ], TRUE);
      $this->attachServerEndpoint();
      $this->eventDispatcher = $this->solr->getEventDispatcher();
      $plugin = new SearchSubscriber();
      $this->solr->registerPlugin('acquia_solr_search_subscriber', $plugin);
      // Don't use curl.
      $this->solr->setAdapter('Solarium\Core\Client\Adapter\Http');
    }
  }

  /**
   * {@inheritdoc}
   */
  protected function getServerUri() {
    $this->connect();
    return $this->solr->getEndpoint('core')->getBaseUri();
  }

  /**
   * {@inheritdoc}
   */
  public function getCoreLink() {
    return $this->getServerLink();
  }

  /**
   * {@inheritdoc}
   */
  public function viewSettings() {
    $uri = Url::fromUri('http://www.acquia.com/products-services/acquia-search', array('absolute' => TRUE));
    drupal_set_message(t("Search is being provided by the @as.", array('@as' => \Drupal::l(t('Acquia Search'), $uri))));
    return parent::viewSettings();
  }

}
