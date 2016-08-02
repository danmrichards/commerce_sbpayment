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
