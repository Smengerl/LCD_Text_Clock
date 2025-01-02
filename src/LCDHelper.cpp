// include the LCD library
#include <arduino.h>
#include "LCDHelper.hpp"

#define DIMMING_BACKLIGHT_STEPSIZE 10
#define DIMMING_CONTRAST_STEPSIZE 10

/*
// make some custom characters:
const byte CHAR_HEART[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

const byte CHAR_SMILEY[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

const byte CHAR_FROWNIE[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};

const byte CHAR_ARMS_DOWN[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

const byte CHAR_ARMS_UP[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

*/


LCDHelper::LCDHelper(
  const int LCD_CHAR_PER_LINE, 
  const int LCD_LINES, 
  const int CONTRAST_ON_START, 
  const int BACKLIGHT_ENABLED_ON_START, 
  const int RS, const int EN, const int D4, const int D5, const int D6, const int D7, 
  const int BACKLIGHT_PIN, const int CONTRAST_PIN) 
  : lcd(RS, EN, D4, D5, D6, D7)
  {


  backlight_pin = BACKLIGHT_PIN;
  contrast_pin = CONTRAST_PIN;

  maxWidth = LCD_CHAR_PER_LINE;
  maxHeight = LCD_LINES;

  // set up the LCD's number of columns and rows:
  lcd.begin(LCD_CHAR_PER_LINE, LCD_LINES);
/*
  lcd.createChar(0, CHAR_HEART);
  lcd.createChar(1, CHAR_SMILEY);
  lcd.createChar(2, CHAR_FROWNIE);
  lcd.createChar(3, CHAR_ARMS_DOWN);
  lcd.createChar(4, CHAR_ARMS_UP);
*/
  // enable LCD backlight
  pinMode(backlight_pin, OUTPUT);

  setBacklightEnabled(BACKLIGHT_ENABLED_ON_START);

  // set contrast
  pinMode(contrast_pin, OUTPUT);
  setContrast(CONTRAST_ON_START);

  lcd.clear();
  lcd.noCursor();
  lcd.noBlink();
  lcd.display();
  
}







void LCDHelper::print(char *s) {
  lcd.clear();
  for (int y = 0; y < maxHeight; y++) {
    lcd.setCursor(0, y);
    for (int x = 0; x < maxWidth; x++) {
      char c = s[y * maxWidth + x];
      if (c != 0) {
        lcd.print(c);
      } else {
        return;
      }
    }
  }
}






void LCDHelper::setBacklightEnabled(bool value, uint16_t duration) {
  if (backlightEnabled == value) return;
  if (duration > 0) {
    if (value) {
      // Dimmen zum Einschalten
      Serial.print(F("Dim backlight from off to on in "));
      Serial.print(duration);
      Serial.println(F("ms "));
      for (int i = 0; i < duration; i = i + DIMMING_BACKLIGHT_STEPSIZE) {
        float t = ((float)255 * i) / duration;
        analogWrite(backlight_pin, (int)t);
        delay(DIMMING_BACKLIGHT_STEPSIZE);
        /*
        Serial.print(i);
        Serial.print(F(": Backlight adapted: "));
        Serial.println(t);
        */
      }
    } else {
      // Dimmen zum Ausschalten
      Serial.print(F("Dim backlight from on to off in "));
      Serial.print(duration);
      Serial.println(F("ms "));
      for (int i = 0; i < duration; i = i + DIMMING_BACKLIGHT_STEPSIZE) {
        float t = 255 - ((float)255 * i) / duration;
        analogWrite(backlight_pin, (int)t);
        delay(DIMMING_BACKLIGHT_STEPSIZE);
        /*
        Serial.print(i);
        Serial.print(F(": Backlight adapted: "));
        Serial.println(t);
        */
      }
    }
  }
  // Finally use regular "digital" write to ensure the backlight is fully on/off
  setBacklightEnabled(value);
}

void LCDHelper::setBacklightEnabled(bool value) {
  if (value) {
    Serial.println(F("Backlight enabled"));
  } else {
    Serial.println(F("Backlight disabled"));
  }
  digitalWrite(backlight_pin, value);
  backlightEnabled = value;
}
bool LCDHelper::isBacklightEnabled() {
  return backlightEnabled;
}





void LCDHelper::setContrast(uint8_t value, uint16_t duration) {
  if (contrast == value) return;
  if (duration > 0) {
    if (value > contrast) {
      // Dimmen zum Einschalten
      Serial.print(F("Dim contrast up: "));
      Serial.print(contrast);
      Serial.print(F("-->"));
      Serial.print(value);
      Serial.print(F(" in "));
      Serial.print(duration);
      Serial.println(F("ms "));
      
      for (int i = 0; i < duration; i = i + DIMMING_CONTRAST_STEPSIZE) {
        float t = contrast + ((float)(value-contrast) * i) / duration;
        analogWrite(contrast_pin, (int)t);
        delay(DIMMING_CONTRAST_STEPSIZE);
        /*
        Serial.print(i);
        Serial.print(F(": Contrast adapted: "));
        Serial.println(t);
        */
      }
    } else {
      // Dimmen zum Ausschalten
      Serial.print(F("Dim contrast down: "));
      Serial.print(contrast);
      Serial.print(F("-->"));
      Serial.print(value);
      Serial.print(F(" in "));
      Serial.print(duration);
      Serial.println(F("ms "));
      for (int i = 0; i < duration; i = i + DIMMING_CONTRAST_STEPSIZE) {
        float t = contrast - ((float)(contrast-value) * i) / duration;
        analogWrite(contrast_pin, (int)t);
        delay(DIMMING_CONTRAST_STEPSIZE);
        /*
        Serial.print(i);
        Serial.print(F(": Contrast adapted: "));
        Serial.println(t);
        */
      }
    }
  }
  // Finally use regular "digital" write to ensure the backlight is fully on/off
  setContrast(value);
}

