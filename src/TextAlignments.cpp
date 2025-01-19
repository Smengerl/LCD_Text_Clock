#include <Arduino.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "TextAlignments.hpp"

//#define LCD_CHAR_PER_LINE 20
//#define LCD_LINES 4
//#define STRING_BUFFER_SIZE LCD_CHAR_PER_LINE * LCD_LINES + 20

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 11




// Function to split a string into words
char** splitWords(const char* text, int* wordCount) {
  char* textCopy = strdup(text);
  char* token = strtok(textCopy, " ");
  char** words = (char**)malloc(sizeof(char*) * 1024); // Allocate space for words
  int count = 0;

  while (token != NULL) {
      words[count++] = strdup(token);
      token = strtok(NULL, " ");
  }

  *wordCount = count;
  free(textCopy);

  log_d("Splitted text (\"%s\") into %d words", text, count);
  return words;
}

// Function to free words
void freeWords(char** words, int wordCount) {
    for (int i = 0; i < wordCount; i++) {
        free(words[i]);
    }
    free(words);
}


/// @brief Replaces Umlaute in string by char code used in LCD
/// @param s The input string to modify
void normalizeUmlaute(char *s) {
    int readIdx = 0, writeIdx = 0;

    while (s[readIdx] != '\0') {
        if ((unsigned char)s[readIdx] == 0xC3) {  // Starta multibyte sequence
            unsigned char nextChar = (unsigned char)s[readIdx + 1];
            switch (nextChar) {
                case 0x84:  // Ä
                case 0xA4:  // ä
                    s[writeIdx++] = 0xE1;
                    readIdx += 2;
                    continue;
                case 0x96:  // Ö
                case 0xB6:  // ö
                    s[writeIdx++] = 0xEF;
                    readIdx += 2;
                    continue;
                case 0x9C:  // Ü
                case 0xBC:  // ü
                    s[writeIdx++] = 0xF5;
                    readIdx += 2;
                    continue;
                default:
                    break;  // Not a recognized umlaut sequence
            }
        }

        // Copy character as is
        s[writeIdx++] = s[readIdx++];
    }

    s[writeIdx] = '\0';  // Null-terminate the string
}



uint8_t* createWeightTable(uint8_t displayWidth, uint8_t displayHeight, t_horizontal_text_alignment h_alignment, t_vertical_text_alignment v_alignment) {
  uint8_t* weights = (uint8_t*)malloc(displayWidth * displayHeight * sizeof(uint8_t));

  log_d("Creating weight table:");
  switch (v_alignment) {
    case TEXT_V_ALIGNMENT_TOP:      log_d("Vertical alignment: TOP");         break;
    case TEXT_V_ALIGNMENT_MIDDLE:   log_d("Vertical alignment: MIDDLE");      break;
    case TEXT_V_ALIGNMENT_BOTTOM:   log_d("Vertical alignment: BOTTOM");      break;
    case TEXT_V_ALIGNMENT_STRETCH:  log_d("Vertical alignment: STRETCH");     break;
  }
  switch (h_alignment) {
    case TEXT_H_ALIGNMENT_LEFT:     log_d("Horizontal alignment: LEFT");      break;
    case TEXT_H_ALIGNMENT_CENTER:   log_d("Horizontal alignment: CENTERED");  break;
    case TEXT_H_ALIGNMENT_RIGHT:    log_d("Horizontal alignment: RIGHT");     break;
    case TEXT_H_ALIGNMENT_STRETCH:  log_d("Horizontal alignment: STRETCH");   break;
  }

  for (int y = 0; y < displayHeight; y++) {
    char debug_output[255] = ""; 
    snprintf(debug_output, sizeof(debug_output), "Line %d: ", y);

    int yWeights;
    switch (v_alignment) {
      case TEXT_V_ALIGNMENT_TOP:
        yWeights = y;
        break;

      case TEXT_V_ALIGNMENT_MIDDLE:
        yWeights = abs(((displayHeight - 1) / 2.0 - y));
        break;

      case TEXT_V_ALIGNMENT_BOTTOM:
        yWeights = (displayHeight - 1 - y);
        break;

      case TEXT_V_ALIGNMENT_STRETCH:
        yWeights = 1;
        break; 

      default:
        log_e("undefined V alignment!");
        yWeights = 1;
        break;
    }

    for (int x = 0; x < displayWidth; x++) {
      int xWeights;
      switch (h_alignment) {
        case TEXT_H_ALIGNMENT_LEFT:
          xWeights = x;
          break;

        case TEXT_H_ALIGNMENT_CENTER:
          xWeights = abs(((displayWidth - 1) / 2.0 - x));
          break;
          
        case TEXT_H_ALIGNMENT_RIGHT:
          xWeights = (displayWidth - 1- x);
          break;

        case TEXT_H_ALIGNMENT_STRETCH:
          xWeights = 1;
          break; 

        default:
          log_e("undefined H alignment!");
          xWeights = 1;
          break; 
      }

      weights[y * displayWidth + x] = ( xWeights * CHAR_WIDTH * displayHeight + yWeights * CHAR_HEIGHT * displayWidth ) / _min(displayWidth, displayHeight);

      log_i("x=%d / y=%d = %3d: xWeight=%d, yWeights=%d, displayWidth=%d, displayHeight=%d, XTotal=%d, YTotal=%d", 
        x, y, weights[y * displayWidth + x],
        xWeights, yWeights, displayWidth, displayHeight, 
        xWeights * CHAR_WIDTH * displayHeight, 
        yWeights * CHAR_HEIGHT * displayWidth);

      char debug_output_cell[6];
      snprintf(debug_output_cell, sizeof(debug_output_cell), "[%3X]", xWeights + yWeights); 
      strcat(debug_output, debug_output_cell);
    };

    log_d("%s", debug_output);
  }

  return weights;
}


