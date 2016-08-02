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
   * Constructor.
   *
   * @param array $data
   *   The data provided by the SoftBank API to this response.
   */
  public function __construct(array $data);

  /**
   * Get the response string.
   *
   * @return string
   *   The response string as expected by the SoftBank API.
   */
  public function __toString();

}
