<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Service\LinkType.
 */

namespace Commerce\SBPayment\Service;

use Commerce\SBPayment\SBPayment;
use Commerce\SBPayment\SBPaymentException;
use Commerce\SBPayment\Util\Util;
use Commerce\SBPayment\Response\LinkTypeResponse;

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
   * Constructor.
   */
  public function __construct() {
    // Set the request date to the current time in Japan.
    $this->requestDate = Util::generateJapanTime('YmdHis');
  }

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
    return LinkTypeResponse::class;
  }

  /**
   * {@inheritdoc}
   */
  public function getResultHandler() {
    return LinkTypeResponse::class;
  }

  /**
   * {@inheritdoc}
   */
  public function getPaymentFields() {
    return array(
      'pay_method' => Util::PAYMENT_FIELD_MANDATORY,
      'merchant_id' => Util::PAYMENT_FIELD_MANDATORY,
      'service_id' => Util::PAYMENT_FIELD_MANDATORY,
      'cust_code' => Util::PAYMENT_FIELD_MANDATORY,
      'sps_cust_no' => Util::PAYMENT_FIELD_OPTIONAL,
      'sps_payment_no' => Util::PAYMENT_FIELD_OPTIONAL,
      'order_id' => Util::PAYMENT_FIELD_MANDATORY,
      'item_id' => Util::PAYMENT_FIELD_MANDATORY,
      'pay_item_id' => Util::PAYMENT_FIELD_OPTIONAL,
      'item_name' => Util::PAYMENT_FIELD_OPTIONAL,
      'tax' => Util::PAYMENT_FIELD_OPTIONAL,
      'amount' => Util::PAYMENT_FIELD_MANDATORY,
      'pay_type' => Util::PAYMENT_FIELD_MANDATORY,
      'auto_charge_type' => Util::PAYMENT_FIELD_OPTIONAL,
      'service_type' => Util::PAYMENT_FIELD_MANDATORY,
      'div_settele' => Util::PAYMENT_FIELD_OPTIONAL,
      'last_charge_month' => Util::PAYMENT_FIELD_OPTIONAL,
      'camp_type' => Util::PAYMENT_FIELD_OPTIONAL,
      'tracking_id' => Util::PAYMENT_FIELD_OPTIONAL,
      'terminal_type' => Util::PAYMENT_FIELD_OPTIONAL,
      'success_url' => Util::PAYMENT_FIELD_OPTIONAL,
      'cancel_url' => Util::PAYMENT_FIELD_OPTIONAL,
      'error_url' => Util::PAYMENT_FIELD_OPTIONAL,
      'pagecon_url' => Util::PAYMENT_FIELD_OPTIONAL,
      'free1' => Util::PAYMENT_FIELD_OPTIONAL,
      'free2' => Util::PAYMENT_FIELD_OPTIONAL,
      'free3' => Util::PAYMENT_FIELD_OPTIONAL,
      'free_csv' => Util::PAYMENT_FIELD_OPTIONAL,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getChecksum() {
    // We can't continue without the hash key.
    if (empty($this->hashKey)) {
      throw new SBPaymentException('SoftBank hash key is missing');
    }

    // Start with the payment data.
    $checksum_data = $this->paymentData;

    // Append the data for each product.
    $i = 1;
    foreach ($this->products as $product) {
      foreach ($product->toArray() as $product_field => $product_data) {
        $checksum_data[$product_field . '__' . $i] = $product_data;
      }
      ++$i;
    }

    // Add the final details.
    $checksum_data['request_date'] = $this->requestDate;
    $checksum_data['limit_second'] = $this->limitSecond;

    // Generate the hash code.
    return Util::generateSpsHashcode($checksum_data, $this->hashKey);
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
