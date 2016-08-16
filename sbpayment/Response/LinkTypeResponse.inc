<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Response\LinkTypeResponse.
 */

namespace Commerce\SBPayment\Response;

use Commerce\SBPayment\Util\Util;

/**
 * SoftBank response class for the 'Link Type' service.
 */
class LinkTypeResponse extends Response {

  /**
   * {@inheritdoc}
   */
  public function getRemoteStatus() {
    return !empty($this->data['res_result']) ? $this->data['res_result'] : '';
  }

  /**
   * {@inheritdoc}
   */
  public function getRemoteId() {
    return !empty($this->data['res_tracking_id']) ? $this->data['res_tracking_id'] : '';
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
      'free1' => Util::PAYMENT_FIELD_OPTIONAL,
      'free2' => Util::PAYMENT_FIELD_OPTIONAL,
      'free3' => Util::PAYMENT_FIELD_OPTIONAL,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getResponseFields() {
    return array(
      'res_pay_method' => Util::PAYMENT_FIELD_MANDATORY,
      'res_result' => Util::PAYMENT_FIELD_MANDATORY,
      'res_tracking_id' => Util::PAYMENT_FIELD_MANDATORY,
      'res_sps_cust_no' => Util::PAYMENT_FIELD_OPTIONAL,
      'res_sps_payment_no' => Util::PAYMENT_FIELD_OPTIONAL,
      'res_payinfo_key' => Util::PAYMENT_FIELD_OPTIONAL,
      'res_payment_date' => Util::PAYMENT_FIELD_OPTIONAL,
      'res_err_code' => Util::PAYMENT_FIELD_OPTIONAL,
      'res_date' => Util::PAYMENT_FIELD_MANDATORY,
      'limit_second' => Util::PAYMENT_FIELD_MANDATORY,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function verify() {
    // We can't continue without the hash key.
    if (empty($this->hashKey)) {
      $this->error('SoftBank hash key is missing');
    }

    // Ensure we have some response data.
    if (empty($this->data)) {
      $this->error('Response data from SoftBank was empty.');
    }

    // Build the array of data for checksum verification.
    $checksum_data = array();

    // Ensure all of the mandatory payment fields are present in the response
    // data.
    foreach ($this->getPaymentFields() as $field_name => $mandatory) {
      // Ensure the field has been populated if it is mandatory.
      if ($mandatory === Util::PAYMENT_FIELD_MANDATORY && !isset($this->data[$field_name])) {
        $this->error(sprintf('Mandatory payment field %s is not populated.', $field_name));
      }

      // Add the field value to the checksum array.
      $checksum_data[$field_name] = $this->data[$field_name];
    }

    // Append the data for each product to the checksum array.
    $i = 1;
    foreach ($this->products as $product) {
      foreach ($product->toArray() as $product_field => $product_data) {
        $checksum_data[$product_field . '__' . $i] = $product_data;
      }
      ++$i;
    }

    // Verify the request_date field seperately because it has to be inserted in
    // the checksum string after the product data.
    if (!empty($this->data['request_date'])) {
      $checksum_data = $this->data['request_date'];
    }
    else {
      $this->error('Mandatory response field request_date is not populated.');
    }

    // Ensure all of the mandatory response fields are present in the response
    // data.
    foreach ($this->getResponseFields() as $field_name => $mandatory) {
      // Ensure the field has been populated if it is mandatory.
      if ($mandatory === Util::PAYMENT_FIELD_MANDATORY && !isset($this->data[$field_name])) {
        $this->error(sprintf('Mandatory response field %s is not populated.', $field_name));
      }

      // Add the field value to the checksum array.
      $checksum_data[$field_name] = $this->data[$field_name];
    }

    // Verify the sps_hashcode in the response data.
    if ($this->data['sps_hashcode'] !== Util::generateSpsHashcode($checksum_data, $this->hashKey)) {
      $this->error('Invalid sps_hashcode.');
    }

    return TRUE;
  }

}
