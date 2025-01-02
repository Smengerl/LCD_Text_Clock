#ifndef LCD_HELPER_H
#define LCD_HELPER_H

/**
 * @file LCDHelper.hpp
 * @brief Helper class for managing an LCD display.
 */

#include <arduino.h>
#include <LiquidCrystal.h>

/**
 * @enum t_text_alignment
 * @brief Enum for text alignment options.
 */
enum t_text_alignment {
  TEXT_ALIGNMENT_LEFT,    /**< Align text to the left */
  TEXT_ALIGNMENT_RIGHT,   /**< Align text to the right */
  TEXT_ALIGNMENT_CENTERED /**< Center the text */
};

/**
 * @class LCDHelper
 * @brief A helper class for managing an LCD display.
 */
class LCDHelper {
  public:
    /**
     * @brief Constructor for LCDHelper.
     * @param LCD_CHAR_PER_LINE Number of characters per line on the LCD.
     * @param LCD_LINES Number of lines on the LCD.
     * @param CONTRAST_ON_START Initial contrast value.
     * @param BACKLIGHT_ENABLED_ON_START Initial backlight state.
     * @param RS Register select pin.
     * @param EN Enable pin.
     * @param D4 Data pin 4.
     * @param D5 Data pin 5.
     * @param D6 Data pin 6.
     * @param D7 Data pin 7.
     * @param BACKLIGHT_PIN Backlight control pin.
     * @param CONTRAST_PIN Contrast control pin.
     */
    LCDHelper(
      const int LCD_CHAR_PER_LINE, 
      const int LCD_LINES, 
      const int CONTRAST_ON_START, 
      const int BACKLIGHT_ENABLED_ON_START, 
      const int RS, const int EN, const int D4, const int D5, const int D6, const int D7, 
      const int BACKLIGHT_PIN, const int CONTRAST_PIN);

    /**
     * @brief Enable or disable the backlight.
     * @param value True to enable, false to disable.
     */
    void setBacklightEnabled(bool value);

    /**
     * @brief Enable or disable the backlight using a gradual dimming.
     * @param value True to enable, false to disable.
     * @param duration Duration in ms for the dimming.
     */
    void setBacklightEnabled(bool value, uint16_t duration);

    /**
     * @brief Check if the backlight is enabled.
     * @return True if the backlight is enabled, false otherwise.
     */
    bool isBacklightEnabled();

    /**
     * @brief Set the contrast value.
     * @param value Contrast value to set.
     */
    void setContrast(uint8_t value);

    /**
     * @brief Set the contrast using a gradual dimming.
     * @param value Contrast value to set.
     * @param duration Duration in ms for the dimming.
     */
    void setContrast(uint8_t value, uint16_t duration);

    /**
     * @brief Get the current contrast value.
     * @return Current contrast value.
     */
    uint8_t getContrast();

    /**
     * @brief Centers a string within the LCD dimensions padding the remaining space with blanks and optionally wrapping the text across multiple lines.
     * @param dest Destination buffer.
     * @param source Source string.
     * @param wordWrap Enable or disable word wrapping.
     * @param alignment Text alignment.
     */
    void centerStringToDimensions(char *dest, char *source, bool wordWrap, t_text_alignment alignment);

    /**
     * @brief Normalize umlaut characters in a string as C/C++ uses different character encoding for them than the HD44780 controller.
     * @param s String to normalize.
     */
    static void normalizeUmlaute(char *s);

    /**
     * @brief Print a string to the LCD.
     * @param s String to print.
     */
    void print(char *s);

  private:
    /**
     * @brief Helper function to center a string within the LCD dimensions.
     * @param dest Destination buffer.
     * @param source Source string.
     * @param y Y-coordinate.
     * @param beginChar Beginning character index.
     * @param endChar Ending character index.
     * @param alignment Text alignment.
     */
    void centerStringToDimensions_helper(char *dest, char *source, int y, int beginChar, int endChar, t_text_alignment alignment);

    LiquidCrystal lcd; /**< LiquidCrystal object for controlling the LCD */
    uint8_t backlight_pin; /**< Pin for controlling the backlight */
    uint8_t contrast_pin;  /**< Pin for controlling the contrast */

    uint8_t maxWidth, maxHeight;
    uint8_t contrast;

    bool backlightEnabled;
};

#endif // TEXT_ANIMATION_H