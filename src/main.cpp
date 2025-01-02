#define HAS_ENCODER
#define HAS_PAGE_TIME
#define HAS_PAGE_DATE
#define HAS_PAGE_TEMP


#include <Arduino.h>
// Rotary encoder
#ifdef HAS_ENCODER
#include <Encoder.h>
#endif
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <SPI.h>

#include "TextHelper.hpp"
#include "TextAnimation.hpp"
#include "LCDHelper.hpp"



// CONSTANTS TO ADAPT BEHAVIOR ****************************************************

enum t_page {
#ifdef HAS_PAGE_TIME
  PAGE_TIME,
#endif
#ifdef HAS_PAGE_DATE
  PAGE_DATE,
#endif
#ifdef HAS_PAGE_TEMP
  PAGE_TEMP,
#endif
  MAX_VALUE
};


/**
 * Contrast when display is active
 */
#define CONTRAST_DISPLAY_ACTIVE 100

/**
 * Contrast when display is inactive
 */
#define CONTRAST_DISPLAY_INACTIVE 200


/** 
 * Time in s to keep the display backlight on after user input 
 * Do not define if no dynamic backlight control is needed
 */
#define BACKLIGHT_ON_TIME 3


/** 
 * Time in ms to dim the contrast to on. 
 * Do not define or set to 0 if no dimming shall happen
 */
#define CONTRAST_DIMMING_DURATION_ON 0

/** 
 * Time in ms to dim the contrast to off. 
 * Do not define or set to 0 if no dimming shall happen
 */
#define CONTRAST_DIMMING_DURATION_OFF 1000

/** 
 * Time in ms to dim the backlight to full brightness. 
 * Do not define or set to 0 if no dimming shall happen
 */
#define BACKLIGHT_DIMMING_DURATION_ON 0

/** 
 * Time in ms to dim the backlight to off. 
 * Do not define or set to 0 if no dimming shall happen
 */
#define BACKLIGHT_DIMMING_DURATION_OFF 1000




// Animationsframes wenn Seite umgeschaltet wird
#define ANIMATION_DELAY_SWITCH_PAGE_FRAMES 20

// Animationstyp wenn Seite umgeschaltet wird
#define ANIMATION_DELAY_SWITCH_PAGE_CW_TYPE ANIMATION_TYPE_OVERLAY_RIGHT
// Animationstyp wenn Seite umgeschaltet wird
#define ANIMATION_DELAY_SWITCH_PAGE_CCW_TYPE ANIMATION_TYPE_OVERLAY_LEFT 

// Animationsframes wenn Werte manuell verstellt werden
#define ANIMATION_DELAY_CHANGE_VALUE_FRAMES 10
// Animationstyp wenn Werte manuell verstellt werden
#define ANIMATION_DELAY_CHANGE_VALUE_TYPE ANIMATION_TYPE_MATRIX


// Animationsframes wenn Text sich ändert innerhalb einer Seite
#define ANIMATION_TEXT_CHANGE_FRAMES 20
// Animationstyp wenn Text sich ändert innerhalb einer Seite
#define ANIMATION_TEXT_CHANGE_TYPE ANIMATION_TYPE_MATRIX

// Startseite
#define START_PAGE PAGE_TIME

// Text wird umgebrochen im Display am Wordende
#define TEXT_WORD_WRAP true

// Textausrichtung im Display
#define TEXT_ALIGNMENT TEXT_ALIGNMENT_CENTERED


#define ANIMATION_FRAME_LENGTH 5



// REAL TIME CLOCK **************************************************************
RTC_DS3231 rtc;


// ROTARY ENCODER ***************************************************************


#define PIN_ENCODER_CLK 9
#define PIN_ENCODER_DT 8
#define PIN_ENCODER_SW 10
long encoderPos = -999;

Encoder meinEncoder(PIN_ENCODER_DT, PIN_ENCODER_CLK);


// LCD **************************************************************************

#define PIN_LCD_RS 12
#define PIN_LCD_EN 11
#define PIN_LCD_D4 5
#define PIN_LCD_D5 4
#define PIN_LCD_D6 3
#define PIN_LCD_D7 2

/** Make sure to use a analog output pin to be able to use PWM (3, 5, 6, 9, 10 and 11 on Uno) */
#define PIN_LCD_BACKLIGHT 7 
#define PIN_LCD_CONTRAST 6

#define LCD_CHAR_PER_LINE 16
#define LCD_LINES 2

LCDHelper lcd(LCD_CHAR_PER_LINE, LCD_LINES, CONTRAST_DISPLAY_ACTIVE, true, PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7, PIN_LCD_BACKLIGHT, PIN_LCD_CONTRAST);



// ANIMATON *******************************************************************************************

TextAnimation animation(LCD_CHAR_PER_LINE, LCD_LINES);



// *******************************************************************************************

#define STRING_BUFFER_SIZE LCD_CHAR_PER_LINE * LCD_LINES + 20

t_page page(START_PAGE);

uint32_t displayOnUntil = 0;         // interner Timeout Zähler, wann Display wieder abgeschaltet werden soll

char last_stable_text[STRING_BUFFER_SIZE] = "";




