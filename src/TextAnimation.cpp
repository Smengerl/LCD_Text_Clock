#include <arduino.h>
#include "TextAnimation.hpp"



TextAnimation::TextAnimation(uint8_t width, uint8_t height) {
  maxWidth = width;
  maxHeight = height;
}

void TextAnimation::start(uint16_t total_frames, t_animation_type a_type) {
  remaining_animation_frames = total_animation_frames = total_frames;
  animation_Type = a_type;
}

void TextAnimation::stop() {
  remaining_animation_frames = total_animation_frames = 0;
}

void TextAnimation::tick() {
  remaining_animation_frames--;
}

bool TextAnimation::isEnded() {
  return remaining_animation_frames == 0;
}

void TextAnimation::apply(char *dest, char *beginFrame, char *endFrame) {
 
  Serial.print(F("Animation "));
  switch (animation_Type) {
    case ANIMATION_TYPE_OVERLAY_LEFT:
      Serial.print(F("OVERLAY_LEFT"));
      break;
    case ANIMATION_TYPE_OVERLAY_RIGHT:
      Serial.print(F("OVERLAY_RIGHT"));
      break;
    case ANIMATION_TYPE_MATRIX:
      Serial.print(F("MATRIX"));
      break;
    case ANIMATION_TYPE_WRITE:
      Serial.print(F("WRITE"));
      break;
  }
  Serial.print(F(" ("));
  Serial.print(remaining_animation_frames);
  Serial.print(F("/"));
  Serial.print(total_animation_frames);
  Serial.print(F("): \""));
  Serial.print(beginFrame);
  Serial.print(F("\" --> \""));
  Serial.print(endFrame);
  Serial.print(F("\""));

  for (uint16_t i = 0; i < maxWidth*maxHeight; i++) {
    if (i >= strlen(endFrame)) {
      endFrame[i] = 0;
    }
    if (i >= strlen(beginFrame)) {
      beginFrame[i] = 0;
    }


    // For robustness reasons, always use final state in last frame
    if (remaining_animation_frames == 0) {
      dest[i] = endFrame[i];




    } else if (animation_Type == ANIMATION_TYPE_OVERLAY_LEFT) {
      // alten Text von rechts nach links durch neuen Text ersetzen
      //Serial.print(F("OVERLAY_LEFT"));
      if ((i % maxWidth) >= ((float)(remaining_animation_frames) / (float)total_animation_frames) * maxWidth) {
        dest[i] = endFrame[i];
      } else {
        dest[i] = beginFrame[i];
      }



    } else if (animation_Type == ANIMATION_TYPE_OVERLAY_RIGHT) {
      // alten Text von links nach rechts durch neuen Text ersetzen
      //Serial.print(F("OVERLAY_RIGHT"));
      if ((i % maxWidth) <= ((float)(total_animation_frames - remaining_animation_frames) / (float)total_animation_frames) * maxWidth) {
        dest[i] = endFrame[i];
      } else {
        dest[i] = beginFrame[i];
      }




    } else if (animation_Type == ANIMATION_TYPE_MATRIX) {
      // Buchstaben rotieren
      int16_t c = endFrame[i] - beginFrame[i];
      if (c > 0) {
        // Aufwärts rotieren
        if ((total_animation_frames - remaining_animation_frames) < c) {
          // Rolliere die Zeichen, so lange die Nummer des aktuelle Animationsframe kleiner ist, als der per Animaton insgesamt zu überbrückende Zeichenabstand
          dest[i] = endFrame[i] - remaining_animation_frames;
        } else {
          // Wir sind am Zielzeichen angekommen, sobald die Nummer des Animationsframes größer oder gleich der Anzahl der zu überbrückenden Zeichen ist
          dest[i] = endFrame[i];
        }
      } else if (c < 0) {
        // Abwärts rotieren
        if ((total_animation_frames - remaining_animation_frames) < -c) {
          // Rolliere die Zeichen, so lange die Nummer des aktuelle Animationsframe kleiner ist, als der per Animaton insgesamt zu überbrückende Zeichenabstand
          dest[i] = endFrame[i] + remaining_animation_frames;
        } else {
          // Wir sind am Zielzeichen angekommen, sobald die Nummer des Animationsframes größer oder gleich der Anzahl der zu überbrückenden Zeichen ist
          dest[i] = endFrame[i];
        }
      } else {
        dest[i] = endFrame[i];
      }


    } else if (animation_Type == ANIMATION_TYPE_WRITE) {
      // Zeilen nacheinander von leer aus aufbauen
      //Serial.print(F("WRITE"));
      float percentage = (float)(total_animation_frames - remaining_animation_frames) / (float)total_animation_frames;
      if (i < strlen(endFrame) * percentage) {
        dest[i] = endFrame[i];
      } else {
        dest[i] = 0;
      }


    } else {
      Serial.print(F("UNKNOWN ANIMATION TYPE"));
      dest[i] = endFrame[i];
    }
  }
  Serial.print(F(" ==== \""));
  Serial.print(dest);
  Serial.println(F("\""));
}
