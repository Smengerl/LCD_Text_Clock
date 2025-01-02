#include <avr/pgmspace.h>



const char STRING_HOUR_00[] PROGMEM = "zwölf";
const char STRING_HOUR_01[] PROGMEM = "eins";
const char STRING_HOUR_02[] PROGMEM = "zwei";
const char STRING_HOUR_03[] PROGMEM = "drei";
const char STRING_HOUR_04[] PROGMEM = "vier";
const char STRING_HOUR_05[] PROGMEM = "fünf";
const char STRING_HOUR_06[] PROGMEM = "sechs";
const char STRING_HOUR_07[] PROGMEM = "sieben";
const char STRING_HOUR_08[] PROGMEM = "acht";
const char STRING_HOUR_09[] PROGMEM = "neun";
const char STRING_HOUR_10[] PROGMEM = "zehn";
const char STRING_HOUR_11[] PROGMEM = "elf";
const char STRING_HOUR_12[] PROGMEM = "zwölf";
const char STRING_HOUR_13[] PROGMEM = "eins";
const char STRING_HOUR_14[] PROGMEM = "zwei";
const char STRING_HOUR_15[] PROGMEM = "drei";
const char STRING_HOUR_16[] PROGMEM = "vier";
const char STRING_HOUR_17[] PROGMEM = "fünf";
const char STRING_HOUR_18[] PROGMEM = "sechs";
const char STRING_HOUR_19[] PROGMEM = "sieben";
const char STRING_HOUR_20[] PROGMEM = "acht";
const char STRING_HOUR_21[] PROGMEM = "neun";
const char STRING_HOUR_22[] PROGMEM = "zehn";
const char STRING_HOUR_23[] PROGMEM = "elf";

void getHourOfTheDay(char *dest, int hour) {
  const static char *const HOURS_OF_THE_DAY[] PROGMEM = {
    STRING_HOUR_00,
    STRING_HOUR_01,
    STRING_HOUR_02,
    STRING_HOUR_03,
    STRING_HOUR_04,
    STRING_HOUR_05,
    STRING_HOUR_06,
    STRING_HOUR_07,
    STRING_HOUR_08,
    STRING_HOUR_09,
    STRING_HOUR_10,
    STRING_HOUR_11,
    STRING_HOUR_12,
    STRING_HOUR_13,
    STRING_HOUR_14,
    STRING_HOUR_15,
    STRING_HOUR_16,
    STRING_HOUR_17,
    STRING_HOUR_18,
    STRING_HOUR_19,
    STRING_HOUR_20,
    STRING_HOUR_21,
    STRING_HOUR_22,
    STRING_HOUR_23
  };
  if (hour >= 0 && hour < 24) {
    strcpy_P(dest, (char *)pgm_read_ptr(&(HOURS_OF_THE_DAY[hour])));
  } else
    *dest = 0;
}


const char STRING_HOUR_LONG_00[] PROGMEM = "Mitternacht";
const char STRING_HOUR_LONG_01[] PROGMEM = "ein Uhr nachts";
const char STRING_HOUR_LONG_02[] PROGMEM = "zwei Uhr nachts";
const char STRING_HOUR_LONG_03[] PROGMEM = "drei Uhr nachts";
const char STRING_HOUR_LONG_04[] PROGMEM = "vier Uhr morgens";
const char STRING_HOUR_LONG_05[] PROGMEM = "fünf Uhr morgens";
const char STRING_HOUR_LONG_06[] PROGMEM = "sechs Uhr morgens";
const char STRING_HOUR_LONG_07[] PROGMEM = "sieben Uhr morgens";
const char STRING_HOUR_LONG_08[] PROGMEM = "acht Uhr morgens";
const char STRING_HOUR_LONG_09[] PROGMEM = "neun Uhr morgens";
const char STRING_HOUR_LONG_10[] PROGMEM = "zehn Uhr vormittags";
const char STRING_HOUR_LONG_11[] PROGMEM = "elf Uhr vormittags";
const char STRING_HOUR_LONG_12[] PROGMEM = "Mittag";
const char STRING_HOUR_LONG_13[] PROGMEM = "eins Uhr mittags";
const char STRING_HOUR_LONG_14[] PROGMEM = "zwei Uhr nachmittags";
const char STRING_HOUR_LONG_15[] PROGMEM = "drei Uhr nachmittags";
const char STRING_HOUR_LONG_16[] PROGMEM = "vier Uhr nachmittags";
const char STRING_HOUR_LONG_17[] PROGMEM = "fünf Uhr nachmittags";
const char STRING_HOUR_LONG_18[] PROGMEM = "sechs Uhr abends";
const char STRING_HOUR_LONG_19[] PROGMEM = "sieben Uhr abends";
const char STRING_HOUR_LONG_20[] PROGMEM = "acht Uhr abends";
const char STRING_HOUR_LONG_21[] PROGMEM = "neun Uhr abends";
const char STRING_HOUR_LONG_22[] PROGMEM = "zehn Uhr abends";
const char STRING_HOUR_LONG_23[] PROGMEM = "elf Uhr nachts";