/// @brief Initial setup
void setup() {
  Serial.begin(115200);
  Serial.print(F("Waiting for RTC to connect... "));

#ifndef ESP8266
  while (!Serial)
    ;  // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    Serial.flush();
    while (1) delay(10);
  }
  Serial.println(F("RTC connected"));

  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, let's set the time!"));
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  DateTime now = rtc.now();
  Serial.print(F("RTC already initialized: "));
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();


  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

#ifdef HAS_ENCODER
  pinMode(PIN_ENCODER_SW, INPUT_PULLUP);

  // Attach rotary encoder press to interrupt callback on any change
  //  attachInterrupt(digitalPinToInterrupt(ENCODER_SW), Interrupt, CHANGE);
  // Enable Interrupt handling just in case
  //  interrupts();
#endif

  last_stable_text[0] = 0;
}





#ifdef HAS_PAGE_TIME
/// @brief Generates the text to display for the page "time"
/// @param s Pointer to the string buffer to write the time to
/// @param now The date and time to display
void pageTime(char *s, DateTime now) {

  char buffer[50];

  switch (now.minute()) {
    case 0:
    case 1:
    case 2:
      getHourOfTheDayLong(buffer, now.hour());
      snprintf(s, STRING_BUFFER_SIZE, "%s", buffer);
      break;

    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      getHourOfTheDay(buffer, now.hour());
      snprintf(s, STRING_BUFFER_SIZE, "Fünf nach %s", buffer);
      break;

    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      getHourOfTheDay(buffer, now.hour());
      snprintf(s, STRING_BUFFER_SIZE, "Zehn nach %s", buffer);
      break;

    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
      getHourOfTheDay(buffer, now.hour());
      snprintf(s, STRING_BUFFER_SIZE, "Viertel nach %s", buffer);
      break;

    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
      getHourOfTheDay(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "Zehn vor halb %s", buffer);
      break;

    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
      getHourOfTheDay(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "Fünf vor halb %s", buffer);
      break;

    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
      getHourOfTheDay(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "Halb %s", buffer);
      break;

    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
      getHourOfTheDay(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "Fünf nach halb %s", buffer);
      break;

    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
      getHourOfTheDay(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "Zehn nach halb %s", buffer);
      break;

    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
      getHourOfTheDay(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "Viertel vor %s", buffer);
      break;

    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
      getHourOfTheDay(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "Zehn vor %s", buffer);
      break;

    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
      getHourOfTheDay(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "Fünf vor %s", buffer);
      break;

    case 58:
    case 59:
      getHourOfTheDayLong(buffer, (now.hour() + 1) % 24);
      snprintf(s, STRING_BUFFER_SIZE, "%s", buffer);
      break;

    default:
      Serial.println("FEHLER!");
      snprintf(s, STRING_BUFFER_SIZE, "FEHLER! Unplausible Uhrzeit");
  }
}
#endif


#ifdef HAS_PAGE_DATE
/// @brief Generates the text to display for the page "date"
/// @param s Pointer to the string buffer to write the date to
/// @param now The date and time to display
void pageDate(char *s, DateTime now) {

  char month[50];
  getMonthOfTheYear(month, now.month() - 1);

  char dayOfTheWeek[50];
  getDayOfTheWeek(dayOfTheWeek, now.dayOfTheWeek());

  snprintf(s, STRING_BUFFER_SIZE, "%s der %d. %s", dayOfTheWeek, now.day(), month);
}
#endif


#ifdef HAS_PAGE_TEMP
/// @brief Generates the text to display for the page "temp"
/// @param s Pointer to the string buffer to write the temp to
void pageTemp(char *s) {
  float f = rtc.getTemperature();
  int val_int = (int)f;
  int val_fra = (int)((f - val_int) * 10);
  snprintf(s, STRING_BUFFER_SIZE, "%d.%d Grad Celsius", val_int, val_fra);
}
#endif





