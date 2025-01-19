#include <Arduino.h>

#include <cstring>

#include "TextHelper.hpp"
#include "TextAnimation.hpp"
#include "TextAlignments.hpp"

#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include "ESPDateTime.h"
#include "config.h"
#include "esp_bt.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include "lcd_text_clock_config.hpp"



// PAGES ****************************************************

enum t_page {
#ifdef HAS_PAGE_TIME
  PAGE_TIME,
#endif
#ifdef HAS_PAGE_DATE
  PAGE_DATE,
#endif
  MAX_VALUE
};

#if !defined(HAS_PAGE_TIME)
#if !defined(HAS_PAGE_DATE)
  #error No page defined!
#endif
#endif

t_page page(START_PAGE);




// VARS ****************************************************

// LCD
LiquidCrystal_I2C lcd(I2C_ADDRESS_LCD, LCD_CHAR_PER_LINE, LCD_LINES);

// Cached weight table for faster text layouting
uint8_t* weights = createWeightTable(LCD_CHAR_PER_LINE, LCD_LINES, H_TEXT_ALIGNMENT, V_TEXT_ALIGNMENT);

// Animation helper object
TextAnimation animation(LCD_CHAR_PER_LINE, LCD_LINES);






#define STRING_BUFFER_SIZE LCD_CHAR_PER_LINE * LCD_LINES + 20
char last_stable_text[STRING_BUFFER_SIZE] = "";



uint32_t displayOnUntil = 0;      













void printToLcd(char *s) {
  char buf[STRING_BUFFER_SIZE];
  formatString(buf, s, LCD_CHAR_PER_LINE, LCD_LINES, weights);
  lcd.setCursor(0, 0);
  lcd.printstr(buf);
}



// BACKLIGHT HANDLING *****************************************


#ifdef HAS_BACKLIGHT

  bool backlightEnabled;

  bool isBacklightEnabled() {
    return backlightEnabled;
  }

  void setBacklightEnabled(bool value) {
    if (value) {
      log_d("Backlight enabled");
    } else {
      log_d("Backlight disabled");
    }
    digitalWrite(PIN_LCD_BACKLIGHT, value);
    backlightEnabled = value;
  }

  void setBacklightEnabled(bool value, uint16_t duration) {
    if (backlightEnabled == value) return;

    // "Forecast" final value
    backlightEnabled = value;

    if (duration > 0) {
      if (value) {
        // Dimmen zum Einschalten
        log_d("Dim backlight from off to on in %d ms", duration);
        for (int i = 0; i < duration; i = i + DIMMING_BACKLIGHT_STEPSIZE) {
          float t = ((float)255 * i) / duration;
          analogWrite(PIN_LCD_BACKLIGHT, (int)t);
          vTaskDelay(pdMS_TO_TICKS(DIMMING_BACKLIGHT_STEPSIZE));
        }
      } else {
        // Dimmen zum Ausschalten
        log_d("Dim backlight from on to off in %d ms", duration);
        for (int i = 0; i < duration; i = i + DIMMING_BACKLIGHT_STEPSIZE) {
          float t = 255 - ((float)255 * i) / duration;
          analogWrite(PIN_LCD_BACKLIGHT, (int)t);
          vTaskDelay(pdMS_TO_TICKS(DIMMING_BACKLIGHT_STEPSIZE));
        }
      }
    }
    // Finally use regular "digital" write to ensure the backlight is fully on/off
    setBacklightEnabled(value);
  }



  SemaphoreHandle_t backlightSemaphore = NULL;

  /**
   * Helper function to be used in separate task to run asynchronously
   */
  void fadeBacklightTask(void *pvParameters) {
    if (backlightSemaphore == NULL) {
      backlightSemaphore = xSemaphoreCreateBinary();
    } else {
      if (!xSemaphoreTake(backlightSemaphore, pdMS_TO_TICKS(
        _max(BACKLIGHT_DIMMING_DURATION_ON, BACKLIGHT_DIMMING_DURATION_OFF)))) {

        log_e("Timeout taking semaphore for backlight fading!");
        vTaskDelete(NULL); 
      }
    }

    // The parameter provided when the task was created determines if this is to fade the backlight in or out
    if ((bool) pvParameters ) {
      setBacklightEnabled(true, BACKLIGHT_DIMMING_DURATION_ON);
    } else {
      setBacklightEnabled(false, BACKLIGHT_DIMMING_DURATION_OFF);
    }
    
    xSemaphoreGive(backlightSemaphore);
    vTaskDelete(NULL); // Delete this task when done
  }


#endif










// NTP *****************************************

