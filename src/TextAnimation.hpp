#ifndef TEXT_ANIMATION_H
#define TEXT_ANIMATION_H

/**
 * @file TextAnimation.hpp
 * @brief Defines the TextAnimation class for handling text animations on an LCD display.
 */

/**
 * @enum t_animation_type
 * @brief Enum for different types of text animations.
 */
enum t_animation_type {
  ANIMATION_TYPE_WRITE,        /** Write animation */
  ANIMATION_TYPE_OVERLAY_LEFT, /** animation overlaying old characters by new character from left to right */
  ANIMATION_TYPE_OVERLAY_RIGHT,/** animation overlaying old characters by new character from right to left */
  ANIMATION_TYPE_MATRIX        /** Matrix style animation flipping through different characters */
};

/**
 * @class TextAnimation
 * @brief A class for managing text animations on an LCD display.
 */
class TextAnimation {
  public:
    /**
     * @brief Constructor for TextAnimation. A specific animation type and length is defined by the start command.
     * @param width Width of the display in characters
     * @param height Height of the display in characters
     */
    TextAnimation(uint8_t width, uint8_t height);

    /**
     * @brief Renders the text animation between the beginFrame and endFrame using the current frame counter
     * @param dest String buffer to use.
     * @param beginFrame Text to display at the beginning of the animation.
     * @param endFrame Text to display at the end of the animation.
     */
    void apply(char *dest, char *beginFrame, char *endFrame);

    /**
     * @brief Start the animation with the given number of total frames and the animation type.
     * @param total_frames Total number of frames for the animation.
     * @param animation_Type Type of the animation.
     */
    void start(uint16_t total_frames, t_animation_type animation_Type);

    /**
     * @brief Stop the animation.
     */
    void stop();

    /**
     * @brief Update the animation state and increases the frame counter
     */
    void tick();

    /**
     * @brief Check if the animation has ended.
     * @return True if the animation has ended, false otherwise.
     */
    bool isEnded();

  private:
    t_animation_type animation_Type; /**< Type of the animation */
    uint16_t remaining_animation_frames = 0; /**< Remaining frames for the animation */
    uint16_t total_animation_frames = 0; /**< Total frames for the animation */
    uint8_t maxWidth; /**< Maximum characters per line of the display */
    uint8_t maxHeight; /**< Maximum lines of the display */
};

#endif // TEXT_ANIMATION_H