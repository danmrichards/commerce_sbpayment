<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Service\LinkType.
 */

namespace Commerce\SBPayment\Service;

use Commerce\SBPayment\SBPayment;

/**
 * Implements the SoftBank Payment 'Link Type' Service.
 */
class LinkType extends SBPayment {

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

}
