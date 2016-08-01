<?php

/**
 * @file
 * Contains the SBPaymentMethods class.
 */

/**
 * Class to provide information about methods supported by SoftBank.
 */
class SBPaymentMethods {

  /**
   * Available payment methods supported by the SoftBank Payment Service.
   *
   * @var string[]
   */
  public static $methods = array(
    'credit' => t('Credit card payment'),
    'credit3d' => t('3D-Secure credit card payment'),
    'webcvs' => t('Web convenience store payment'),
    'jnbank'  => t('Japan Net Bank'),
    'payeasy' => t('Pay-easy payment'),
    'banktransfer' => t('General wire transfer'),
    'cyberedy' => t('Cyber Rakuten Edy payment'),
    'mobileedy' => t('Mobile Rakuten Edy payment'),
    'suica' => t('Mobile Suica'),
    'webmoney' => t('WebMoney payment'),
    'netcash' => t('NET CASH payment'),
    'bitcash' => t('BitCash payment'),
    'prepaid' => t('JCB Prepaid payment'),
    'gmoney' => t('G-MONEY payment'),
    'gic' => t('GIC payment'),
    'docomo' => t('Docomo Keitai Barai'),
    'auone' => t('Au Kantan Kessai'),
    'softbank' => t('S! Matomete Shiharai'),
    'yahoowallet' => t('Yahoo! Wallet'),
    'yahoowalletdg' => t('Yahoo! Wallet (Digital Content)'),
    'rakuten' => t('Rakuten ID Kessai'),
    'recruit' => t('RECUIT Kantan Shiharai'),
    'alipay' => t('Alipay Cross-border Payment'),
    'paypal' => t('Paypal'),
    'netmile' => t('Netmile'),
    'mysoftbank' => t('SoftBank Matomete Shiharai (A)'),
    'softbank2' => t('SoftBank Matomete Shiharai (B)'),
    'pollet' => t('POINT ONLINE pollet payment'),
    'saisonpoint' => t('SAISON Eikyu Fumetsu Point'),
    'oempin' => t('OEM PIN'),
    'oempin2' => t('OEM PIN 2'),
  );

  /**
   * Get a single payment method by it's machine name.
   *
   * @return mixed
   *   Array in the format machine_name => Human Name. Or FALSE if no match.
   */
  public static function getPaymentMethodByMachineName($machine_name) {
    return !empty($this->paymentMethods[$machine_name]) ? $this->paymentMethods[$machine_name] : FALSE;
  }

}
