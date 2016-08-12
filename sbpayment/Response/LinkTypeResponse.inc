<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Response\LinkTypeResponse.
 */

namespace Commerce\SBPayment\Response;

use Commerce\SBPayment\SBPaymentException;

/**
 * SoftBank response class for the 'Link Type' service.
 */
class LinkTypeResponse implements ResponseInterface {

  /**
   * List of fields that are always expected in the response data.
   *
   * @var array
   */
  private $mandatoryResultFields = array(
    'res_pay_method',
    'res_result',
    'res_tracking_id',
    'res_date',
    'sps_hashcode',
  );

  /**
   * Store the response data from SoftBank.
   *
   * @var array
   */
  private $data;

  /**
   * Result of this payment response.
   *
   * @var string
   */
  private $result = ResponseInterface::RESPONSE_OK;

  /**
   * Result message.
   *
   * @var string
   */
  private $message;

  /**
   * {@inheritdoc}
   */
  public function setResponseData(array $data) {
    // Ensure we have some response data.
    if (empty($data)) {
      $this->result = ResponseInterface::RESPONSE_FAIL;
      $this->message = 'Response data from SoftBank was empty.';
      throw new SBPaymentException($this->message);
    }

    // Ensure all of the mandatory fields are present in the response data.
    foreach ($this->mandatoryResultFields as $mandatory_field) {
      if (empty($data[$mandatory_field])) {
        $this->result = ResponseInterface::RESPONSE_FAIL;
        $this->message = sprintf('Mandatory field %s is not populated in response data from SoftBank.', $mandatory_field);
        throw new SBPaymentException($this->message);
      }

      $this->data[$mandatory_field] = $data[$mandatory_field];
    }

    // TODO: Verify the returned payment hash.
  }

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
  public function toArray() {
    return $this->data;
  }

  /**
   * {@inheritdoc}
   */
  public function __toString() {
    $output = $this->result;

    if (!empty($this->message)) {
      $output .= ', ' . $this->message;
    }

    return $output;
  }

}
