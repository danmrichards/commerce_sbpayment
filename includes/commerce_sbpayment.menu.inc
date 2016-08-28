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
  // Load the relevant payment file include for this service.
  module_load_include('inc', 'commerce_sbpayment', 'includes/commerce_sbpayment_' . $sbpayment_service_name . '.payment');

  // Load the payment method instance.
  $payment_method_id = sprintf('%s_%s', COMMERCE_SBPAYMENT_PAYMENT_METHOD_BASE, $sbpayment_service_name);
  $payment_method_instance_id = sprintf('%s|commerce_payment_%s', $payment_method_id, $payment_method_id);
  $sbpayment_method = commerce_payment_method_instance_load($payment_method_instance_id);

  // Log the debug data if required.
  if ((bool) $sbpayment_method['settings']['debug'] === TRUE) {
    watchdog('commerce_sbpayment', 'Payment CGI notification debug: <pre>!data</pre>', array(
      '!data' => var_export($_POST, TRUE),
    ));
  }

  // Get the service object.
  $sbpayment_service = commerce_sbpayment_get_service_object($sbpayment_service_name);

  // Create the repsonse handler.
  $sbpayment_response_class = $sbpayment_service->getNotificationHandler();

  try {
    // Attempt the create the response object.
    $response_data = !empty($_POST) ? $_POST : array();
    $sbpayment_response = new $sbpayment_response_class($response_data);

    // Set the hash key.
    $sbpayment_response->setHashKey($sbpayment_method['settings']['hash_key']);

    // Load the order and check it is valid.
    $order = commerce_order_load($order_id);
    if (empty($order)) {
      return $sbpayment_response::RESPONSE_FAIL . ', Invalid order ID';
    }

    // Check the redirect key matches.
    if ($payment_redirect_key != $order->data['payment_redirect_key']) {
      return $sbpayment_response::RESPONSE_FAIL . ', Invalid payment redirect key';
    }

    // Build the product preparation callback name.
    $product_prepare_callback = 'commerce_sbpayment_' . $sbpayment_service_name . '_prepare_order_products';

    // Prepare the order products if the function exists.
    if (function_exists($product_prepare_callback)) {
      // Load the products.
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $sbpayment_products = $product_prepare_callback($order_wrapper);

      // Set the products on the response object.
      $sbpayment_response->setProducts($sbpayment_products);
    }

    // Verify the response data.
    if ($sbpayment_response->verify() === TRUE) {
      // Create the drupal transaction.
      commerce_sbpayment_transaction($sbpayment_method, $sbpayment_response, $order);
    }
  }
  catch (SBPaymentException $e) {
    watchdog_exception('commerce_sbpayment', $e);
  }

  return (string) $sbpayment_response;
}