void getHourOfTheDayLong(char *dest, int hour) {
  const static char *const HOURS_OF_THE_DAY_LONG[] PROGMEM = {
    STRING_HOUR_LONG_00,
    STRING_HOUR_LONG_01,
    STRING_HOUR_LONG_02,
    STRING_HOUR_LONG_03,
    STRING_HOUR_LONG_04,
    STRING_HOUR_LONG_05,
    STRING_HOUR_LONG_06,
    STRING_HOUR_LONG_07,
    STRING_HOUR_LONG_08,
    STRING_HOUR_LONG_09,
    STRING_HOUR_LONG_10,
    STRING_HOUR_LONG_11,
    STRING_HOUR_LONG_12,
    STRING_HOUR_LONG_13,
    STRING_HOUR_LONG_14,
    STRING_HOUR_LONG_15,
    STRING_HOUR_LONG_16,
    STRING_HOUR_LONG_17,
    STRING_HOUR_LONG_18,
    STRING_HOUR_LONG_19,
    STRING_HOUR_LONG_20,
    STRING_HOUR_LONG_21,
    STRING_HOUR_LONG_22,
    STRING_HOUR_LONG_23
  };
  if (hour >= 0 && hour < 24) {
    strcpy_P(dest, (char *)pgm_read_ptr(&(HOURS_OF_THE_DAY_LONG[hour])));
  } else
    *dest = 0;
}




const char STRING_MONTHS_00[] PROGMEM = "Januar";
const char STRING_MONTHS_01[] PROGMEM = "Februar";
const char STRING_MONTHS_02[] PROGMEM = "März";
const char STRING_MONTHS_03[] PROGMEM = "April";
const char STRING_MONTHS_04[] PROGMEM = "Mai";
const char STRING_MONTHS_05[] PROGMEM = "Juni";
const char STRING_MONTHS_06[] PROGMEM = "Juli";
const char STRING_MONTHS_07[] PROGMEM = "August";
const char STRING_MONTHS_08[] PROGMEM = "September";
const char STRING_MONTHS_09[] PROGMEM = "Oktober";
const char STRING_MONTHS_10[] PROGMEM = "November";
const char STRING_MONTHS_11[] PROGMEM = "Dezember";

void getMonthOfTheYear(char *dest, int month) {
  const static char *const MONTHS_OF_THE_YEAR[] PROGMEM = {
    STRING_MONTHS_00,
    STRING_MONTHS_01,
    STRING_MONTHS_02,
    STRING_MONTHS_03,
    STRING_MONTHS_04,
    STRING_MONTHS_05,
    STRING_MONTHS_06,
    STRING_MONTHS_07,
    STRING_MONTHS_08,
    STRING_MONTHS_09,
    STRING_MONTHS_10,
    STRING_MONTHS_11
  };
  if (month >= 0 && month < 12) {
    strcpy_P(dest, (char *)pgm_read_ptr(&(MONTHS_OF_THE_YEAR[month])));
  } else
    *dest = 0;
}




const char STRING_DAY_0[] PROGMEM = "Sonntag";
const char STRING_DAY_1[] PROGMEM = "Montag";
const char STRING_DAY_2[] PROGMEM = "Dienstag";
const char STRING_DAY_3[] PROGMEM = "Mittwoch";
const char STRING_DAY_4[] PROGMEM = "Donnerstag";
const char STRING_DAY_5[] PROGMEM = "Freitag";
const char STRING_DAY_6[] PROGMEM = "Samstag";


void getDayOfTheWeek(char *dest, int day) {
  const static char *const DAY_OF_THE_WEEK[] PROGMEM = {
    STRING_DAY_0,
    STRING_DAY_1,
    STRING_DAY_2,
    STRING_DAY_3,
    STRING_DAY_4,
    STRING_DAY_5,
    STRING_DAY_6
  };
  if (day >= 0 && day < 7) {
    strcpy_P(dest, (char *)pgm_read_ptr(&(DAY_OF_THE_WEEK[day])));
  } else
    *dest = 0;
}

