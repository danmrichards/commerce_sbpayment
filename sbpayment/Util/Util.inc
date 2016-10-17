<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Util\Util.
 */

namespace Commerce\SBPayment\Util;

use DateTime;
use DateTimeZone;
use Commerce\SBPayment\SBPaymentException;

/**
 * A utility class providing common functionaltiy for SoftBank API usage.
 */
class Util {

  /**
   * Payment field mandatory flag.
   *
   * @var string
   */
  const PAYMENT_FIELD_MANDATORY = 'mandatory';

  /**
   * Payment field optional flag.
   *
   * @var string
   */
  const PAYMENT_FIELD_OPTIONAL = 'optional';

  /**
   * Dummy customer code to use in the event of guest checkouts.
   *
   * @var string
   */
  const DUMMY_CUST_CODE = 'reg_sps';

  /**
   * Generate the current time in Japan.
   *
   * @param string $format
   *   A valid PHP date format string.
   *
   * @return string
   *   The formatted timestamp in Japan.
   *
   * @see http://php.net/manual/en/function.date.php
   */
  public static function generateJapanTime($format) {
    $datetime = new DateTime('now', new DateTimeZone('Asia/Tokyo'));
    return $datetime->format($format);
  }

  /**
   * Generates a SoftBank Payment Service hashcode.
   *
   * Intended for usage in payment requests, verification of payment cgi
   * notification and verification of response data.
   *
   * @param array $data
   *   Array of data being used in the request/verification.
   * @param string $hashKey
   *   Hash key assigned by SoftBank for the merchant account.
   *
   * @return string
   *   A valid checksum string based on the data and hash key.
   */
  public static function generateSpsHashcode(array $data, $hashKey) {
    // We can't continue without the hash key.
    if (empty($hashKey)) {
      throw new SBPaymentException('SoftBank hash key is missing');
    }

    // Concatenate the data and append the hash key.
    $checksum_string = implode($data);
    $checksum_string .= $hashKey;

    // We need to convert the encoding to allow for japanese characters.
    $checksum_string = drupal_convert_to_utf8($checksum_string, 'SJIS');

    // Calculate the hash code.
    return strtoupper(sha1($checksum_string));
  }

}