// Writes the ideal positioning of the start indexes per word 
int recursiveF(int bestWeight, char** words, int wordCount, int currentWord, int currentLine, int currentCosts, int displayWidth, int displayHeight, uint8_t* weights, int* startIndexPerWord) {

  // All words placed -> success
  if (currentWord == wordCount) {
    log_d("Success (Costs %d, best %d)!", currentCosts, bestWeight);
    return currentCosts;
  } else {
    // Not all words placed yet...
    
    // ...fail if we already have exceeded the display height
    if (currentLine == displayHeight) {
      log_d("Not all words could be placed (%d of %d placed), aborting, currentCosts=%d (Best: %d)", currentWord, wordCount, currentCosts, bestWeight);
      return -1;
    }

    // ...fail if we already have higher costs than the best solution so far
    if (bestWeight > -1 && currentCosts > bestWeight) {
      log_d("Current costs %d higher than best cost %d, aborting", currentCosts, bestWeight);
      return -1;
    }
  }


  // For each line, try to squeeze in as much words as possible from the remaining ones
  for (int wordsInLine = wordCount - currentWord; wordsInLine >= 0; wordsInLine--) {

    // We can skip to try if we are in the last line and the current number of words does not match the remaining words
    if (currentLine == displayHeight - 1) {
      if (currentWord + wordsInLine != wordCount) {
        log_d("Line %.*d: Skipping to try %d words in last line, since %d more words need to be placed", currentLine + 1, currentLine, wordsInLine, wordCount - currentWord);
        continue;
      }
    }

    // If any word was placed already in the previous line, than this line must not be blank
    if (currentWord > 0 && wordsInLine == 0) {
      log_d("Line %.*d: Skipping to try no words for this line since there was already words placed", currentLine + 1, currentLine);
      continue;
    }

    log_d("Line %.*d: Trying %d words in line, beginning with %d (total: %d), currentCosts=%d (Best: %d)", currentLine + 1, currentLine, wordsInLine, currentWord, wordCount, currentCosts, bestWeight);

//      totalLineLength-=strlen(words[currentWord + wordsInLine]);

    int totalLineLength = 0; // Länge der Zeile in Zeichen
    int lineCosts = 0; // Minimale Kosten der Zeile
    int bestWordStartingPositioningInLine = -1; // Startposition des ersten Zeichens des Worts aus der Zeile bei minimalen Kosten

    if (wordsInLine > 0) {
      // Determine length of all words line in characters
      for (int i = 0; i < wordsInLine; i++) {
        if (i>0) totalLineLength++;
        totalLineLength+=strlen(words[currentWord + i]);
      }
      
      // Stop trying this combination or any longer concatenation of words if the length of these words already exceeds the display line 
      if (totalLineLength > displayWidth) continue;

  
      // if there are any words in this line, we only need to find the best positioning of these words to find this line's weight
      // For this, iterate over all possible starting positions of these words in the line to find the ideal placement
      lineCosts = INT_MAX;
      for (int startCharInLine = 0; startCharInLine <= displayWidth - totalLineLength; startCharInLine++) {

        // Calculate costs of the words at this positioning
        int totalCosts = 0;
        for (int i = startCharInLine; i < startCharInLine + totalLineLength; i++) {
          totalCosts+=weights[currentLine * displayWidth + i];
        }
        if (totalCosts < lineCosts) {
          bestWordStartingPositioningInLine = startCharInLine;
          lineCosts = totalCosts;
        }
      }
      if (bestWordStartingPositioningInLine == -1) {
        log_e("No positioning in line found! Internal error - Abort");
        return -1;
      } 
    }

    // Now recursively find positions for remaining words in the next line
    int recursiveCosts = recursiveF(bestWeight, words, wordCount, currentWord + wordsInLine, currentLine + 1, currentCosts + lineCosts, displayWidth, displayHeight, weights, startIndexPerWord);
    
    // If a placement of all remaining words was found...
    if (recursiveCosts != -1) {

      // Check if this placement is the first one found ever or better than any other one found so far
      if (bestWeight == -1 || recursiveCosts < bestWeight) {
        bestWeight = recursiveCosts;

        // As this seems to be the best fit yet, write start index of all words in line to result
        if (wordsInLine > 0) {
          startIndexPerWord[currentWord] = displayWidth * currentLine + bestWordStartingPositioningInLine; 
          for (int i = currentWord; i < currentWord + wordsInLine - 1; i++) {
            startIndexPerWord[i + 1] = startIndexPerWord[i] + 1 + strlen(words[i]);
          }
        }
      }
    }
  }
  return bestWeight;
}



