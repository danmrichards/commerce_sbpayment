INTRODUCTION
------------
This module provides implements the SoftBank Payment Service for use with Drupal
Commerce. This allows Japanese customers to pay for their orders using any of
the payment methods provided by SoftBank. See below for a list of supported
payment methods.

PAYMENT METHODS
---------------
These are the payment methods (provided by SoftBank) this module supports.

* Credit card payment
* 3D-Secure credit card payment
* Web convenience store payment
* Japan Net Bank
* Pay-easy payment
* General wire transfer
* Cyber Rakuten Edy payment
* Mobile Rakuten Edy payment
* Mobile Suica
* WebMoney payment
* NET CASH payment
* BitCash payment
* JCB Prepaid payment
* G-MONEY payment
* GIC payment
* Docomo Keitai Barai
* Au Kantan Kessai
* S! Matomete Shiharai
* Yahoo! Wallet
* Yahoo! Wallet (Digital Content)
* Rakuten ID Kessai
* RECUIT Kantan Shiharai
* Alipay Cross-border Payment
* Paypal
* Netmile
* SoftBank Matomete Shiharai (A)
* SoftBank Matomete Shiharai (B)
* POINT ONLINE pollet payment
* SAISON Eikyu Fumetsu Point
* OEM PIN
* OEM PIN 2

REQUIREMENTS
------------
This module requires the following modules:
  * Drupal Commerce (https://drupal.org/project/commerce)
  * Autoload (https://drupal.org/project/autoload)

INSTALLATION
------------
  * Enable the Commerce SoftBank Payment Service module.
  * Clear cache.

CONFIGURATION
-------------
To access the configuration UI you must first ensure that the 'Payment UI'
(commerce_payment_ui) module is enabled. Then you can go to the following URL:

admin/commerce/config/payment-methods

From this page simply click the 'Edit' link next to 'SoftBank Payment Service'
and you will be taken to the config form. From here you can enter your SoftBank
payment service credentials, choose the active payment methods and if the
gateway is in test or live mode.

MAINTAINERS
-----------
Current maintainers:
  * Dan Richards - https://www.drupal.org/user/3157375

This project has been sponsored by:
  * LUSH Digital - https://www.drupal.org/node/2529922
    In order for us to become a company of the future and clear cosmetic leader
    we are putting the internet at the heart of our business. It's time for Lush
    to be at the forefront of digital and revolutionise the cosmetics industry.

    Lush Digital enables technology to connect people throughout our community
    and build relationships bringing customer to the heart of our business.
  * FFW - https://www.drupal.org/marketplace/FFW
    FFW is a digital agency built on technology, driven by data, and focused on
    user experience.
