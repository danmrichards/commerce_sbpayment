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
   * Test API mode flag.
   *
   * @string
   */
  const API_MODE_TEST = 'test';

  /**
   * Live API mode flag.
   *
   * @string
   */
  const API_MODE_LIVE = 'live';

  /**
   * Payment field mandatory flag.
   *
   * @string
   */
  const PAYMENT_FIELD_MANDATORY = 'mandatory';

  /**
   * Payment field optional flag.
   *
   * @string
   */
  const PAYMENT_FIELD_OPTIONAL = 'optional';

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
   * Data required to create a payment keyed by field name.
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
   * API base URLs for each valid mode keyed by API mode.
   *
   * @var string[]
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
   *
   * @return self
   *   This instantiated SBPayment object.
   */
  public function setMode($mode) {
    if (in_array($mode, array(SBPayment::API_MODE_TEST, SBPayment::API_MODE_LIVE))) {
      $this->mode = $mode;
    }
    else {
      throw new SBPaymentException(t('Invalid SoftBank API mode specified'));
    }

    return $this;
  }

  /**
   * Set the flag to indicate if API calls be made using https.
   *
   * @param bool $https
   *   Should API calls be made using https.
   *
   * @return self
   *   This instantiated SBPayment object.
   */
  public function setHttps($https) {
    // Check we have an endpoint for the requested format.
    if (is_bool($https)) {
      $this->https = $https;
    }
    else {
      throw new SBPaymentException(t('SoftBank HTTPS flag must be a boolean.'));
    }

    return $this;
  }

  /**
   * Get the base URL for the current API mode.
   *
   * @return string
   *   Fully qualified URL for the base of the SoftBank API.
   */
  public function getBaseUrl() {
    // Determine the protocol we're using.
    $protocol = $this->https === TRUE ? 'https' : 'http';

    return sprintf('%s://%s', $protocol, $this->baseUrls[$this->mode]);
  }

  /**
   * Get the service URL for the current API mode.
   *
   * @return string
   *   Fully qualified URL for the SoftBank service in the current mode.
   */
  public abstract function getServiceUrl();

  /**
   * Get an associatvie array of the fields required to create a payment.
   *
   * @return string[]
   *   Array of field names required to create a payment. Array in the
   *   format 'machine_name => required', where required is either
   *   'mandatory' or 'required' as defined in this class constants.
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
  public abstract function setPaymentData(array $payment_data);

  /**
   * Set the list of products that are being purchased.
   *
   * @param SBPaymentProduct[] $products
   *   List containing SoftBank product objects.
   */
  public abstract function setProducts(array $products);

  /**
   * Calculate and return the checksum value based on the payment data.
   *
   * This is required to sign the payment data when sending it to the API.
   *
   * @return string
   *   Checksum value based on the payment data.
   */
  public abstract function getChecksum();

}
