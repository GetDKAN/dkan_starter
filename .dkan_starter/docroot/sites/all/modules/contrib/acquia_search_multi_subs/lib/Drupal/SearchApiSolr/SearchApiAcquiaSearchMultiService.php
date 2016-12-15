<?php

/**
 * @file
 * Contains SearchApiAcquiaSearchMultiService.
 */

/**
 * Provides automatic environment switching for Acquia Search servers.
 */
class SearchApiAcquiaSearchMultiService extends SearchApiAcquiaSearchService {

  /**
   * {@inheritdoc}
   */
  public function connect() {
    if (!$this->solr) {
      // Set our special overrides, if applicable.
      $this->setConnectionOptions();
      parent::connect();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function setConnectionOptions() {
    $has_id = (isset($this->options['acquia_override_subscription']['acquia_override_subscription_id'])) ? true : false;
    $has_key = (isset($this->options['acquia_override_subscription']['acquia_override_subscription_key'])) ? true : false;
    $has_corename = (isset($this->options['acquia_override_subscription']['acquia_override_subscription_corename'])) ? true : false;

    if ($has_id && $has_key && $has_corename) {
      $identifier = $this->options['acquia_override_subscription']['acquia_override_subscription_id'];
      $key = $this->options['acquia_override_subscription']['acquia_override_subscription_key'];
      $corename = $this->options['acquia_override_subscription']['acquia_override_subscription_corename'];

      $this->options['path'] = '/solr/' . $corename;
      // Set the derived key for this environment.
      $subscription = $this->getAcquiaSubscription($identifier, $key);

      if (!$this->getAcquiaSubscriptionError($subscription, $identifier)) {
        $derived_key_salt = $subscription['derived_key_salt'];
        $derived_key = _acquia_search_multi_subs_create_derived_key($derived_key_salt, $corename, $key);
        $this->options['derived_key'] = $derived_key;

        // Get and set our search core hostname.
        $search_host = acquia_search_multi_subs_get_hostname($corename);
        $this->options['host'] = $search_host;
      }
    }
    else {
      parent::setConnectionOptions();
    }
  }

  /**
   * Overrides SearchApiAcquiaSearchService::configurationForm().
   *
   * Adds configuration for switching the Solr server, either automatically
   * based on the environment or manually.
   *
   * @see acquia_search_multi_subs_get_settings_form()
   */
  public function configurationForm(array $form, array &$form_state) {
    $form = parent::configurationForm($form, $form_state);

    // Only allow overriding of the connection information with our form.
    $form['modify_acquia_connection']['#access'] = FALSE;
    $form['modify_acquia_connection']['#default_value'] = FALSE;
    $form['host']['#access'] = FALSE;
    $form['path']['#access'] = FALSE;

    // Get our common settings form.
    $configuration = isset($this->options['acquia_override_subscription']) ? $this->options['acquia_override_subscription'] : array();
    acquia_search_multi_subs_get_settings_form($form, $form_state, $configuration);
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function viewSettings() {
    // If Search API 1.10+ is used, this method is deprecated in favor of
    // getExtraInformation().
    if (method_exists('SearchApiAbstractService', 'getExtraInformation')) {
      return NULL;
    }
    $output = parent::viewSettings();

    // Set our special overrides, if applicable.
    $this->setConnectionOptions();

    $options = $this->options;
    $auto_detection = (isset($options['acquia_override_subscription']['acquia_override_auto_switch']) && $options['acquia_override_subscription']['acquia_override_auto_switch']);
    $auto_detection_state = ($auto_detection) ? t('enabled') : t('disabled');
    $output .= "<dl>\n  <dt>";
    $output .= t('Acquia Search Auto Detection');
    $output .= "</dt>\n  <dd>";
    $output .= t('Auto detection of your environment is <strong>@state</strong>', array('@state' => $auto_detection_state));
    $output .= '</dd>';
    $output .= "\n</dl>";

    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function getExtraInformation() {
    $auto_detection = (!isset($this->options['acquia_override_subscription']['acquia_override_auto_switch']) || $this->options['acquia_override_subscription']['acquia_override_auto_switch']);
    $auto_detection_state = ($auto_detection) ? t('enabled') : t('disabled');
    $info[] = array(
      'label' => t('Acquia Search Auto Detection'),
      'info' => t('Auto detection of your environment is <strong>@state</strong>.', array('@state' => $auto_detection_state)),
    );

    $this->setConnectionOptions();
    $info = array_merge($info, parent::getExtraInformation());
    return $info;
  }

  /**
   * Overrides SearchApiSolrService::configurationFormValidate().
   *
   * Verifies the subscription if the user has specified an subscription_id and
   * a subscription_key by switching off auto-selection of core-name and
   * choosing the "other" option in the list of cores-names.
   */
  public function configurationFormValidate(array $form, array &$values, array &$form_state) {
    parent::configurationFormValidate($form, $values, $form_state);

    $has_id = (isset($values['acquia_override_subscription']['acquia_override_subscription_id'])) ? true : false;
    $has_key = (isset($values['acquia_override_subscription']['acquia_override_subscription_key'])) ? true : false;
    $has_corename = (isset($values['acquia_override_subscription']['acquia_override_subscription_corename'])) ? true : false;
    $has_auto_switch = !empty($values['acquia_override_subscription']['acquia_override_auto_switch']) ? true : false;
    if (!$has_auto_switch && $has_id && $has_key && $has_corename) {
      $identifier = $values['acquia_override_subscription']['acquia_override_subscription_id'];
      $key = $values['acquia_override_subscription']['acquia_override_subscription_key'];

      // Make sure that we'll have cached subscription in submit.
      $subscription = $this->getAcquiaSubscription($identifier, $key);
      if ($error_message = $this->getAcquiaSubscriptionError($subscription, $identifier, TRUE)) {
        // Error message already displayed by the getAcquiaSubscription.
        form_set_error('options][form][acquia_override_subscription][acquia_override_subscription_key');
        form_set_error('options][form][acquia_override_subscription][acquia_override_subscription_id');
      }
    }
  }

  /**
   * Overrides SearchApiSolrService::configurationFormSubmit().
   *
   * If auto detection is not on, changes our search core name to the one that
   * was inputted.
   */
  public function configurationFormSubmit(array $form, array &$values, array &$form_state) {
    parent::configurationFormSubmit($form, $values, $form_state);

    // If we do not have auto switch enabled, statically configure the right
    // core to options.
    $has_id = (isset($values['acquia_override_subscription']['acquia_override_subscription_id'])) ? true : false;
    $has_key = (isset($values['acquia_override_subscription']['acquia_override_subscription_key'])) ? true : false;
    $has_corename = (isset($values['acquia_override_subscription']['acquia_override_subscription_corename'])) ? true : false;
    $has_auto_switch = !empty($values['acquia_override_subscription']['acquia_override_auto_switch']) ? true : false;

    if (!$has_auto_switch && $has_id && $has_key && $has_corename) {
      $identifier = $values['acquia_override_subscription']['acquia_override_subscription_id'];
      $key = $values['acquia_override_subscription']['acquia_override_subscription_key'];
      $corename = $values['acquia_override_subscription']['acquia_override_subscription_corename'];

      // Set our solr path
      $this->options['path'] = '/solr/' . $corename;

      // Set the derived key for this environment.
      // Subscription already cached by configurationFormValidate().
      $subscription = $this->getAcquiaSubscription($identifier, $key);
      $derived_key_salt = $subscription['derived_key_salt'];
      $derived_key = _acquia_search_multi_subs_create_derived_key($derived_key_salt, $corename, $key);
      $this->options['derived_key'] = $derived_key;

      $search_host = acquia_search_multi_subs_get_hostname($corename);
      $this->options['host'] = $search_host;
    }
  }

  /**
   * Check Acquia subscription data.
   *
   * @param $subscription
   *   FALSE, integer (xmlrpc error number), or subscription data array.
   *   @see acquia_agent_get_subscription()
   * @param string $identifier
   * @param bool $quiet
   *   If FALSE display or log error message.
   *
   * @return bool|string
   *   FALSE if subscription is valid or translated error message otherwise.
   */
  protected function getAcquiaSubscriptionError($subscription, $identifier, $quiet = FALSE) {
    if (!is_array($subscription)) {
      $t_args = array('%subscription' => $identifier);
      $error_message = t('Unable to get %subscription subscription data. Please try later.', $t_args);
      if (is_numeric($subscription)) {
        switch ($subscription) {
          case SUBSCRIPTION_NOT_FOUND:
            $error_message = t('Your %subscription subscription not found.', $t_args);
            break;

          case SUBSCRIPTION_EXPIRED:
            $error_message = t('Your %subscription subscription expired.', $t_args);
            break;
        }
      }
      elseif ($subscription === FALSE) {
        // Occurs when response validation failed.
        $error_message = t('Acquia subscription response validation error. Please check your <a href="!url">Acquia Subscription Settings</a> settings and try again.',
          array('!url' => url('admin/config/system/acquia-agent')));
      }

      // Log and display error message (if the user has access).
      if (!$quiet) {
        if (user_access('administer search_api')) {
          drupal_set_message($error_message, 'warning', FALSE);
        }
        watchdog('acquia_search_multi_subs', $error_message, array(), WATCHDOG_WARNING);
      }

      return $error_message;
    }

    return FALSE;
  }

  /**
   * Get subscription info from the acquia_connector module, and cache it for
   * 6 hours.
   *
   * @param $acquia_identifier
   * @param $acquia_key
   * @return FALSE, integer (xmlrpc error number), or subscription data
   *   @see acquia_agent_get_subscription()
   */
  protected function getAcquiaSubscription($acquia_identifier, $acquia_key) {
    $subscription = FALSE;
    $subscription_cache = &drupal_static(__FUNCTION__, array());
    // Get subscription and use cache.
    $cid = 'asms-subscription-' . $acquia_identifier . ':' . $acquia_key;
    if (isset($subscription_cache[$cid])) {
      $subscription = $subscription_cache[$cid];
    }
    else {
      $cached = cache_get($cid);
      if ($cached && $cached->data && REQUEST_TIME < $cached->expire && is_array($cached->data)) {
        $subscription = $cached->data;
        $subscription_cache[$cid] = $subscription;
      }
    }

    // Get subscription from Acquia.
    if (empty($subscription) || !is_array($subscription)) {
      // We do not want to send a heartbeat to the server, we only need
      // subscription data.
      $subscription = acquia_agent_get_subscription(array('no_heartbeat' => 1), $acquia_identifier, $acquia_key);
      // Don't cache non-subscription data: XML-RPC client error, etc.
      if (!$this->getAcquiaSubscriptionError($subscription, $acquia_identifier)) {
        $subscription_cache[$cid] = $subscription;
        // Cache this for 6 hours.
        cache_set($cid, $subscription, 'cache', REQUEST_TIME + variable_get('acquia_search_multi_subs_subscription_cache_time', 6 * 60 * 60));
      }
    }

    return $subscription;
  }
}