SemaphoreHandle_t ntpSyncSemaphore = NULL;

void DateTimeTask(void *pvParameters) {
  
  log_i("NTP synching, please wait... ");

  DateTime.setServer(NTP_SERVER);
  DateTime.setTimeZone(TIMEZONE);

  while (!DateTime.begin(NTP_SYNC_TIMEOUT)) {
    log_i("Timeout, retrying\n");
  }
  log_i("Sync NTP done");

  log_d("Date Now is %s\n", DateTime.toISOString().c_str());
  log_d("Timestamp is %ld\n", DateTime.now());

  xSemaphoreGive(ntpSyncSemaphore); // Give the semaphore to signal task completion
  vTaskDelete(NULL); // Delete this task when done
}

void setupDateTime() {
  printToLcd("NTP Sync");

  ntpSyncSemaphore = xSemaphoreCreateBinary(); // Create the semaphore
  xTaskCreate(
    DateTimeTask, // Task function
    "NTPSync",    // Name of the task
    4096,         // Stack size (in words)
    NULL,         // Task input parameter
    1,            // Priority of the task
    NULL          // Task handle
  );

  while (!xSemaphoreTake(ntpSyncSemaphore, pdMS_TO_TICKS(500))) { 
    Serial.print(".");
  }
  vSemaphoreDelete(ntpSyncSemaphore); // Delete the semaphore
}







// HARDWARE INITALIZATION *****************************************

void setupWiFi() {
  //WiFiManager, Local variable sufficient as once its business is done, there is no need to keep it any longer
  WiFiManager wm;

  log_i("WiFi connecting... ");

  char buf[100];
  snprintf(buf, sizeof(buf), "To setup Wifi connect to: %s", wm.getDefaultAPName().c_str());
  printToLcd(buf);

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  wm.setConfigPortalBlocking(false); // asynchroneous mode in order to display the message on the LED matrix
  if (!wm.autoConnect()) { // Try to reconnect to last WiFi or open non password protected AP with default name
    while (!wm.process()) {
      Serial.print(".");
    }
    log_i("WiFi successfully connected (using manual config in AP portal)");
  } else {
    log_i("WiFi successfully connected (using saved credentials)");
  }
}

void disableWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  log_i("WiFi disabled");
}

// Function to disable Bluetooth
void disableBluetooth() {
  if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED) {
      esp_err_t ret = esp_bt_controller_disable();
      if (ret != ESP_OK) {
          log_e("Bluetooth disable failed: %s\n", esp_err_to_name(ret));
          return;
      }
  }

  if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_INITED) {
      esp_err_t ret = esp_bt_controller_deinit();
      if (ret != ESP_OK) {
          log_e("Bluetooth deinit failed: %s\n", esp_err_to_name(ret));
          return;
      }
  }

  log_i("Bluetooth disabled");
}


void setupLCD() {
    // enable LCD backlight
  pinMode(PIN_LCD_BACKLIGHT, OUTPUT);
  #ifdef HAS_BACKLIGHT
    setBacklightEnabled(true);
  #endif

  lcd.init(); 
  lcd.clear();
  lcd.noCursor();
  lcd.noBlink();
  lcd.display();
  lcd.backlight();
}


#ifdef HAS_BUTTONS
  void setupButtons() {
    pinMode(PIN_BUTTON_1, INPUT_PULLDOWN);
    pinMode(PIN_BUTTON_2, INPUT_PULLDOWN);
    pinMode(PIN_BUTTON_3, INPUT_PULLDOWN);
    pinMode(PIN_BUTTON_4, INPUT_PULLDOWN);
  }
#endif

void setup() {
  Serial.begin(115200);

  setupLCD();

  disableBluetooth();

  // Sync with NTP only on initial startup. After that, power off Wifi to save energy
  setupWiFi();
  setupDateTime();
  disableWiFi();

  #ifdef HAS_BUTTONS
    setupButtons();
  #endif

  // pinMode(PIN_ENCODER_SW, INPUT_PULLUP);

  // Attach rotary encoder press to interrupt callback on any change
  //  attachInterrupt(digitalPinToInterrupt(ENCODER_SW), Interrupt, CHANGE);
  // Enable Interrupt handling just in case
  //  interrupts();

  last_stable_text[0] = 0;
}




