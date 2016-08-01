<?php

/**
 * @file
 * Contains the SBPaymentclass.
 */

/**
 * Abstract class for interacting with the SoftBank Payment Service.
 */
abstract class SBPayment {

  /**
   * The test API mode flag.
   *
   * @string
   */
  const API_MODE_TEST = 'test';

  /**
   * The live API mode flag.
   *
   * @string
   */
  const API_MODE_LIVE = 'live';

  /**
   * What mode should the API be used in?
   *
   * @var string
   */
  protected $mode;

  /**
   * Should API calls be made using https.
   *
   * @var bool
   */
  protected $https = TRUE;

  /**
   * Data required to create a payment.
   *
   * @var string[]
   */
  protected $paymentData = array();

  /**
   * List of products that are being purchased.
   *
   * @var SBPaymentProduct[]
   */
  protected $products = array();

  /**
   * API base URLs for each valid mode.
   *
   * @var array
   */
  protected $baseUrls = array(
    SBPayment::API_MODE_TEST => 'stbfep.sps-system.com',
    SBPayment::API_MODE_LIVE => 'fep.sps-system.com',
  );

  /**
   * Set the API mode.
   *
   * @param string $mode
   *   What mode should the API be used in? Must be a valid mode name as defined
   *   in this class.
   */
  public function setMode($mode) {
    if (in_array($mode, array(SBPayment::API_MODE_TEST, SBPayment::API_MODE_LIVE))) {
      $this->mode = $mode;
    }
    else {
      throw new SBPaymentException('Invalid SoftBank API mode specified');
    }
  }

  /**
   * Set the flag to indicate if API calls be made using https.
   *
   * @param bool $https
   *   Should API calls be made using https.
   */
  public function setHttps($https) {
    // Check we have an endpoint for the requested format.
    if (is_bool($https)) {
      $this->https = $https;
    }
    else {
      throw new SBPaymentException('SoftBank HTTPS flag must be a boolean.');
    }
  }

  /**
   * Get the base URL for the current API mode.
   *
   * @return string
   *   Base URL.
   */
  public function getBaseUrl() {
    return $this->baseUrls[$this->mode];
  }

  /**
   * Get the service URL for the current API mode.
   *
   * @return string
   *   Fully qualified URL for the SoftBank service in the current mode.
   */
  public abstract function getServiceUrl();

  /**
   * Get a list of the fields required to create a payment.
   *
   * @return string[]
   *   Array of field names as defined in the API docuementation.
   */
  public abstract function getPaymentFields();

  /**
   * Set the data required to create a payment.
   *
   * Each field returned from getPaymentFields() must be populated.
   *
   * @param array $payment_data
   *   Associative array of payment data.
   */
  public abstract function setPaymentData($payment_data);

  /**
   * Set the list of products that are being purchased.
   *
   * @param SBPaymentProduct[] $products
   *   List containing SoftBank product objects.
   */
  public abstract function setProducts($products);

}
