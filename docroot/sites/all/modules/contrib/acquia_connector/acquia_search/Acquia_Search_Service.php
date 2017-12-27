<?php

/**
 * Starting point for the Solr API. Represents a Solr server resource and has
 * methods for pinging, adding, deleting, committing, optimizing and searching.
 *
 */
class AcquiaSearchService extends DrupalApacheSolrService {

  /**
   * Modify the url and add headers appropriate to authenticate to Acquia Search.
   *
   * @return
   *  The nonce used in the request.
   */
  protected function prepareRequest(&$url, &$options, $use_data = TRUE) {
    // Add a unique request ID to the URL.
    $id = uniqid();
    if (!stristr($url,'?')) {
      $url .= "?";
    }
    else {
      $url .= "&";
    }
    $url .= 'request_id=' . $id;
    // If we're hosted on Acquia, and have an Acquia request ID, append it to
    // the request so that we map Solr queries to Acquia search requests.
    if (isset($_ENV['HTTP_X_REQUEST_ID'])) {
      $xid = empty($_ENV['HTTP_X_REQUEST_ID']) ? '-' : $_ENV['HTTP_X_REQUEST_ID'];
      $url .= '&x-request-id=' . rawurlencode($xid);
    }
    if ($use_data && isset($options['data'])) {
      list($cookie, $nonce) = acquia_search_auth_cookie($url, $options['data'], NULL, $this->env_id);
    }
    else {
      list($cookie, $nonce) = acquia_search_auth_cookie($url, NULL, NULL, $this->env_id);
    }
    if (empty($cookie)) {
      throw new Exception('Invalid authentication string - subscription keys expired or missing.');
    }
    $options['headers']['Cookie'] = $cookie;
    $options['headers'] += array('User-Agent' => 'acquia_search/'. variable_get('acquia_search_version', '7.x'));
    $options['context'] = acquia_agent_stream_context_create($url, 'acquia_search');
    if (!$options['context']) {
      throw new Exception(t("Could not create stream context"));
    }
    return $nonce;
  }

  /**
   * Validate the hmac for the response body.
   *
   * @return
   *  The response object.
   */
  protected function authenticateResponse($response, $nonce, $url) {
    $hmac = acquia_search_extract_hmac($response->headers);
    if (!acquia_search_valid_response($hmac, $nonce, $response->data, NULL, $this->env_id)) {
      throw new Exception('Authentication of search content failed url: '. $url);
    }
    return $response;
  }

  /**
   * Make a request to a servlet (a path) that's not a standard path.
   *
   * @override
   */
  public function makeServletRequest($servlet, $params = array(), $options = array()) {
    // Add default params.
    $params += array(
      'wt' => 'json',
    );

    $url = $this->_constructUrl($servlet, $params);
    // We assume we only authenticate the URL for other servlets.
    $nonce = $this->prepareRequest($url, $options, FALSE);
    $response = $this->_makeHttpRequest($url, $options);
    $response = $this->checkResponse($response);
    return $this->authenticateResponse($response, $nonce, $url);
  }

  /**
   * Central method for making a GET operation against this Solr Server
   *
   * @override
   */
  protected function _sendRawGet($url, $options = array()) {
    $nonce = $this->prepareRequest($url, $options);
    $response = $this->_makeHttpRequest($url, $options);
    $response = $this->checkResponse($response);
    return $this->authenticateResponse($response, $nonce, $url);
  }

  /**
   * Central method for making a POST operation against this Solr Server
   *
   * @override
   */
  protected function _sendRawPost($url, $options = array()) {
    $options['method'] = 'POST';
    // Normally we use POST to send XML documents.
    if (!isset($options['headers']['Content-Type'])) {
      $options['headers']['Content-Type'] = 'text/xml; charset=UTF-8';
    }
    $nonce = $this->prepareRequest($url, $options);
    $response = $this->_makeHttpRequest($url, $options);
    $response = $this->checkResponse($response);
    return $this->authenticateResponse($response, $nonce, $url);
  }
}
