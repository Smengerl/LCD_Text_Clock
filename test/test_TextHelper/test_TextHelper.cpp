#include <unity.h>

#include <Arduino.h>

#include <cstring>


#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include "ESPDateTime.h"
#include "config.h"
#include "esp_bt.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>


#include "TextHelper.hpp"
#include "TextHelper.cpp"



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_getMonthOfTheYear_valid_inputs() {
    TEST_ASSERT_EQUAL_STRING(STRING_MONTHS_00, getMonthOfTheYear(0));
    TEST_ASSERT_EQUAL_STRING(STRING_MONTHS_01, getMonthOfTheYear(1));
    TEST_ASSERT_EQUAL_STRING(STRING_MONTHS_11, getMonthOfTheYear(11));
}

void test_getMonthOfTheYear_invalid_inputs() {
    TEST_ASSERT_NULL(getMonthOfTheYear(-1));  // Invalid month
    TEST_ASSERT_NULL(getMonthOfTheYear(12)); // Invalid month
}



void test_getDayOfTheWeek_valid_inputs() {
    TEST_ASSERT_EQUAL_STRING(STRING_DAY_0, getDayOfTheWeek(0));
    TEST_ASSERT_EQUAL_STRING(STRING_DAY_1, getDayOfTheWeek(1));
    TEST_ASSERT_EQUAL_STRING(STRING_DAY_6, getDayOfTheWeek(6));
}

void test_getDayOfTheWeek_invalid_inputs() {
    TEST_ASSERT_NULL(getDayOfTheWeek(-1)); // Invalid day
    TEST_ASSERT_NULL(getDayOfTheWeek(7));  // Invalid day
}




void test_getHourOfTheDayLong_valid_inputs() {
    TEST_ASSERT_EQUAL_STRING(STRING_HOUR_LONG_00, getHourOfTheDayLong(0));
    TEST_ASSERT_EQUAL_STRING(STRING_HOUR_LONG_01, getHourOfTheDayLong(1));
    TEST_ASSERT_EQUAL_STRING(STRING_HOUR_LONG_12, getHourOfTheDayLong(12));
    TEST_ASSERT_EQUAL_STRING(STRING_HOUR_LONG_23, getHourOfTheDayLong(23));
}

void test_getHourOfTheDayLong_invalid_inputs() {
    TEST_ASSERT_NULL(getHourOfTheDayLong(-1)); // Invalid hour
    TEST_ASSERT_NULL(getHourOfTheDayLong(24)); // Invalid hour
}



void test_getHourOfTheDay_valid_inputs() {
    TEST_ASSERT_EQUAL_STRING(STRING_HOUR_00, getHourOfTheDay(0));
    TEST_ASSERT_EQUAL_STRING(STRING_HOUR_01, getHourOfTheDay(1));
    TEST_ASSERT_EQUAL_STRING(STRING_HOUR_12, getHourOfTheDay(12));
    TEST_ASSERT_EQUAL_STRING(STRING_HOUR_23, getHourOfTheDay(23));
}

void test_getHourOfTheDay_invalid_inputs() {
    TEST_ASSERT_NULL(getHourOfTheDay(-1)); // Invalid hour
    TEST_ASSERT_NULL(getHourOfTheDay(24)); // Invalid hour
}



void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    
    UNITY_BEGIN();

    // Month tests
    RUN_TEST(test_getMonthOfTheYear_valid_inputs);
    RUN_TEST(test_getMonthOfTheYear_invalid_inputs);

    // Day tests
    RUN_TEST(test_getDayOfTheWeek_valid_inputs);
    RUN_TEST(test_getDayOfTheWeek_invalid_inputs);

    // Hour (long) tests
    RUN_TEST(test_getHourOfTheDayLong_valid_inputs);
    RUN_TEST(test_getHourOfTheDayLong_invalid_inputs);

    // Hour (short) tests
    RUN_TEST(test_getHourOfTheDay_valid_inputs);
    RUN_TEST(test_getHourOfTheDay_invalid_inputs);

    UNITY_END();
}

void loop() {
    // Not needed for Unity tests
}