void formatString(char *dest, const char *source, uint8_t displayWidth, uint8_t displayHeight, uint8_t* weights) {


  // Copy paramter to buffer to be able to work and modify it in any way
  char buf[strlen(source) + 1];
  strcpy(buf, source);

  // Modifies the string
  normalizeUmlaute(buf);



  int wordCount;
  char** words = splitWords(buf, &wordCount);
  int startIndexPerWord[wordCount];

  int r = recursiveF(-1, words, wordCount, 0, 0, 0, displayWidth, displayHeight, weights, startIndexPerWord);

  memset(dest, ' ', displayWidth * displayHeight);
  dest[displayWidth * displayHeight] = '\0';

  if (r != -1) {
    log_d("Formatting success!");
    for (int i = 0; i<wordCount; i++) {
      int y = startIndexPerWord[i] / displayWidth;
      int x = startIndexPerWord[i] % displayWidth;

      log_d("Adding word %s to x=%d, y=%d", words[i], x, y);


      // Swap lines in case of 4*20 display
      if (displayHeight == 4 && displayWidth == 20) {
        if (y == 1) {
          y = 2;
        } else if (y == 2) {
          y = 1;
        }
      }

      memcpy(&dest[y * displayWidth + x], words[i], strlen(words[i]));
    }
  } else {
    log_e("Formatting failed!");
  }
  freeWords(words, wordCount);


  for (int i = 0; i<displayHeight; i++) {
    log_d("Line %d: \"%.*s\"", i, displayWidth, &dest[i * displayWidth]);
  }
}


void formatString(char *dest, const char *source, uint8_t displayWidth, uint8_t displayHeight, t_horizontal_text_alignment h_alignment, t_vertical_text_alignment v_alignment) {
  // Create wrapped and aligned string
  uint8_t* weights = createWeightTable(displayWidth, displayHeight, h_alignment, v_alignment);

  formatString(dest, source, displayWidth, displayHeight, weights);

  free(weights);
}



/*
void setup()
{
  Serial.begin(115200);
  char formattedString[STRING_BUFFER_SIZE];

  formatString(formattedString, "left top text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_H_ALIGNMENT_LEFT, TEXT_V_ALIGNMENT_TOP);
  formatString(formattedString, "right top aligned text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_H_ALIGNMENT_RIGHT, TEXT_V_ALIGNMENT_TOP);
  formatString(formattedString, "center top aligned text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_H_ALIGNMENT_CENTER, TEXT_V_ALIGNMENT_TOP);

  formatString(formattedString, "left middle text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_H_ALIGNMENT_LEFT, TEXT_V_ALIGNMENT_MIDDLE);
  formatString(formattedString, "right middle aligned text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_H_ALIGNMENT_RIGHT, TEXT_V_ALIGNMENT_MIDDLE);
  formatString(formattedString, "center middle aligned text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_H_ALIGNMENT_CENTER, TEXT_V_ALIGNMENT_MIDDLE);

  formatString(formattedString, "left bottom text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_ALIGNMENT_LEFT, TEXT_H_V_ALIGNMENT_BOTTOM);
  formatString(formattedString, "right bottom aligned text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_H_ALIGNMENT_RIGHT, TEXT_V_ALIGNMENT_BOTTOM);
  formatString(formattedString, "center bottom aligned text that needs optimal formatting.", LCD_CHAR_PER_LINE, LCD_LINES, TEXT_H_ALIGNMENT_CENTER, TEXT_V_ALIGNMENT_BOTTOM);
}


void loop() {

}
*/