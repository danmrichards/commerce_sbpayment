<?php

/**
 * @file
 * Contains the SBPaymentLink class.
 */

/**
 * Implements the SoftBank Payment 'Link Type' Service.
 */
class SBPaymentLink extends SBPayment {

  /**
   * The API endpoint for the 'Link Type' service.
   *
   * @var string
   */
  protected $endpoint = 'f01/FepBuyInfoReceive.do';

  /**
   * The encryption key required to generate the checksum.
   *
   * @var string
   */
  protected $hashKey;

  /**
   * Constructor.
   *
   * @param string $hash_key
   *   The specified encryption key.
   */
  public function __construct($hash_key) {
    $this->hashKey = $hash_key;
  }

  /**
   * {@inheritdoc}
   */
  public function getServiceUrl() {
    return sprintf('%s/%s', $this->getBaseUrl(), $this->endpoint);
  }

  /**
   * {@inheritdoc}
   */
  public function getPaymentFields() {
    return array(
      'pay_method' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'merchant_id' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'service_id' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'cust_code' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'sps_cust_no' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'sps_payment_no' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'order_id' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'item_id' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'pay_item_id' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'item_name' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'tax' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'amount' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'pay_type' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'auto_charge_type' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'service_type' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'div_settele' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'last_charge_mont' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'camp_type' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'tracking_id' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'terminal_type' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'success_url' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'cancel_url' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'error_url' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'pagecon_url' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'free1' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'free2' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'free3' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'free_csv_input' => SBPayment::PAYMENT_FIELD_OPTIONAL,
      'request_date' => SBPayment::PAYMENT_FIELD_MANDATORY,
      'limit_second' => SBPayment::PAYMENT_FIELD_OPTIONAL,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setPaymentData(array $payment_data) {
    // Strip out any unexpected fields.
    $payment_data = array_filter($payment_data, array($this, 'validatePaymentDataField'), ARRAY_FILTER_USE_KEY);

    // Check all of the payment fields are present and the mandatory fields
    // have been populated.
    array_walk($this->getPaymentFields(), function ($mandatory, $field_name) use ($payment_data) {
      // Ensure the field has been populated if it is mandatory.
      if ($mandatory === SBPayment::PAYMENT_FIELD_MANDATORY && empty($payment_data[$field_name])) {
        throw new SBPaymentException(t('Mandatory payment field @field_name is not populated.', array('@field_name' => $field_name)));
      }
      elseif (!isset($payment_data[$field_name])) {
        // Otherwise just ensure the field is present.
        throw new SBPaymentException(t('Payment field @field_name is missing', array('@field_name' => $field_name)));
      }
    });

    // Everything seems to be alright so set the data.
    $this->paymentData = $payment_data;
  }

  /**
   * {@inheritdoc}
   */
  public function setProducts(array $products) {
    // Validate the products.
    array_walk($products, array($this, 'validateProduct'));

    // Everything seems to be alright so set the products.
    $this->products = $products;
  }

  /**
   * {@inheritdoc}
   */
  public function getChecksum() {
    // Concatenate the fields and convert encoding.
    $checksum_fields = array_reduce($this->paymentData, function ($carry, $item) {
      return $carry . mb_convert_encoding($item, 'UTF-8', 'SJIS');
    });

    // Append the hashkey.
    $checksum_fields = mb_convert_encoding($checksum_fields . $this->hashKey, 'UTF-8', 'SJIS');

    // Calculate the hash.
    return sha1($checksum_fields);
  }

  /**
   * {@inheritdoc}
   */
  protected function validatePaymentDataField($payment_data_field) {
    return in_array($payment_data_field, array_keys($this->getPaymentFields()));
  }

  /**
   * Validate a product for usage with the SoftBank Payment Service.
   *
   * @param SBPaymentProduct $product
   *   An instantiated product object.
   */
  protected function validateProduct(SBPaymentProduct $product) {
    // Check that each of the mandatory product fields are populated.
    foreach (SBPaymentProduct::getMandatoryFields() as $mandatory_field) {
      if (empty($product->{$mandatory_field})) {
        throw new SBPaymentException(t('Mandatory product field @field_name is not populated', array('@field_name' => $field_name)));
      }
    }
  }

}
