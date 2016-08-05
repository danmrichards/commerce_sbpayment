<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Service\LinkType.
 */

namespace Commerce\SBPayment\Service;

use Commerce\SBPayment\SBPayment;
use Commerce\SBPayment\SBPaymentException;
use Commerce\SBPayment\Util\Util;
use Commerce\SBPayment\Response\LinkTypeCgi;
use Commerce\SBPayment\Response\PurchaseResult;

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
  public function getNotificationHandler() {
    return LinkTypeCgi::class;
  }

  /**
   * {@inheritdoc}
   */
  public function getResultHandler() {
    return PurchaseResult::class;
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
    $this->requestDate = Util::generateJapanTime('Ymdhis');

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

    // Generate the initial checksum string with just the payment data.
    $checksum_fields = $this->paymentData;
    $checksum_string = implode('', $checksum_fields);

    // Append the data for each product.
    foreach ($this->products as $product) {
      $checksum_string .= (string) $product;
    }

    // Append the final pieces of manually set data.
    $checksum_string .= $this->requestDate;
    $checksum_string .= $this->limitSecond;
    $checksum_string .= $this->hashKey;

    $checksum_string = mb_convert_encoding($checksum_string, 'UTF-8', 'SJIS');

    // Calculate the hash.
    return sha1($checksum_string);
  }

  /**
   * Get the request date value.
   *
   * @return string
   *   Timestamp, in Japan time, for this request.
   */
  public function getRequestDate() {
    return $this->requestDate;
  }

  /**
   * Get the limit_second value.
   *
   * @return int
   *   The time, in seconds, that this request is valid for.
   */
  public function getLimitSecond() {
    return $this->limitSecond;
  }

}
