<?php

/**
 * @file
 * Contains \Commerce\SBPayment\SBPayment.
 */

namespace Commerce\SBPayment;

use Commerce\SBPayment\Product\ProductInterface;

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
   *
   * @throws SBPaymentException
   */
  public function setMode($mode) {
    if (in_array($mode, array($self::API_MODE_TEST, $self::API_MODE_LIVE))) {
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
    $protocol = $this->https === TRUE ? 'https' : 'http';

    return sprintf('%s://%s', $protocol, $this->baseUrls[$this->mode]);
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
    array_walk($this->getPaymentFields(), function ($mandatory, $field_name) use ($payment_data) {
      // Ensure the field has been populated if it is mandatory.
      if ($mandatory === $self::PAYMENT_FIELD_MANDATORY && empty($payment_data[$field_name])) {
        throw new SBPaymentException(sprintf('Mandatory payment field %s is not populated.', $field_name));
      }
      elseif (!isset($payment_data[$field_name])) {
        // Otherwise just ensure the field is present.
        throw new SBPaymentException(sprintf('Payment field %s is missing.', $field_name));
      }
    });

    // Everything seems to be alright so set the data.
    $this->paymentData = $payment_data;
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
   * Calculate and return the checksum value based on the payment data.
   *
   * This is required to sign the payment data when sending it to the API.
   *
   * @return string
   *   Checksum value based on the payment data.
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
    // Check that each of the mandatory product fields are populated.
    foreach (ProductInterface::getMandatoryFields() as $mandatory_field) {
      if (empty($product->{$mandatory_field})) {
        throw new SBPaymentException(sprintf('Mandatory product field %s is not populated', $field_name));
      }
    }
  }

}
