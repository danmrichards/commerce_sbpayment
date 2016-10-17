<?php

/**
 * @file
 * Contains \Commerce\SBPayment\SBPayment.
 */

namespace Commerce\SBPayment;

use Commerce\SBPayment\Product\ProductInterface;
use Commerce\SBPayment\Util\Util;

/**
 * Abstract class for interacting with the SoftBank Payment Service.
 */
abstract class SBPayment {

  /**
   * Test API mode flag.
   *
   * @var string
   */
  const API_MODE_TEST = 'test';

  /**
   * Live API mode flag.
   *
   * @var string
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
   * Data required to create a payment keyed by field name.
   *
   * @var array
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
   * The encryption key required to generate the checksum.
   *
   * @var string
   */
  protected $hashKey;

  /**
   * Set the encryption hash key.
   *
   * @param string $hash_key
   *   The specified encryption hash key.
   */
  public function setHashKey($hash_key) {
    $this->hashKey = $hash_key;

    return $this;
  }

  /**
   * Get the API mode.
   *
   * @return string
   *   String representing the current API mode.
   */
  public function getMode() {
    return $this->mode;
  }

  /**
   * Set the API mode.
   *
   * @param string $mode
   *   What mode should the API be used in? Must be a valid mode name as defined
   *   in this class.
   *
   * @return self
   *   This instantiated SBPayment object.
   *
   * @throws SBPaymentException
   */
  public function setMode($mode) {
    if (in_array($mode, array(SBPayment::API_MODE_TEST, SBPayment::API_MODE_LIVE))) {
      $this->mode = $mode;
    }
    else {
      throw new SBPaymentException('Invalid SoftBank API mode specified');
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
   *
   * @throws SBPaymentException
   */
  public function setHttps($https) {
    // Check we have an endpoint for the requested format.
    if (is_bool($https)) {
      $this->https = $https;
    }
    else {
      throw new SBPaymentException('SoftBank HTTPS flag must be a boolean.');
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
    $protocol = $this->https ? 'https' : 'http';

    return sprintf('%s://%s', $protocol, $this->baseUrls[$this->mode]);
  }

  /**
   * Get the data required to create a payment.
   *
   * @return array
   *   Associative array of payment data.
   */
  public function getPaymentData() {
    return $this->paymentData;
  }

  /**
   * Set the data required to create a payment.
   *
   * Each field returned from getPaymentFields() must be populated.
   *
   * @param array $payment_data
   *   Associative array of payment data.
   *
   * @throws SBPaymentException
   */
  public function setPaymentData(array $payment_data) {
    // Strip out any unexpected fields.
    $payment_data = array_filter($payment_data, array($this, 'isValidPaymentDataField'), ARRAY_FILTER_USE_KEY);

    // Check all of the payment fields are present and the mandatory fields
    // have been populated.
    $payment_fields = $this->getPaymentFields();

    array_walk($payment_fields, function ($mandatory, $field_name) use (&$payment_data) {
      // Ensure the field has been populated if it is mandatory.
      if ($mandatory === Util::PAYMENT_FIELD_MANDATORY && !isset($payment_data[$field_name])) {
        throw new SBPaymentException(sprintf('Mandatory payment field %s is not populated.', $field_name));
      }

      // Set the field or blank it out if not present.
      $this->paymentData[$field_name] = '';
      if (!empty($payment_data[$field_name])) {
        $this->paymentData[$field_name] = trim($payment_data[$field_name]);
      }
      elseif (isset($payment_data[$field_name]) && $payment_data[$field_name] === 0) {
        $this->paymentData[$field_name] = $payment_data[$field_name];
      }
    });

    return $this;
  }

  /**
   * Get the list of products that are being purchased.
   *
   * @return SBPaymentProduct[] $products
   *   List containing SoftBank product objects.
   */
  public function getProducts() {
    return $this->products;
  }

  /**
   * Set the list of products that are being purchased.
   *
   * @param SBPaymentProduct[] $products
   *   List containing SoftBank product objects.
   */
  public function setProducts(array $products) {
    // Validate the products.
    array_walk($products, array($this, 'validateProduct'));

    // Everything seems to be alright so set the products.
    $this->products = $products;

    return $this;
  }

  /**
   * Returns the type of this SoftBank payment service.
   *
   * @return string
   *   Machine name of this SoftBank payment service.
   */
  public abstract function __toString();

  /**
   * Get the service URL for the current API mode.
   *
   * @return string
   *   Fully qualified URL for the SoftBank service in the current mode.
   */
  public abstract function getServiceUrl();

  /**
   * Get the PHP class name of the notification handler for this service.
   *
   * The class must extend the Commerce\SBPayment\Response\Response abstract
   * class.
   *
   * @return string
   *   PHP class name.
   */
  public abstract function getNotificationHandler();

  /**
   * Get the PHP class name of the notification handler for this service.
   *
   * The class must extend the Commerce\SBPayment\Response\Response abstract
   * class.
   *
   * @return string
   *   PHP class name.
   */
  public abstract function getResultHandler();

  /**
   * Get an associative array of the fields required to create a payment.
   *
   * @return string[]
   *   Array of field names required to create a payment. Array in the
   *   format 'machine_name => required', where required is either
   *   'mandatory' or 'required' as defined in the util class constants.
   */
  public abstract function getPaymentFields();

  /**
   * Calculate and return the checksum value based on the payment data.
   *
   * This is required to sign the payment data when sending it to the API.
   *
   * @return string
   *   Checksum value based on the payment data.
   *
   * @throws SBPaymentException
   */
  public abstract function getChecksum();

  /**
   * Is the supplied field name a valid payment data field.
   *
   * @param string $payment_data_field
   *   The name of a payment data field.
   *
   * @return bool
   *   Is the supplied field name a valid one?
   *
   * @see getPaymentFields()
   */
  protected function isValidPaymentDataField($payment_data_field) {
    return in_array($payment_data_field, array_keys($this->getPaymentFields()));
  }

  /**
   * Validate a product for usage with the SoftBank Payment Service.
   *
   * @param ProductInterface $product
   *   An instantiated product object.
   *
   * @throws SBPaymentException
   */
  protected function validateProduct(ProductInterface $product) {
    $product_data = $product->toArray();

    // Check that each of the mandatory product fields are populated.
    foreach ($product::getMandatoryFields() as $mandatory_field) {
      if (!isset($product_data[$mandatory_field])) {
        throw new SBPaymentException(sprintf('Mandatory product field %s is not populated', $mandatory_field));
      }
    }
  }

}
