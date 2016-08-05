<?php

/**
 * @file
 * Contains \Commerce\SBPayment\Util\Util.
 */

namespace Commerce\SBPayment\Util;

use DateTime;
use DateTimeZone;

/**
 * A utility class providing common functionaltiy for SoftBank API usage.
 */
class Util {

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

}
