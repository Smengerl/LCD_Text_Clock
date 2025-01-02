#ifndef TEXT_HELPER_H
#define TEXT_HELPER_H

/**
 * @file TextHelper.hpp
 * @brief Helper functions for text formatting.
 */

/**
 * @brief Get the name of the month.
 * @param dest Destination buffer to store the month name.
 * @param month Month number (1-12).
 */
void getMonthOfTheYear(char *dest, int month);

/**
 * @brief Get the name of the day of the week.
 * @param dest Destination buffer to store the day name.
 * @param day Day number (0-6, where 0 is Sunday).
 */
void getDayOfTheWeek(char *dest, int day);

/**
 * @brief Get the long format of the hour of the day.
 * @param dest Destination buffer to store the hour in long format.
 * @param hour Hour of the day (0-23).
 */
void getHourOfTheDayLong(char *dest, int hour);

/**
 * @brief Get the short format of the hour of the day.
 * @param dest Destination buffer to store the hour in short format.
 * @param hour Hour of the day (0-23).
 */
void getHourOfTheDay(char *dest, int hour);

#endif // TEXT_HELPER_H