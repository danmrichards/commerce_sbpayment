<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Product\ProductInterface.
 */

namespace Commerce\SBPayment\Product;

/**
 * Interface that all SoftBank product classes need to implement.
 */
interface ProductInterface {

  /**
   * Constructor.
   *
   * @param array $product_data
   *   Assocative array of product data.
   */
  public function __construct(array $product_data);

  /**
   * Get the list of fields mandatory for a product.
   *
   * @return string[]
   *   Array of field names matching properties of this class.
   */
  public static function getMandatoryFields();

  /**
   * Get an array representation of this product.
   *
   * @return array
   *   Array containing the product data.
   */
  public function toArray();

  /**
   * Returns a string representation of this SoftBank product.
   *
   * Intended for use with checksum generation.
   *
   * @return string
   *   String representation of this SoftBank product. A concatenation of all
   *   product data fields.
   *
   * @see getChecksum()
   */
  public function __toString();

}
