<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Response\Response.
 */

namespace Commerce\SBPayment\Response;

use Commerce\SBPayment\Product\ProductInterface;
use Commerce\SBPayment\SBPaymentException;

/**
 * Abstract class that all SoftBank response classes need to extend.
 */
abstract class Response {

  /**
   * String indicating a succesful response.
   *
   * @var string
   */
  const RESPONSE_OK = 'OK';

  /**
   * String indicating a failed response.
   *
   * @var string
   */
  const RESPONSE_FAIL = 'NG';

  /**
   * String indicating a canceled charge.
   *
   * @var string
   */
  const RESPONSE_CANCELED = 'CR';

  /**
   * String indicating a termination on the carrier side.
   *
   * @var string
   */
  const RESPONSE_TERMINATED = 'CC';

  /**
   * String indicating a deposit receipt notification.
   */
  const RESPONSE_DEPOSIT = 'PY';

  /**
   * String indicating a canceled charge due to expiration.
   *
   * @var string
   */
  const RESPONSE_EXPIRED = 'CN';

  /**
   * String indicating a canceled charge due to last billing month.
   *
   * Only applies to RECUIT Kantan.
   *
   * @var string
   */
  const RESPONSE_LAST_MONTH = 'CL';

  /**
   * Store the raw response data from SoftBank.
   *
   * @var array
   */
  protected $data;

  /**
   * List of products that are being purchased.
   *
   * @var SBPaymentProduct[]
   */
  protected $products = array();

  /**
   * The encryption key required to generate the checksum.
   *
   * @var string
   */
  protected $hashKey;

  /**
   * Result of this payment response.
   *
   * @var string
   */
  private $result = Response::RESPONSE_OK;

  /**
   * Result message.
   *
   * @var string
   */
  private $message;

  /**
   * Constructor.
   *
   * @param array $data
   *   Raw response data from SoftBank. Taken from the $_REQUEST superglobal.
   */
  public function __construct(array $data) {
    $this->data = $data;
  }

  /**
   * Set the list of products that are being purchased.
   *
   * @param SBPaymentProduct[] $products
   *   List containing SoftBank product objects.
   */
  public function setProducts(array $products) {
    // Validate the products.
    array_walk($products, array($this, 'validateProduct'));

    // Everything seems to be alright so set the products.
    $this->products = $products;
  }

  /**
   * Set the encryption hash key.
   *
   * @param string $hash_key
   *   The specified encryption hash key.
   */
  public function setHashKey($hash_key) {
    $this->hashKey = $hash_key;
  }

  /**
   * Get an array representation of this response.
   *
   * @return array
   *   Array containing the response data.
   */
  public function toArray() {
    return $this->data;
  }

  /**
   * Get the response string.
   *
   * @return string
   *   The response string as expected by the SoftBank API.
   */
  public function __toString() {
    $output = $this->result;

    if (!empty($this->message)) {
      $output .= ', ' . $this->message;
    }

    return $output;
  }

  /**
   * Get the SoftBank payment status based on the current data.
   *
   * @return string
   *   A payment status string from SoftBank.
   */
  public abstract function getRemoteStatus();

  /**
   * Get the SoftBank internal ID for the payment associated with this response.
   *
   * @return string
   *   A unique ID string containing the SoftBank payment ID.
   */
  public abstract function getRemoteId();

  /**
   * Get an associative array of the fields required to create a payment.
   *
   * @return string[]
   *   Array of field names required to create a payment. Array in the
   *   format 'machine_name => required', where required is either
   *   'mandatory' or 'required' as defined in the util class constants.
   */
  public abstract function getPaymentFields();

  /**
   * Get an associative array of the fields expected from a payment response.
   *
   * @return string[]
   *   Array of field names expected from a payment response. Array in the
   *   format 'machine_name => required', where required is either
   *   'mandatory' or 'required' as defined in the util class constants.
   */
  public abstract function getResponseFields();

  /**
   * Verify the response data from SoftBank.
   *
   * Ensure that the payment and response data is valid by checking raw data,
   * products and the sps_hashcode.
   *
   * @return bool
   *   Was the payment data succesfully verified?
   */
  public abstract function verify();

  /**
   * Validate a product for usage with the SoftBank Payment Service.
   *
   * @param ProductInterface $product
   *   An instantiated product object.
   *
   * @throws SBPaymentException
   */
  protected function validateProduct(ProductInterface $product) {
    // Check that each of the mandatory product fields are populated.
    foreach ($product::getMandatoryFields() as $mandatory_field) {
      if (!isset($product->toArray()[$mandatory_field])) {
        $this->error(sprintf('Mandatory product field %s is not populated', $mandatory_field));
      }
    }
  }

  /**
   * Report an error that occured with the response.
   *
   * @param string $message
   *   The error message.
   *
   * @throws SBPaymentException
   */
  protected function error($message = '') {
    $this->result = Response::RESPONSE_FAIL;
    $this->message = $message;
    throw new SBPaymentException($this->message);
  }

}