/// @brief Core loop executed repeatedly after initialization
void loop() {
  DateTime now = rtc.now();

  #ifdef HAS_ENCODER
    long p = meinEncoder.read() / 4;
    bool encoder_pressed = digitalRead(PIN_ENCODER_SW) == LOW;

    // Activate backlight for predefined time after button was pressed or turned
    #ifdef BACKLIGHT_ON_TIME
      if (
        p != encoderPos         // ...if encoder rotated
        || displayOnUntil == 0  // ... on startup
        || encoder_pressed      // ... if encoder pressed
      ) {
        // Turn on display
        Serial.println(F("Trigger display backlight"));
        displayOnUntil = now.unixtime() + BACKLIGHT_ON_TIME;
      }

      if (now.unixtime() < displayOnUntil) {
        if (!lcd.isBacklightEnabled()) {
          #ifdef BACKLIGHT_DIMMING_DURATION_ON
            lcd.setBacklightEnabled(true, BACKLIGHT_DIMMING_DURATION_ON);
          #else
            lcd.setBacklightEnabled(true);
          #endif
          #ifdef CONTRAST_DIMMING_DURATION_ON
            lcd.setContrast(CONTRAST_DISPLAY_ACTIVE, CONTRAST_DIMMING_DURATION_ON);
          #endif
          Serial.println(F("Backlight turned on"));
        }
      } else {
        if (lcd.isBacklightEnabled()) {
          #ifdef CONTRAST_DIMMING_DURATION_OFF
            lcd.setContrast(CONTRAST_DISPLAY_INACTIVE, CONTRAST_DIMMING_DURATION_OFF);
          #endif
          #ifdef BACKLIGHT_DIMMING_DURATION_OFF
            lcd.setBacklightEnabled(false, BACKLIGHT_DIMMING_DURATION_OFF);
          #else
            lcd.setBacklightEnabled(false);
          #endif
          Serial.println(F("Backlight turned off"));
        }
      }
    #endif

    // Has encoder been rotated?
    if (p != encoderPos && encoder_pressed) {
      switch (page) {
        #ifdef HAS_PAGE_TIME
          case t_page::PAGE_TIME:
            // if encoder is pressed and turned, then adjust time or date
            if (p < encoderPos) {
              Serial.println(F("Adjust time +5min"));
              DateTime newTime(now + TimeSpan(300));
              rtc.adjust(newTime);
            } else {
              Serial.println(F("Adjust time -5min"));
              DateTime newTime(now - TimeSpan(300));
              rtc.adjust(newTime);
            }
            break;
        #endif
        #ifdef HAS_PAGE_DATE
          case t_page::PAGE_DATE:
            // if encoder is pressed and turned, then adjust time or date
            if (p < encoderPos) {
              Serial.println(F("Adjust time +1day"));
              DateTime newTime(now + TimeSpan(1, 0, 0, 0));
              rtc.adjust(newTime);
            } else {
              Serial.println(F("Adjust time -1day"));
              DateTime newTime(now - TimeSpan(1, 0, 0, 0));
              rtc.adjust(newTime);
            }
            break;
        #endif
        #ifdef HAS_PAGE_TEMP
          case t_page::PAGE_TEMP:
            Serial.println(F("Cannot adjust temperature"));
            break;
        #endif
        default:
          Serial.println(F("Unknown page"));
      }
      now = rtc.now();
      animation.start(ANIMATION_DELAY_CHANGE_VALUE_FRAMES, ANIMATION_DELAY_CHANGE_VALUE_TYPE);
    }

    if (p != encoderPos && !encoder_pressed && animation.isEnded()) {
      // In case encoder is just turned without being pressed, then flip pages
      if (p < encoderPos) {
        page = static_cast<t_page>(page + 1);
        if (page == t_page::MAX_VALUE) {
          page = static_cast<t_page>(0);
        }
        animation.start(ANIMATION_DELAY_SWITCH_PAGE_FRAMES, ANIMATION_DELAY_SWITCH_PAGE_CW_TYPE);
        Serial.println(F("Next page flip animation started"));
      } else if (p > encoderPos) {
        page = (page) ? static_cast<t_page>(page - 1) : static_cast<t_page>(t_page::MAX_VALUE - 1);
        animation.start(ANIMATION_DELAY_SWITCH_PAGE_FRAMES, ANIMATION_DELAY_SWITCH_PAGE_CCW_TYPE);
        Serial.println(F("Previous page flip animation started"));
      }
    }
    encoderPos = p;
  #endif





  char s1[STRING_BUFFER_SIZE] = "";
  char s2[STRING_BUFFER_SIZE] = "";

  switch (page) {
#ifdef HAS_PAGE_TIME
    case t_page::PAGE_TIME:
      pageTime(s1, now);
      break;
#endif
#ifdef HAS_PAGE_DATE
    case t_page::PAGE_DATE:
      pageDate(s1, now);
      break;
#endif
#ifdef HAS_PAGE_TEMP
    case t_page::PAGE_TEMP:
      pageTemp(s1);
      break;
#endif
    default:
      Serial.println(F("Unknown page"));
  }

  lcd.normalizeUmlaute(s1);
  lcd.centerStringToDimensions(s2, s1, TEXT_WORD_WRAP, TEXT_ALIGNMENT);


  // If a change in the text was detected and there is currently no running animation, then start an animation
  if (animation.isEnded() && strcmp(s2, last_stable_text) != 0) {
    if (!encoder_pressed) {
      animation.start(ANIMATION_TEXT_CHANGE_FRAMES, ANIMATION_TEXT_CHANGE_TYPE);
      Serial.println(F("Text change animation started!"));
    }
  }

  // Render animation frame if still running
  if (!animation.isEnded()) {
    animation.tick();
    animation.apply(s1, last_stable_text, s2);
/*
    Serial.print("Animate: \"");
    Serial.print(last_stable_text);
    Serial.print("\"--> \"");
    Serial.print(s2);
    Serial.print("\" = \"");
    Serial.print(s1);
    Serial.println("\"");
  */  
    lcd.print(s1);

    // if this was last frame and animation has now ended, then store the final state
    if (animation.isEnded()) {
      Serial.println(F("Animation endet"));
      strcpy(last_stable_text, s1);
//      printStringToLCD(s2);
    } else {
      delay(ANIMATION_FRAME_LENGTH);
    }
  }

}
