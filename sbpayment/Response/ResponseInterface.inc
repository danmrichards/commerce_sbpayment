<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Response\ResponseInterface.
 */

namespace Commerce\SBPayment\Response;

/**
 * Interface that all SoftBank response classes need to implement.
 */
interface ResponseInterface {

  /**
   * String expected by SoftBank in the event of a succesful response.
   *
   * @var string
   */
  const RESPONSE_OK = 'OK';

  /**
   * String expected by SoftBank in the event of a failed response.
   *
   * @var string
   */
  const RESPONSE_FAIL = 'NG';

  /**
   * Set the payment response data.
   *
   * @param array $data
   *   The data provided by the SoftBank API to this response.
   */
  public function setResponseData(array $data);

  /**
   * Get the SoftBank payment status based on the current data.
   *
   * @return string
   *   A payment status string from SoftBank.
   */
  public function getRemoteStatus();

  /**
   * Get the SoftBank internal ID for the payment associated with this response.
   *
   * @return string
   *   A unique ID string containing the SoftBank payment ID.
   */
  public function getRemoteId();

  /**
   * Get an array representation of this response.
   *
   * @return array
   *   Array containing the response data.
   */
  public function toArray();

  /**
   * Get the response string.
   *
   * @return string
   *   The response string as expected by the SoftBank API.
   */
  public function __toString();

}
