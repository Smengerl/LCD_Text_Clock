
#ifndef TEXT_ALIGNMENT_H
#define TEXT_ALIGNMENT_H

/**
 * @file TextAlignments.h
 * @brief Helper for aligning text on an LCD.
 */

typedef enum {
  TEXT_H_ALIGNMENT_LEFT,    /**< Align text to the left */
  TEXT_H_ALIGNMENT_RIGHT,   /**< Align text to the right */
  TEXT_H_ALIGNMENT_CENTER,   /**< Center the text */
  TEXT_H_ALIGNMENT_STRETCH
} t_horizontal_text_alignment;

typedef enum {
  TEXT_V_ALIGNMENT_TOP,    /**< Align text to the top */
  TEXT_V_ALIGNMENT_MIDDLE, /**< Align text vertically centered */
  TEXT_V_ALIGNMENT_BOTTOM,  /**< Align the text to the bottom */
  TEXT_V_ALIGNMENT_STRETCH
} t_vertical_text_alignment;



uint8_t* createWeightTable(uint8_t displayWidth, uint8_t displayHeight, t_horizontal_text_alignment h_alignment, t_vertical_text_alignment v_alignment);

void formatString(char *dest, const char *source, uint8_t displayWidth, uint8_t displayHeight, uint8_t* weights);

void formatString(char *dest, const char *source, uint8_t displayWidth, uint8_t displayHeight, t_horizontal_text_alignment h_alignment, t_vertical_text_alignment v_alignment);

#endif // TEXT_ALIGNMENT_H
