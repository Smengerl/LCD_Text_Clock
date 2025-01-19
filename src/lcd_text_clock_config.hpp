
#include "TextAnimation.hpp"



// ***********************************************************
// FEATURES
// ***********************************************************

// Pushbuttons supported
#define HAS_BUTTONS

// Smart background light
#define HAS_BACKLIGHT

// Pages
#define HAS_PAGE_TIME
#define HAS_PAGE_DATE

// Page to start with
#define START_PAGE PAGE_TIME


// ***********************************************************
// NTP SETTINGS
// ***********************************************************

/** Servername of NTP server to use to get the time from */
#define NTP_SERVER "pool.ntp.org" 

/** Which timezone we are in */
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3" 

/** Max roundtrip time for NTP sync in ms */
#define NTP_SYNC_TIMEOUT 15000




// ***********************************************************
// HARDWARE
// ***********************************************************

// Minimum wait time between two loop cycles
#define IDLE_CYCLE 50

#ifdef HAS_BUTTONS
  // GPIO 36 --> T1
  // GPIO 39 --> T2
  // GPIO 34 --> T3
  // GPIO 35 --> T4
  #define PIN_BUTTON_1 36
  #define PIN_BUTTON_2 39
  #define PIN_BUTTON_3 34
  #define PIN_BUTTON_4 35
#endif

/** Make sure to use a analog output pin to be able to use PWM (3, 5, 6, 9, 10 and 11 on Uno) */
#define PIN_LCD_BACKLIGHT 32

// GPIO 32 --> LCD Backlight
// GPIO 22 --> LCD I2C SCL
// GPIO 21 --> LCD I2C SDA
#define I2C_ADDRESS_LCD 0x27




// ***********************************************************
// LCD
// ***********************************************************

// 20*4 character I2C display 
#define LCD_CHAR_PER_LINE 20
#define LCD_LINES 4


// ***********************************************************
// BACKLIGHT
// ***********************************************************

#ifdef HAS_BACKLIGHT
  /** 
   * Time in s to keep the display backlight on after user input 
   * Do not define if no dynamic backlight control is needed
   */
  #define BACKLIGHT_ON_TIME 10

  /** 
   * Time in ms to dim the backlight to full brightness. 
   * Do not define or set to 0 if no dimming shall happen
   */
  #define BACKLIGHT_DIMMING_DURATION_ON 500

  /** 
   * Time in ms to dim the backlight to off. 
   * Do not define or set to 0 if no dimming shall happen
   */
  #define BACKLIGHT_DIMMING_DURATION_OFF 2000

  /**
   * Steps in ms to fade in/out of backlight
   */
  #define DIMMING_BACKLIGHT_STEPSIZE 10
#endif




// ***********************************************************
// ANIMATIONS 
// ***********************************************************

/**
 * Duration in ms of a single animation frame
 */
#define ANIMATION_FRAME_DURATION 50


// Animation type when going to next page
#define ANIMATION_NEXT_PAGE_TYPE ANIMATION_TYPE_OVERLAY_RIGHT

// Number of frames in animation when going to the next page
#define ANIMATION_NEXT_PAGE_FRAMES 10



// Animation type when going to previous page
#define ANIMATION_PREVIOUS_PAGE_TYPE ANIMATION_TYPE_OVERLAY_LEFT 

// Number of frames in animation when going to the next page
#define ANIMATION_PREVIOUS_PAGE_FRAMES 10



// Animation type when text in page is changed
#define ANIMATION_TEXT_CHANGE_TYPE ANIMATION_TYPE_MATRIX

// Number of frames when text in page is changed
#define ANIMATION_TEXT_CHANGE_FRAMES 20



// ***********************************************************
// TEXT ALIGNMENT
// ***********************************************************

// Textausrichtung im Display
#define H_TEXT_ALIGNMENT TEXT_H_ALIGNMENT_CENTER
#define V_TEXT_ALIGNMENT TEXT_V_ALIGNMENT_MIDDLE
