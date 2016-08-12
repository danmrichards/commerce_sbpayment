<?php

/**
 * @file
 * Menu callbacks for Commerce SoftBank Payment Service.
 */

use Commerce\SBPayment\SBPaymentException;

/**
 * Payment notification controller.
 *
 * @param string $sbpayment_service_name
 *   A valid SoftBank payment service machine name.
 * @param int $order_id
 *   A Drupal commerce order ID.
 * @param string $payment_redirect_key
 *   The payment redirect key as generated during the checkout process.
 *
 * @return string
 *   The response from the SoftBank payment service response handler.
 *
 * @see commerce_sbpayment_services()
 */
function commerce_sbpayment_notification_controller($sbpayment_service_name, $order_id, $payment_redirect_key) {
  // Load the payment method instance.
  $payment_method_id = sprintf('%s_%s', COMMERCE_SBPAYMENT_PAYMENT_METHOD_BASE, $sbpayment_service_name);
  $payment_method_instance_id = sprintf('%s|commerce_payment_%s', $payment_method_id, $payment_method_id);
  $sbpayment_method = commerce_payment_method_instance_load($payment_method_instance_id);

  // Log the debug data if required.
  if ((bool) $sbpayment_method['settings']['debug'] === TRUE) {
    watchdog('commerce_sbpayment', 'Payment CGI notification debug: <pre>!data</pre>', array(
      '!data' => print_r($_POST, TRUE),
    ));
  }

  // Get the service object.
  $sbpayment_service = commerce_sbpayment_get_service_object($sbpayment_service_name);

  // Create the repsonse handler.
  $sbpayment_response_class = $sbpayment_service->getNotificationHandler();

  try {
    // Attempt the create the response object.
    $sbpayment_response = new $sbpayment_response_class();

    // Load the order and check it is valid.
    $order = commerce_order_load($order_id);
    if (empty($order)) {
      return $sbpayment_response::RESPONSE_FAIL . ', Invalid order ID';
    }

    // Check the redirect key matches.
    if ($payment_redirect_key != $order->data['payment_redirect_key']) {
      return $sbpayment_response::RESPONSE_FAIL . ', Invalid payment redirect key';
    }

    $sbpayment_response->setResponseData($_POST);

    // Create the drupal transaction.
    commerce_sbpayment_transaction($sbpayment_method, $sbpayment_response, $order);
  }
  catch (SBPaymentException $e) {
    watchdog_exception('commerce_sbpayment', $e);
    drupal_set_message(t('A problem occurred when starting the SoftBank Payment Service. Please contact an administrator.'), 'error');
  }

  return (string) $sbpayment_response;
}
