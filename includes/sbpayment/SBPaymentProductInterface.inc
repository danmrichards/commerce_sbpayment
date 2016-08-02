<?php

/**
 * @file
 * Contains SBPaymentProductInterface.
 */

/**
 * Interface that all SoftBank product classes need to implement.
 */
interface SBPaymentProductInterface {

  /**
   * Get the list of fields mandatory for a product.
   *
   * @return string[]
   *   Array of field names matching properties of this class.
   */
  public static function getMandatoryFields();

}
