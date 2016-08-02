<?php

/**
 * @file
 * Drupal Commerce payment method callbacks for SoftBank Payment Service.
 */

/**
 * Payment method callback: settings form.
 *
 * @see commerce_sbpayment_commerce_payment_method_info()
 * @see commerce_checkout_pane_settings_form
 */
function commerce_sbpayment_settings_form($settings = array()) {
  $form = array();

  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_sbpayment_default_settings();

  $form['mode'] = array(
    '#type' => 'select',
    '#title' => t('Gateway mode'),
    '#description' => t('The mode in which to use the SoftBank Payment Service.'),
    '#options' => _commerce_sbpayment_mode_options(),
    '#default_value' => $settings['mode'],
    '#required' => TRUE,
  );

  $form['pay_methods'] = array(
    '#type' => 'select',
    '#title' => t('Enabled payment methods'),
    '#description' => t('Choose the payment methods available to the customer'),
    '#multiple' => TRUE,
    '#options' => \Commerce\SBPayment\Payment\Methods::$methods,
    '#default_value' => $settings['pay_methods'],
    '#size' => 15,
    '#required' => TRUE,
  );

  $form['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#description' => t('The merchant ID provided to you by SoftBank'),
    '#default_value' => $settings['merchant_id'],
    '#required' => TRUE,
  );

  $form['service_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Service ID'),
    '#description' => t('The service ID provided to you by SoftBank'),
    '#default_value' => $settings['service_id'],
    '#required' => TRUE,
  );

  $form['hash_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Hash Key'),
    '#description' => t('The hash key provided to you by SoftBank'),
    '#default_value' => $settings['hash_key'],
    '#required' => TRUE,
  );

  // TODO: Make this an editable field if required in future.
  $form['pay_type'] = array(
    '#type' => 'value',
    '#default_value' => $settings['pay_type'],
  );

  // TODO: Make this an editable field if required in future.
  $form['auto_charge_type'] = array(
    '#type' => 'value',
    '#default_value' => $settings['auto_charge_type'],
  );

  // TODO: Make this an editable field if required in future.
  $form['service_type'] = array(
    '#type' => 'value',
    '#default_value' => $settings['service_type'],
  );

  return $form;
}

/**
 * Payment method callback: submit form.
 *
 * @see commerce_payment_pane_checkout_form()
 */
function commerce_sbpayment_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array();

  // Add the SoftBank payment methods selector.
  $form['softbank_pay_method'] = array(
    '#type' => 'radios',
    '#title' => t('SoftBank Payment Service Method'),
    '#description' => t('Choose the payment method you want to pay with using SoftBank'),
    '#options' => _commerce_sbpayment_payment_method_options($payment_method['settings']['pay_methods']),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Payment method callback: submit form validation.
 *
 * @see commerce_payment_pane_checkout_form_validate()
 */
function commerce_sbpayment_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  // Check the selected payment method value is present and valid.
  if (empty($pane_values['softbank_pay_method']) || !in_array($pane_values['softbank_pay_method'], $payment_method['settings']['pay_methods'])) {
    return FALSE;
  }
}

/**
 * Payment method callback: submit form submission.
 *
 * @see commerce_payment_pane_checkout_form_submit()
 */
function commerce_sbpayment_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Set the selected SoftBank payment method on the order.
  $order->data['softbank_pay_method'] = $pane_values['softbank_pay_method'];
}

/**
 * Payment method callback: redirect form.
 *
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_sbpayment_redirect_form($form, &$form_state, $order, $payment_method) {
  // TODO: Implement redirect form.
  print __FUNCTION__ . "\r\n";
  print_r(func_get_args());
  die;
}

/**
 * Payment method callback: redirect form return validation.
 *
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_sbpayment_redirect_form_validate($order, $payment_method) {
  // TODO: Implement redirect form validation.
  print __FUNCTION__ . "\r\n";
  print_r(func_get_args());
  die;
}
