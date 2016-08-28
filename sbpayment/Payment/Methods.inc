<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Payment\Methods.
 */

namespace Commerce\SBPayment\Payment;

/**
 * Class to provide information about methods supported by SoftBank.
 */
class Methods {

  /**
   * Get the available payment methods supported by SoftBank.
   *
   * @return string[]
   *   Array of payment methods keyed by machine name.
   */
  public static function getPaymentMethods() {
    return array(
      'credit' => 'Credit card payment',
      'credit3d' => '3D-Secure credit card payment',
      'webcvs' => 'Web convenience store payment',
      'jnbank'  => 'Japan Net Bank',
      'payeasy' => 'Pay-easy payment',
      'banktransfer' => 'General wire transfer',
      'cyberedy' => 'Cyber Rakuten Edy payment',
      'mobileedy' => 'Mobile Rakuten Edy payment',
      'suica' => 'Mobile Suica',
      'webmoney' => 'WebMoney payment',
      'netcash' => 'NET CASH payment',
      'bitcash' => 'BitCash payment',
      'prepaid' => 'JCB Prepaid payment',
      'gmoney' => 'G-MONEY payment',
      'gic' => 'GIC payment',
      'docomo' => 'Docomo Keitai Barai',
      'auone' => 'Au Kantan Kessai',
      'softbank' => 'S! Matomete Shiharai',
      'yahoowallet' => 'Yahoo! Wallet',
      'yahoowalletdg' => 'Yahoo! Wallet (Digital Content)',
      'rakuten' => 'Rakuten ID Kessai',
      'recruit' => 'RECUIT Kantan Shiharai',
      'alipay' => 'Alipay Cross-border Payment',
      'paypal' => 'Paypal',
      'netmile' => 'Netmile',
      'mysoftbank' => 'SoftBank Matomete Shiharai (A)',
      'softbank2' => 'SoftBank Matomete Shiharai (B)',
      'pollet' => 'POINT ONLINE pollet payment',
      'saisonpoint' => 'SAISON Eikyu Fumetsu Point',
      'oempin' => 'OEM PIN',
      'oempin2' => 'OEM PIN 2',
    );
  }

  /**
   * Get a single payment method by it's machine name.
   *
   * @return mixed
   *   Array in the format machine_name => Human Name. Or FALSE if no match.
   */
  public static function getPaymentMethodByMachineName($machine_name) {
    $payment_methods = self::getPaymentMethods();
    return !empty($payment_methods[$machine_name]) ? $payment_methods[$machine_name] : FALSE;
  }

}
