<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Response\LinkTypeCgi.
 */

namespace Commerce\SBPayment;

use Commerce\SBPayment\Response\ResponseInterface;

/**
 * Interface that all SoftBank response classes need to implement.
 */
class LinkTypeCgi implements ResponseInterface {

  /**
   * Constructor.
   *
   * @param array $data
   *   The data provided by the SoftBank API to this response.
   */
  public function __construct(array $data) {};

  /**
   * Get the response string.
   *
   * @return string
   *   The response string as expected by the SoftBank API.
   */
  public function __toString() {};

}
