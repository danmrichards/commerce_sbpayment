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
  print __FUNCTION__ . "\r\n"
  print_r(func_get_args());
  die;
}

/**
 * Payment method callback: submit form.
 *
 * @see commerce_payment_pane_checkout_form()
 */
function commerce_sbpayment_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  print __FUNCTION__ . "\r\n"
  print_r(func_get_args());
  die;
}

/**
 * Payment method callback: submit form validation.
 *
 * @see commerce_payment_pane_checkout_form_validate()
 */
function commerce_sbpayment_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  print __FUNCTION__ . "\r\n"
  print_r(func_get_args());
  die;
}

/**
 * Payment method callback: submit form submission.
 *
 * @see commerce_payment_pane_checkout_form_submit()
 */
function commerce_sbpayment_submit_form_submit($payment_method, $pane_form, $pane_values, $order) {
  print __FUNCTION__ . "\r\n"
  print_r(func_get_args());
  die;
}

/**
 * Payment method callback: redirect form.
 *
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_sbpayment_form_redirect_form($form, &$form_state, $order, $payment_method) {
  print __FUNCTION__ . "\r\n"
  print_r(func_get_args());
  die;
}

/**
 * Payment method callback: redirect form return validation.
 *
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_sbpayment_redirect_form_validate($order, $payment_method) {
  print __FUNCTION__ . "\r\n"
  print_r(func_get_args());
  die;
}
