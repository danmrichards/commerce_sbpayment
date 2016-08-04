<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Service\LinkType.
 */

namespace Commerce\SBPayment\Service;

use Commerce\SBPayment\SBPayment;
use Commerce\SBPayment\SBPaymentException;
use Commerce\SBPayment\Response\LinkTypeCgi;

/**
 * Implements the SoftBank Payment 'Link Type' Service.
 */
class LinkType extends SBPayment {

  /**
   * Date stamp for this payment creation.
   *
   * Format: Ymdhis.
   *
   * @var string
   */
  private $requestDate;

  /**
   * Expiration time of this payment request.
   *
   * @var string
   */
  private $limitSecond = '';

  /**
   * The API endpoint for the 'Link Type' service.
   *
   * @var string
   */
  protected $endpoint = 'f01/FepBuyInfoReceive.do';

  /**
   * {@inheritdoc}
   */
  public function __toString() {
    return 'link_type';
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
  public function getResponseHandler() {
    return LinkTypeCgi::class;
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
      'last_charge_month' => SBPayment::PAYMENT_FIELD_OPTIONAL,
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
      'free_csv' => SBPayment::PAYMENT_FIELD_OPTIONAL,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getPaymentData() {
    $payment_data = $this->paymentData;
    $this->requestDate = date('Ymdhis');

    // Add the request data and limit_second fields manually. This is to allow
    // easier form and checksum generation.
    // TODO: Look at making limit_second field configurable.
    $payment_data['request_date'] = $this->requestDate;
    $payment_data['limit_second'] = $this->limitSecond;

    return $payment_data;
  }

  /**
   * {@inheritdoc}
   */
  public function getChecksum() {
    // We can't continue without the hash key.
    if (empty($this->hashKey)) {
      throw new SBPaymentException('SoftBank hash key is missing');
    }

    // Concatenate the fields and convert encoding.
    $checksum_fields = array_reduce($this->paymentData, function ($carry, $item) {
      return $carry . mb_convert_encoding($item, 'UTF-8', 'SJIS');
    });

    // Append the data for each product.
    foreach ($this->products as $product) {
      $checksum_fields = mb_convert_encoding($checksum_fields . (string) $product, 'UTF-8', 'SJIS');
    }

    // Append the request data and limit second values.
    $checksum_fields = mb_convert_encoding($checksum_fields . $this->requestDate, 'UTF-8', 'SJIS');
    $checksum_fields = mb_convert_encoding($checksum_fields . $this->limitSecond, 'UTF-8', 'SJIS');

    // Append the hashkey.
    $checksum_fields = mb_convert_encoding($checksum_fields . $this->hashKey, 'UTF-8', 'SJIS');

    // Calculate the hash.
    return sha1($checksum_fields);
  }

}
