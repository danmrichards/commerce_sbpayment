<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Product\LinkTypeProduct.
 */

namespace Commerce\SBPayment\Product;

/**
 * Class to model a product for use on the SoftBank payment 'Link Type' service.
 */
class LinkTypeProduct implements ProductInterface {

  /**
   * Data required for a SoftBank product.
   */
  private $data = array(
    'dtl_rowno' => 1,
    'dtl_item_id' => '',
    'dtl_item_name' => '',
    'dtl_item_count' => 1,
    'dtl_tax' => '',
    'dtl_amount' => '',
    'dtl_free1' => '',
    'dtl_free2' => '',
    'dtl_free3' => '',
  );

  /**
   * {@inheritdoc}
   */
  public function __construct(array $product_data) {
    // Remove any spaces and URL encode the data.
    foreach ($product_data as $field => $data) {
      $product_data[$field] = urlencode(trim($data));
    }

    $this->data = array_merge($this->data, $product_data);
  }

  /**
   * {@inheritdoc}
   */
  public static function getMandatoryFields() {
    return array(
      'dtl_rowno',
      'dtl_item_id',
      'dtl_item_name',
    );
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
    return implode('', $this->data);
  }

  /**
   * Get the row number value.
   *
   * @return int
   *   Row number.
   */
  public function getDtlRowNo() {
    return $this->dtlRowNo;
  }

  /**
   * Set the row number value.
   *
   * @param int $dtl_row_no
   *   The specified row number.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlRowNo($dtl_row_no) {
    $this->dtlRowNo = $dtl_row_no;

    return $this;
  }

  /**
   * Get the item ID.
   *
   * @return string
   *   The item ID.
   */
  public function getDtlItemId() {
    return $this->dtlItemId;
  }

  /**
   * Set the item id.
   *
   * @param int $dtl_item_id
   *   The specified item id.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlItemId($dtl_item_id) {
    $this->dtlItemId = $dtl_item_id;

    return $this;
  }

  /**
   * Get the Item name.
   *
   * @return string
   *   The item name.
   */
  public function getDtlItemName() {
    return $this->dtlItemName;
  }

  /**
   * Set the item name.
   *
   * @param string $dtl_item_name
   *   The specified item name.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlItemName($dtl_item_name) {
    $this->dtlItemName = $dtl_item_name;

    return $this;
  }

  /**
   * Get the item quantity.
   *
   * @return int
   *   The item quantity.
   */
  public function getDtlItemCount() {
    return $this->dtlItemCount;
  }

  /**
   * Set the item quantity.
   *
   * @param string $dtl_item_count
   *   The specified item quantity.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlItemCount($dtl_item_count) {
    $this->dtlItemCount = $dtl_item_count;

    return $this;
  }

  /**
   * Get the item tax amount in JPY.
   *
   * @return float
   *   The tax amount.
   */
  public function getDtlTax() {
    return $this->dtlTax;
  }

  /**
   * Set the item tax amount in JPY.
   *
   * @param string $dtl_tax
   *   The specified item tax amount in JPY.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlTax($dtl_tax) {
    $this->dtlTax = $dtl_tax;

    return $this;
  }

  /**
   * Get the product price (including tax) in JPY.
   *
   * @return float
   *   The product price in JPY.
   */
  public function getDtlAmount() {
    return $this->dtlAmount;
  }

  /**
   * Set the product price (including tax) in JPY.
   *
   * @param string $dtl_amount
   *   The specified item tax amount in JPY.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlAmount($dtl_amount) {
    $this->dtlAmount = $dtl_amount;

    return $this;
  }

  /**
   * Get the value of free text 1.
   *
   * @return string
   *   The free text 1 value.
   */
  public function getDtlFree1() {
    return $this->dtlFree1;
  }

  /**
   * Set the value of free text 1.
   *
   * @param string $dtl_free_1
   *   The specified  free text 1 value.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlFree1($dtl_free_1) {
    $this->dtlFree1 = $dtl_free_1;

    return $this;
  }

  /**
   * Get the value of free text 2.
   *
   * @return string
   *   The free text 2 value.
   */
  public function getDtlFree2() {
    return $this->dtlFree2;
  }

  /**
   * Set the value of free text 2.
   *
   * @param string $dtl_free_2
   *   The specified  free text 2 value.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlFree2($dtl_free_2) {
    $this->dtlFree2 = $dtl_free_2;

    return $this;
  }

  /**
   * Get the value of free text 3.
   *
   * @return string
   *   The free text 3 value.
   */
  public function getDtlFree3() {
    return $this->dtlFree3;
  }

  /**
   * Set the value of free text 3.
   *
   * @param string $dtl_free_3
   *   The specified  free text 3 value.
   *
   * @return self
   *   This instantiated SBPaymentProduct object.
   */
  public function setDtlFree3($dtl_free_3) {
    $this->dtlFree3 = $dtl_free_3;

    return $this;
  }

}
