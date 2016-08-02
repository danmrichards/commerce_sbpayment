<?php

/**
 * @file
 * Menu callbacks for Commerce SoftBank Payment Service.
 */

/**
 * Payment notification controller.
 *
 * @param string $sbpayment_service_name
 *   A valid SoftBank payment service machine name.
 * @param object $order
 *   A fully loaded commerce order object.
 * @param string $payment_rediect_key
 *   The payment redirect key as generated during the checkout process.
 *
 * @return string
 *   The response from the SoftBank payment service response handler.
 *
 * @see commerce_sbpayment_services()
 */
function commerce_sbpayment_notification_controller($sbpayment_service_name, $order, $payment_rediect_key) {
  $output = __FUNCTION__ . "\r\n";
  $output .= print_r(func_get_args(), TRUE);

  return $output;
}