#ifdef HAS_PAGE_TIME
  /// @brief Generates the text to display for the page "time"
  /// @param s Pointer to the string buffer to write the time to
  /// @param now The date and time to display
  void pageTime(char *s, tm now) {
    switch (now.tm_min) {
      case 0:
      case 1:
      case 2:
        snprintf(s, STRING_BUFFER_SIZE, "%s", getHourOfTheDayLong(now.tm_hour));
        break;

      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
        snprintf(s, STRING_BUFFER_SIZE, "Fünf nach %s", getHourOfTheDay(now.tm_hour));
        break;

      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
        snprintf(s, STRING_BUFFER_SIZE, "Zehn nach %s", getHourOfTheDay(now.tm_hour));
        break;

      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
        snprintf(s, STRING_BUFFER_SIZE, "Viertel nach %s", getHourOfTheDay(now.tm_hour));
        break;

      case 18:
      case 19:
      case 20:
      case 21:
      case 22:
        snprintf(s, STRING_BUFFER_SIZE, "Zehn vor halb %s", getHourOfTheDay((now.tm_hour + 1) % 24));
        break;

      case 23:
      case 24:
      case 25:
      case 26:
      case 27:
        snprintf(s, STRING_BUFFER_SIZE, "Fünf vor halb %s", getHourOfTheDay((now.tm_hour + 1) % 24));
        break;

      case 28:
      case 29:
      case 30:
      case 31:
      case 32:
        snprintf(s, STRING_BUFFER_SIZE, "Halb %s", getHourOfTheDay((now.tm_hour + 1) % 24));
        break;

      case 33:
      case 34:
      case 35:
      case 36:
      case 37:
        snprintf(s, STRING_BUFFER_SIZE, "Fünf nach halb %s", getHourOfTheDay((now.tm_hour + 1) % 24));
        break;

      case 38:
      case 39:
      case 40:
      case 41:
      case 42:
        snprintf(s, STRING_BUFFER_SIZE, "Zehn nach halb %s", getHourOfTheDay((now.tm_hour + 1) % 24));
        break;

      case 43:
      case 44:
      case 45:
      case 46:
      case 47:
        snprintf(s, STRING_BUFFER_SIZE, "Viertel vor %s", getHourOfTheDay((now.tm_hour + 1) % 24));
        break;

      case 48:
      case 49:
      case 50:
      case 51:
      case 52:
        snprintf(s, STRING_BUFFER_SIZE, "Zehn vor %s", getHourOfTheDay((now.tm_hour + 1) % 24));
        break;

      case 53:
      case 54:
      case 55:
      case 56:
      case 57:
        snprintf(s, STRING_BUFFER_SIZE, "Fünf vor %s", getHourOfTheDay((now.tm_hour + 1) % 24));
        break;

      case 58:
      case 59:
        snprintf(s, STRING_BUFFER_SIZE, "%s", getHourOfTheDayLong((now.tm_hour + 1) % 24));
        break;

      default:
        log_e("Inplausible time!");
        snprintf(s, STRING_BUFFER_SIZE, "FEHLER! Unplausible Uhrzeit");
    }
  }
#endif


#ifdef HAS_PAGE_DATE
  /// @brief Generates the text to display for the page "date"
  /// @param s Pointer to the string buffer to write the date to
  /// @param now The date and time to display
  void pageDate(char *s, tm now) {
    const char* month = getMonthOfTheYear(now.tm_mon);
    const char* dayOfTheWeek = getDayOfTheWeek(now.tm_wday);
    snprintf(s, STRING_BUFFER_SIZE, "%s der %d. %s", dayOfTheWeek, now.tm_mday, month);
  }
#endif



#ifdef HAS_BUTTONS
  // Last state of buttons
  bool t1, t2, t3, t4;
#endif

