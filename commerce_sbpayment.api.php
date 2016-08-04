<?php

/**
 * @file
 * API documentation for the Commerce SoftBank Payment Service module.
 */

/**
 * Alter the list of SoftBank payment method options.
 *
 * This list is used in the payment method submit form.
 *
 * @param string[] $payment_method_options
 *   Array of payment method names keyed by machine name.
 *
 * @see commerce_sbpayment_submit_form()
 * @see commerce_payment_pane_checkout_form()
 */
function hook_commerce_sbpayment_payment_method_options_alter(&$payment_method_options) {
  // Combine mobile based payment options into one.
  $payment_method_options['docomo,auone,softbank2'] = t('Pay by Mobile (Docomo, AU, SoftBank)');
  unset($payment_method_options['docomo']);
  unset($payment_method_options['auone']);
  unset($payment_method_options['softbank2']);
}

/**
 * Alter the payment data before it is submitted to the SoftBank service.
 *
 * @param array $payment_data
 *   Array of payment data from a \Commerce\SBPayment\SBPayment object.
 * @param string $payment_service_name
 *   Machine name of the payment service.
 * @param object $order
 *   A fully loaded Drupal commerce order object.
 */
function hook_commerce_sbpayment_service_order_form_data_alter(&$payment_data, $payment_service_name, $order) {
  if ($payment_service_name == 'link_type') {
    $payment_data['sps_cust_no'] = 'CUST' . $order->uid;
  }
}
