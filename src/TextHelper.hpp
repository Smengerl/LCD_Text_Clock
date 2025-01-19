#ifndef TEXT_HELPER_H
#define TEXT_HELPER_H

/**
 * @file TextHelper.hpp
 * @brief Helper functions for text formatting.
 */

/**
 * @brief Get the name of the month.
 * @param month Month number (1-12).
 */
const char* getMonthOfTheYear(int month);

/**
 * @brief Get the name of the day of the week.
 * @param day Day number (0-6, where 0 is Sunday).
 */
const char* getDayOfTheWeek(int day);

/**
 * @brief Get the long format of the hour of the day.
 * @param hour Hour of the day (0-23).
 */
const char* getHourOfTheDayLong(int hour);

/**
 * @brief Get the short format of the hour of the day.
 * @param hour Hour of the day (0-23).
 */
const char* getHourOfTheDay(int hour);

#endif // TEXT_HELPER_H