/// @brief Core loop executed repeatedly after initialization
void loop() {
  unsigned long t0 = micros();

  time_t now = DateTime.now();
  struct tm *tm = localtime(&now);

  // Adjust for daylight saving time if necessary
  if (tm->tm_isdst > 0) {
    tm->tm_hour += 1;
    mktime(tm); // Normalize the time structure
  }


  #ifdef HAS_BUTTONS
    bool t1_pos = false;
    if (!digitalRead(PIN_BUTTON_1)) {
      // Button pressed
      if (!t1) {
        t1 = true;
        t1_pos = true;
      }
    } else {
      t1 = false;
    }

    bool t2_pos = false;
    if (!digitalRead(PIN_BUTTON_2)) {
      // Button pressed
      if (!t2) {
        t2 = true;
        t2_pos = true;
      }
    } else {
      t2 = false;
    }

    bool t3_pos = false;
    if (!digitalRead(PIN_BUTTON_3)) {
      // Button pressed
      if (!t3) {
        t3 = true;
        t3_pos = true;
      }
    } else {
      t3 = false;
    }

    bool t4_pos = false;
    if (!digitalRead(PIN_BUTTON_4)) {
      // Button pressed
      if (!t4) {
        t4 = true;
        t4_pos = true;
      }
    } else {
      t4 = false;
    }

    #ifdef HAS_BACKLIGHT
      // Activate backlight for predefined time after button was pressed or turned
      #ifdef BACKLIGHT_ON_TIME
        if (
          t1 || t2 || t3 || t4 // Any key pressed?
          || displayOnUntil == 0  // ... on startup
        ) {
          // Turn on display
          log_i("Trigger display backlight");

          displayOnUntil = now + BACKLIGHT_ON_TIME;
        }

        if (now < displayOnUntil) {
          if (!isBacklightEnabled()) {
            log_i("Turning Backlight on...");
            #ifdef BACKLIGHT_DIMMING_DURATION_ON
              xTaskCreate(
                fadeBacklightTask, // Task function
                "fadeInBacklight", // Name of the task
                4096,              // Stack size (in words)
                (void*)true,       // Task input parameter
                1,                 // Priority of the task
                NULL               // Task handle
              );
            #else
              setBacklightEnabled(true);
            #endif
            log_i("Backlight turned on");
          }
        } else {
          if (isBacklightEnabled()) {
            log_i("Turning Backlight off...");
            #ifdef BACKLIGHT_DIMMING_DURATION_OFF
              xTaskCreate(
                fadeBacklightTask,  // Task function
                "fadeOutBacklight", // Name of the task
                4096,               // Stack size (in words)
                (void*)false,       // Task input parameter
                1,                  // Priority of the task
                NULL                // Task handle
              );
            #else
              setBacklightEnabled(false);
            #endif
            log_i("Backlight turned off");
          }
        }
      #endif
    #endif
    
    if (t1_pos) {
      page = static_cast<t_page>(page + 1);
      if (page == t_page::MAX_VALUE) {
        page = static_cast<t_page>(0);
      }
      animation.start(ANIMATION_NEXT_PAGE_FRAMES, ANIMATION_NEXT_PAGE_TYPE);
      log_i("Next page: %d", page);
    }
    if (t4_pos) {
      page = (page) ? static_cast<t_page>(page - 1) : static_cast<t_page>(t_page::MAX_VALUE - 1);
      animation.start(ANIMATION_PREVIOUS_PAGE_FRAMES, ANIMATION_PREVIOUS_PAGE_TYPE);
      log_i("Previous page: %d", page);
    }
  #endif



  char s1[STRING_BUFFER_SIZE] = "";
  char s2[STRING_BUFFER_SIZE] = "";

  switch (page) {
#ifdef HAS_PAGE_TIME
    case t_page::PAGE_TIME:
      pageTime(s1, *tm);
      break;
#endif
#ifdef HAS_PAGE_DATE
    case t_page::PAGE_DATE:
      pageDate(s1, *tm);
      break;
#endif
#ifdef HAS_PAGE_TEMP
    case t_page::PAGE_TEMP:
      pageTemp(s1);
      break;
#endif
    default:
      log_e("Unknown page");
  }
  formatString(s2, s1, LCD_CHAR_PER_LINE, LCD_LINES, weights);

  // If a change in the text was detected and there is currently no running animation, then start an animation
  if (animation.isEnded() && strcmp(s2, last_stable_text) != 0) {
    animation.start(ANIMATION_TEXT_CHANGE_FRAMES, ANIMATION_TEXT_CHANGE_TYPE);
    log_i("Text change animation started");
  }

  // Render animation frame if still running
  if (!animation.isEnded()) {
    animation.tick();
    animation.apply(s1, last_stable_text, s2);

    lcd.setCursor(0, 0);
    lcd.printstr(s1);

    // if this was last frame and animation has now ended, then store the final state
    if (animation.isEnded()) {
      log_i("Animation endet");
      strcpy(last_stable_text, s1);
//      printStringToLCD(s2);
    } 
  } 


  unsigned long render_duration = (micros()-t0) / 1000;
  log_d("Rendering cycle took %dms (%d FPS)", render_duration, 1000/render_duration);

  // If animation not yet ended, then wait for remaining time of frame duration, else just go on as fast as possible
  if (!animation.isEnded()) {
    if (render_duration > ANIMATION_FRAME_DURATION) {
      log_w("Render cycle to slow, exceeded time");
    } else {
      vTaskDelay(pdMS_TO_TICKS(ANIMATION_FRAME_DURATION - render_duration));
    }

  } else {
    // Save some energy by limiting speed of idle loop
    if (render_duration < IDLE_CYCLE) {
      vTaskDelay(pdMS_TO_TICKS(IDLE_CYCLE - render_duration));
    }    
  }
}
