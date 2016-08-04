<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Response\PurchaseResult.
 */

namespace Commerce\SBPayment;

use Commerce\SBPayment\Response\ResponseInterface;

/**
 * SoftBank response class for the 'Link Type' service purchase result.
 */
class PurchaseResult implements ResponseInterface {

  /**
   * {@inheritdoc}
   */
  public function __construct(array $data) {};

  /**
   * {@inheritdoc}
   */
  public function getResult() {}

  /**
   * {@inheritdoc}
   */
  public function __toString() {};

}
