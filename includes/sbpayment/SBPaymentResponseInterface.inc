<?php

/**
 * @file
 * Contains SBPaymentResponseInterface.
 */

/**
 * Interface that all SoftBank response classes need to implement.
 */
interface SBPaymentResponseInterface {

  /**
   * The type of response.
   *
   * @return string
   *   A lowercase machine name for this repsonse time. e.g. cgi_notification.
   */
  public static function type();

  /**
   * Get the response string.
   *
   * @return string
   *   The response string as expected by the SoftBank API.
   */
  public function __toString();

  /**
   * Constructor.
   *
   * @param array $data
   *   The data provided by the SoftBank API to this response.
   */
  public function __construct(array $data);

}