void LCDHelper::setContrast(uint8_t value) {
  Serial.print(F("Contrast adapted: "));
  Serial.println(value);
  analogWrite(contrast_pin, value);
  contrast = value;
}
uint8_t LCDHelper::getContrast() {
  return contrast;
}


// Umlaute aus String killen
void LCDHelper::normalizeUmlaute(char *s) {
  int i1 = 0, i2 = 0;
  while (s[i1] != 0) {
    // Kompaktifizieren, sofern die Indizes einmal voneinander abweichen
    if (i1 != i2) s[i2] = s[i1];

    if ((unsigned char)s[i1] == 0xC3) {  // start multiby String detected?
      switch ((unsigned char)s[i1 + 1]) {
        case 0x84:  // Ä
        case 0xA4:  // ä
          s[i2] = 0xE1;
          i1++;
          break;
        case 0x96:  // Ö
        case 0xB6:  // ö
          s[i2] = 0xEF;
          i1++;
          break;
        case 0x9C:  // Ü
        case 0xBC:  // ü
          s[i2] = 0xF5;
          i1++;
          break;
      }
    }
    i1++;
    i2++;
  }
  s[i2] = 0;
}





void LCDHelper::centerStringToDimensions_helper(char *dest, char *source, int y, int beginChar, int endChar, t_text_alignment alignment) {
  // Make sure to stop before we run out of bounds
  if (y > maxHeight) return;

  int lineLength = endChar - beginChar;

  int paddingBegin = 0;
  switch (alignment) {
    case t_text_alignment::TEXT_ALIGNMENT_LEFT:
      break;
    case t_text_alignment::TEXT_ALIGNMENT_RIGHT:
      paddingBegin = maxWidth - lineLength; 
      break;
    case t_text_alignment::TEXT_ALIGNMENT_CENTERED:
      paddingBegin = (maxWidth - lineLength) / 2;
      break;
  }
  int paddingEnd = maxWidth - paddingBegin - lineLength;

  for (int n = 0; n < paddingBegin; n++) {
    dest[maxWidth * y + n] = ' ';
  }
  for (int n = 0; n < lineLength; n++) {
    dest[maxWidth * y + paddingBegin + n] = source[beginChar + n];
  }
  for (int n = 0; n < paddingEnd; n++) {
    dest[maxWidth * y + paddingBegin + lineLength + n] = ' ';
  }
}


// Centers string along lines in LCD with Line break by spaces
void LCDHelper::centerStringToDimensions(char *dest, char *source, bool wordWrap, t_text_alignment alignment) {
  int y = 0;
  int lineBegin = 0;
  int lineEnd = 0;

  int len = strlen(source);
  for (int i = 0; i <= len; i++) {
    // wrap on word end
    if (wordWrap && isSpace(source[i])) {
      lineEnd = i;
    }

    if (i - lineBegin >= maxWidth) {
      if (lineEnd == lineBegin) {
        // Fall 1: Ende der LCD Zeile erreicht, bisher keinen Word wrap gefunden --> Die LCD Zeile voll machen, indem sie bis zum aktuellen Zeichen gefüllt wird, ab dem unmittelbar nachfolgenden Zeichen dann die Suche für die nächste Zeile erneut beginnen lassen
        centerStringToDimensions_helper(dest, source, y, lineBegin, i, alignment);
        lineBegin = i;
        lineEnd = i;
        y++;

      } else {
        // Fall 2: Ende der LCD Zeile erreicht, aber einen Word wrap gefunden --> In der aktuellen Zeile bis zum Word Wrap ausgeben und ab dort nach dem Trennzeichen die Suche für die nächste Zeile beginnen lassen
        centerStringToDimensions_helper(dest, source, y, lineBegin, lineEnd, alignment);
        lineBegin = lineEnd + 1;
        lineEnd = lineEnd + 1;
        y++;
      }
    }

    if (i == len - 1) {
      if (i > lineBegin) {
        // Fall 3: Ende des gesamten Textes erreicht und mindestens ein Zeichen bisher nicht ausgegeben --> Alles noch nicht ausgegebene in der aktuellen Zeile ausgeben
        centerStringToDimensions_helper(dest, source, y, lineBegin, len, alignment);
        lineBegin = i;
        lineEnd = i;
        y++;
      }
    }
  }

  // Fill the remaining space in buffer with blanks
  for (; y < maxHeight; y++) {
    for (int x = 0; x < maxWidth; x++) {
      dest[maxWidth * y + x] = ' ';
    }
  }
}

