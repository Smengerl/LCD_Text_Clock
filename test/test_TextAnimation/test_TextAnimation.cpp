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


#include "TextAnimation.hpp"
#include "TextAnimation.cpp"


void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


#ifdef CMAKE_TESTS
TEST_CASE("Animation state", "[TextAnimation.hpp]") {
#else
void test_animationState() {
#endif
    TextAnimation animation(20, 4);

    animation.start(4, ANIMATION_TYPE_WRITE);
    TEST_ASSERT_FALSE(animation.isEnded());

    animation.tick();
    TEST_ASSERT_FALSE(animation.isEnded());

    animation.tick();
    TEST_ASSERT_FALSE(animation.isEnded());

    animation.tick();
    TEST_ASSERT_FALSE(animation.isEnded());

    animation.tick();
    TEST_ASSERT_TRUE(animation.isEnded());

    animation.stop();
    TEST_ASSERT_TRUE(animation.isEnded());

    animation.start(4, ANIMATION_TYPE_WRITE);
    TEST_ASSERT_FALSE(animation.isEnded());

    animation.stop();
    TEST_ASSERT_TRUE(animation.isEnded());
}

#ifdef CMAKE_TESTS
TEST_CASE("Animation Type Write", "[TextAnimation.hpp]") {
#else
void test_animationWrite() {
#endif
    TextAnimation animation(5, 2);

    animation.start(4, ANIMATION_TYPE_WRITE);

    char dest[11];
    char s1[] = "AAAAABBBBB";
    char s2[] = "CCCCCDDDDD";

    animation.apply(dest, s1, s2);

    TEST_ASSERT_EQUAL_STRING("AAAAABBBBB", dest);

    animation.tick();
    TEST_ASSERT_EQUAL_STRING("CAAAADBBBB", dest);

    animation.tick();
    TEST_ASSERT_EQUAL_STRING("CCAAADDBBB", dest);

    animation.tick();
    TEST_ASSERT_EQUAL_STRING("CCCAADDDBB", dest);

    animation.tick();
    TEST_ASSERT_EQUAL_STRING("CCCCADDDDB", dest);

    animation.tick();
    TEST_ASSERT_EQUAL_STRING("CCCCCDDDDD", dest);
}



void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    
    UNITY_BEGIN();

    RUN_TEST(test_animationState);
    RUN_TEST(test_animationWrite);

    UNITY_END();
}

void loop() {
    // Not needed for Unity tests
}