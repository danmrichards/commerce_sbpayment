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
function commerce_sbpayment_link_type_settings_form($settings = array()) {
  $form = array();

  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_sbpayment_link_type_default_settings();

  $form['mode'] = array(
    '#type' => 'radios',
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

  $form['debug'] = array(
    '#type' => 'radios',
    '#title' => t('Debug'),
    '#description' => t('Output payment request and response information to the system log. This should not be enabled in production.'),
    '#options' => array(
      0 => t('Off'),
      1 => t('On'),
    ),
    '#default_value' => $settings['debug'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Payment method callback: submit form.
 *
 * @see commerce_payment_pane_checkout_form()
 */
function commerce_sbpayment_link_type_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  // Ensure that the payment method has been correctly configured.
  if (!commerce_sbpayment_link_type_validate_settings($payment_method['settings'])) {
    drupal_set_message(t('SoftBank Payment Service: Link Type has not been configured correctly.'), 'error');
    return array();
  }

  $form = array();

  // Add the SoftBank payment methods selector.
  $form['softbank_pay_method'] = array(
    '#type' => 'radios',
    '#title' => t('Payment Method'),
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
function commerce_sbpayment_link_type_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
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
function commerce_sbpayment_link_type_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Set the selected SoftBank payment method on the order.
  if (empty($order->data['softbank'])) {
    $order->data['softbank'] = array();
  }

  $order->data['softbank']['pay_method'] = $pane_values['softbank_pay_method'];
}

/**
 * Payment method callback: redirect form.
 *
 * @see commerce_payment_redirect_pane_checkout_form()
 *
 * @throws \Commerce\SBPayment\SBPaymentException
 */
function commerce_sbpayment_link_type_redirect_form($form, &$form_state, $order, $payment_method) {
  // Ensure that the payment method has been correctly configured.
  if (!commerce_sbpayment_link_type_validate_settings($payment_method['settings'])) {
    drupal_set_message(t('SoftBank Payment Service: Link Type has not been configured correctly.'), 'error');
    return array();
  }

  // Get the payment service object.
  $sbpayment_service_machine_name = str_replace(COMMERCE_SBPAYMENT_PAYMENT_METHOD_BASE . '_', '', $payment_method['method_id']);
  $sbpayment_service = commerce_sbpayment_get_service_object($sbpayment_service_machine_name);

  try {
    // Check the payment service was correctly instantiated.
    if ($sbpayment_service === FALSE) {
      throw new \Commerce\SBPayment\SBPaymentException(t('Service object could not be instantiated for machine name @machine_name', array('@machine_name' => $sbpayment_service_machine_name)));
    }

    // Load the order wrapper.
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    // Get the order total.
    $order_total = $order_wrapper->commerce_order_total->value();

    // Ensure the order total is in JPY.
    if ($order_total['currency_code'] != 'JPY') {
      throw new \Commerce\SBPayment\SBPaymentException(t('Invalid order currency detected. Expected: JPY, Got: @currency_code', array('@currency_code' => $order_total['currency_code'])));
    }

    // Set the gateway mode and encryption hash key.
    $sbpayment_service->setMode($payment_method['settings']['mode']);
    $sbpayment_service->setHashKey($payment_method['settings']['hash_key']);

    // Prepare the data required by the payment service.
    $payment_data = array(
      'pay_method' => $order->data['softbank']['pay_method'],
      'merchant_id' => $payment_method['settings']['merchant_id'],
      'service_id' => $payment_method['settings']['service_id'],
      'cust_code' => $order->uid == 0 ? $sbpayment_service::DUMMY_CUST_CODE : $order->uid,
      'order_id' => _commerce_sbpayment_prepare_order_number($order),
      'item_id' => $order->order_id,
      'item_name' => t('Order @order_number from @site_name', array(
        '@order_number' => $order->order_number,
        '@site_name' => variable_get('site_name', 'Drupal'),
      )),
      // TODO: Get the tax component of the order total. Maybe only needed for
      // bank transfers?
      'tax' => '',
      'amount' => $order_wrapper->commerce_order_total->amount->value(),
      // TODO: Research into this field and make configurable if required.
      'pay_type' => 0,
      // TODO: Research into this field and make configurable if required.
      'service_type' => 0,
      // TODO: Research into this field and make configurable if required.
      'terminal_type' => 0,
      'success_url' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
      'cancel_url' => url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
      'error_url' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
      'pagecon_url' => url(COMMERCE_SBPAYMENT_BASE_URL . '/' . $sbpayment_service_machine_name . '/' . $order->order_id . '/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    );

    // Build an array of SoftBank product objects for the current order.
    $sbpayment_products = array();

    // Loop through the order contents.
    $i = 1;
    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      // We only want to check products.
      if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        // Ensure the product is valid.
        if (isset($line_item_wrapper->commerce_product) && !is_null($line_item_wrapper->commerce_product->value())) {
          // Load the product.
          $product = commerce_product_load($line_item_wrapper->commerce_product->product_id->value());

          // Prepare the product data.
          // TODO: Add the tax component of the product price. Maybe only needed
          // for bank transfers?
          $sbpayment_products[] = new \Commerce\SBPayment\Product\LinkTypeProduct(array(
            'dtl_rowno' => $i,
            'dtl_item_id' => $product->sku,
            'dtl_item_name' => $product->title,
            'dtl_item_count' => (int) $line_item_wrapper->quantity->value(),
            'dtl_amount' => $line_item_wrapper->commerce_unit_price->amount->value(),
          ));

          ++$i;
        }
      }
    }

    // Set the payment fields and product on the service.
    $sbpayment_service->setPaymentData($payment_data);
    $sbpayment_service->setProducts($sbpayment_products);

    // Generate the redirect form for this order and service.
    return commerce_sbpayment_service_order_form($sbpayment_service, $order, (bool) $payment_method['settings']['debug']);
  }
  catch (\Commerce\SBPayment\SBPaymentException $e) {
    watchdog_exception('commerce_sbpayment', $e);
    drupal_set_message(t('A problem occurred when starting the SoftBank Payment Service. Please contact an administrator.'), 'error');

    return array();
  }
}

/**
 * Payment method callback: redirect form back callback.
 */
function commerce_sbpayment_link_type_redirect_form_back($order, $payment_method) {
  // Display a message indicating the customer initiatied cancellation.
  drupal_set_message(t('You have canceled payment at SoftBank but may resume the process here when you are ready.'));

  // Remove the payment information from the order data array.
  unset($order->data['softbank']['pay_method']);

  // Increment the payment attempts counter.
  $order->data['softbank']['payment_attempts'] = !empty($order->data['softbank']['payment_attempts']) ? ++$order->data['softbank']['payment_attempts'] : 1;
}

/**
 * Payment method callback: redirect form return validation.
 *
 * @see commerce_payment_redirect_pane_checkout_form()
 */
function commerce_sbpayment_link_type_redirect_form_validate($order, $payment_method) {
  // Log the debug data if required.
  if ((bool) $payment_method['settings']['debug'] === TRUE) {
    watchdog('commerce_sbpayment', 'Payment Response Debug: <pre>!data</pre>', array(
      '!data' => print_r($_POST, TRUE),
    ));
  }

  // Determine the SoftBank payment service.
  $sbpayment_service_name = str_replace(COMMERCE_SBPAYMENT_PAYMENT_METHOD_BASE . '_', '', $payment_method['method_id']);

  // Get the service object.
  $sbpayment_service = commerce_sbpayment_get_service_object($sbpayment_service_name);

  // Create the repsonse handler.
  $sbpayment_response_class = $sbpayment_service->getResultHandler();

  try {
    // Attempt the create the response object.
    $sbpayment_response = new $sbpayment_response_class();
    $sbpayment_response->setResponseData($_POST);

    // TODO: Handle each payment result accordingly.
    switch ($sbpayment_response->getResultCode()) {

    }

    // Create the drupal transaction.
    //commerce_sbpayment_transaction($payment_method, $sbpayment_response, $order);
  }
  catch (\Commerce\SBPayment\SBPaymentException $e) {
    // Log the error.
    watchdog_exception('commerce_sbpayment', $e);
    drupal_set_message(t('A problem occurred when processing your payment with the SoftBank Payment Service. If the problem persists please contact an administrator.'), 'error');

    // Increment the payment attempts counter.
    $order->data['softbank']['payment_attempts'] = !empty($order->data['softbank']['payment_attempts']) ? ++$order->data['softbank']['payment_attempts'] : 1;

    // Store a record of the failed transaction and send the user back to the
    // previous checkout page.
    commerce_sbpayment_transaction($payment_method, $sbpayment_response, $order);
    commerce_payment_redirect_pane_previous_page($order);

    return FALSE;
  }

  return TRUE;
}

/**
 * Get an array of default settings for the SoftBank Payment Service: Link Type.
 *
 * @return array
 *   Array of default values keyed by field name.
 */
function commerce_sbpayment_link_type_default_settings() {
  return array(
    'mode' => \Commerce\SBPayment\SBPayment::API_MODE_TEST,
    'pay_methods' => array(),
    'merchant_id' => '',
    'service_id' => '',
    'pay_type' => 0,
    'auto_charge_type' => 0,
    'service_type' => 0,
    'hash_key' => '',
    'debug' => 0,
  );
}

/**
 * Validate the settings for the SoftBank Payment Service: Link Type.
 *
 * @param array $settings
 *   Array of settings from an active payment method instance.
 *
 * @return bool
 *   Are the current settings valid?
 */
function commerce_sbpayment_link_type_validate_settings($settings) {
  foreach (commerce_sbpayment_link_type_default_settings() as $settings_field => $default_value) {
    // Check that all non-zero values are present and correct.
    if ($default_value !== 0 && empty($settings[$settings_field])) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Generate a payment redirect form for a SoftBank payment service.
 *
 * @param \Commerce\SBPayment\SBPayment $sbpayment_service
 *   The payment service object with payment data set.
 * @param object $order
 *   A fully loaded commerce order object.
 * @param bool $debug
 *   Should debug information on the payment request be logged?
 *
 * @return array
 *   A renderable Drupal form array.
 */
function commerce_sbpayment_service_order_form(\Commerce\SBPayment\SBPayment $sbpayment_service, $order, $debug = FALSE) {
  $form = array();

  // Set the SoftBank service URL.
  $form['#action'] = $sbpayment_service->getServiceUrl();

  // Get the payment data from the service.
  $payment_data = $sbpayment_service->getPaymentData();

  // Allow other modules to alter the payment data.
  $order_data = clone $order;
  $sbpayment_service_name = (string) $sbpayment_service;
  drupal_alter('commerce_sbpayment_service_order_form_data', $payment_data, $sbpayment_service_name, $order_data);

  // Set the payment data fields.
  foreach ($payment_data as $name => $value) {
    $form[$name] = array('#type' => 'hidden', '#value' => $value);
  }

  // Set the product fields.
  $i = 1;
  foreach ($sbpayment_service->getProducts() as $product) {
    foreach ($product->toArray() as $product_field => $product_data) {
      $form[$product_field . '__' . $i] = array(
        '#type' => 'hidden',
        '#value' => $product_data,
        '#pre_render' => array('commerce_sbpayment_service_product_pre_render'),
      );
    }
    ++$i;
  }

  // Add the request data and limit fields. They have to appear after the
  // product detail fields.
  $form['request_date'] = array(
    '#type' => 'hidden',
    '#value' => $sbpayment_service->getRequestDate(),
  );
  $form['limit_second'] = array(
    '#type' => 'hidden',
    '#value' => $sbpayment_service->getLimitSecond(),
  );

  // Add the checksum value.
  $form['sps_hashcode'] = array(
    '#type' => 'hidden',
    '#value' => $sbpayment_service->getChecksum(),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to SoftBank'),
  );

  $form['#pre_render'][] = 'commerce_sbpayment_service_order_form_pre_render';

  // Log the debug data if required.
  if ($debug === TRUE) {
    watchdog('commerce_sbpayment', 'Payment Request Debug: <pre>!data</pre>', array(
      '!data' => print_r($form, TRUE),
    ));
  }

  return $form;
}

/**
 * Form element pre render callback.
 *
 * Tweak the name attribute of SoftBank product fields.
 *
 * @param array $element
 *   Product form field element.
 *
 * @return array
 *   Tweakedd product form field element.
 */
function commerce_sbpayment_service_product_pre_render($element) {
  // Remove the index from the element name. Workaround for limitation of how
  // Drupal form arrays are built (no duplicate field names).
  $element_name = explode('__', $element['#name']);
  $element['#name'] = $element_name[0];

  return $element;
}

/**
 * Form pre-render callback.
 *
 * Remove Drupal fields from a form (such as "form_token", "form_id" etc).
 * This needs to be done since SoftBank will generate a signature from submitted
 * data and will expect only those fields which described in API. Any other
 * data will lead to wrong signature of payment request.
 */
function commerce_sbpayment_service_order_form_pre_render($form) {
  unset($form['form_build_id']);
  unset($form['form_id']);

  return $form;
}
