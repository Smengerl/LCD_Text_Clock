// #include <avr/pgmspace.h>
#include <Arduino.h>


const char STRING_HOUR_00[] = "zwölf";
const char STRING_HOUR_01[] = "eins";
const char STRING_HOUR_02[] = "zwei";
const char STRING_HOUR_03[] = "drei";
const char STRING_HOUR_04[] = "vier";
const char STRING_HOUR_05[] = "fünf";
const char STRING_HOUR_06[] = "sechs";
const char STRING_HOUR_07[] = "sieben";
const char STRING_HOUR_08[] = "acht";
const char STRING_HOUR_09[] = "neun";
const char STRING_HOUR_10[] = "zehn";
const char STRING_HOUR_11[] = "elf";
const char STRING_HOUR_12[] = "zwölf";
const char STRING_HOUR_13[] = "eins";
const char STRING_HOUR_14[] = "zwei";
const char STRING_HOUR_15[] = "drei";
const char STRING_HOUR_16[] = "vier";
const char STRING_HOUR_17[] = "fünf";
const char STRING_HOUR_18[] = "sechs";
const char STRING_HOUR_19[] = "sieben";
const char STRING_HOUR_20[] = "acht";
const char STRING_HOUR_21[] = "neun";
const char STRING_HOUR_22[] = "zehn";
const char STRING_HOUR_23[] = "elf";


const char* getHourOfTheDay(int hour) {
  const static char *HOURS_OF_THE_DAY[] = {
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
    return HOURS_OF_THE_DAY[hour];
//    strcpy(dest, HOURS_OF_THE_DAY[hour]);
  } else {
    return nullptr; 
//    *dest = 0;
    log_e("invalid hour: %d", hour);
  }
}


const char STRING_HOUR_LONG_00[] = "Mitternacht";
const char STRING_HOUR_LONG_01[] = "ein Uhr nachts";
const char STRING_HOUR_LONG_02[] = "zwei Uhr nachts";
const char STRING_HOUR_LONG_03[] = "drei Uhr nachts";
const char STRING_HOUR_LONG_04[] = "vier Uhr morgens";
const char STRING_HOUR_LONG_05[] = "fünf Uhr morgens";
const char STRING_HOUR_LONG_06[] = "sechs Uhr morgens";
const char STRING_HOUR_LONG_07[] = "sieben Uhr morgens";
const char STRING_HOUR_LONG_08[] = "acht Uhr morgens";
const char STRING_HOUR_LONG_09[] = "neun Uhr morgens";
const char STRING_HOUR_LONG_10[] = "zehn Uhr vormittags";
const char STRING_HOUR_LONG_11[] = "elf Uhr vormittags";
const char STRING_HOUR_LONG_12[] = "Mittag";
const char STRING_HOUR_LONG_13[] = "eins Uhr mittags";
const char STRING_HOUR_LONG_14[] = "zwei Uhr nachmittags";
const char STRING_HOUR_LONG_15[] = "drei Uhr nachmittags";
const char STRING_HOUR_LONG_16[] = "vier Uhr nachmittags";
const char STRING_HOUR_LONG_17[] = "fünf Uhr nachmittags";
const char STRING_HOUR_LONG_18[] = "sechs Uhr abends";
const char STRING_HOUR_LONG_19[] = "sieben Uhr abends";
const char STRING_HOUR_LONG_20[] = "acht Uhr abends";
const char STRING_HOUR_LONG_21[] = "neun Uhr abends";
const char STRING_HOUR_LONG_22[] = "zehn Uhr abends";
const char STRING_HOUR_LONG_23[] = "elf Uhr nachts";

const char* getHourOfTheDayLong(int hour) {
  const static char *HOURS_OF_THE_DAY_LONG[] = {
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
    return HOURS_OF_THE_DAY_LONG[hour];
//    strcpy(dest, HOURS_OF_THE_DAY_LONG[hour]);
  } else {
    return nullptr; 
//    *dest = 0;
    log_e("invalid hour: %d", hour);
  }
}




const char STRING_MONTHS_00[] = "Januar";
const char STRING_MONTHS_01[] = "Februar";
const char STRING_MONTHS_02[] = "März";
const char STRING_MONTHS_03[] = "April";
const char STRING_MONTHS_04[] = "Mai";
const char STRING_MONTHS_05[] = "Juni";
const char STRING_MONTHS_06[] = "Juli";
const char STRING_MONTHS_07[] = "August";
const char STRING_MONTHS_08[] = "September";
const char STRING_MONTHS_09[] = "Oktober";
const char STRING_MONTHS_10[] = "November";
const char STRING_MONTHS_11[] = "Dezember";

const char* getMonthOfTheYear(int month) {
  const static char *MONTHS_OF_THE_YEAR[] = {
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
    return MONTHS_OF_THE_YEAR[month];
//    strcpy(dest, MONTHS_OF_THE_YEAR[month]);
  } else {
    return nullptr; 
    //*dest = '\0';
    log_e("invalid month: %d", month);
  }
}




const char STRING_DAY_0[] = "Sonntag";
const char STRING_DAY_1[] = "Montag";
const char STRING_DAY_2[] = "Dienstag";
const char STRING_DAY_3[] = "Mittwoch";
const char STRING_DAY_4[] = "Donnerstag";
const char STRING_DAY_5[] = "Freitag";
const char STRING_DAY_6[] = "Samstag";


const char* getDayOfTheWeek(int day) {
  const static char *DAY_OF_THE_WEEK[] = {
    STRING_DAY_0,
    STRING_DAY_1,
    STRING_DAY_2,
    STRING_DAY_3,
    STRING_DAY_4,
    STRING_DAY_5,
    STRING_DAY_6
  };
  if (day >= 0 && day < 7) {
    return DAY_OF_THE_WEEK[day];
//    strcpy(dest, DAY_OF_THE_WEEK[day]);
  } else {
    return nullptr; 
//    *dest = 0;
    log_e("invalid day of the week: %d", day);
  }
}

