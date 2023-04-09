/*
  printOnOled.cpp - Library for writing text to Oled
  Created by Patrik Hermansson, september 2021.
  Released into the public domain.
*/
#include <stdio.h>
#include <stdlib.h>

#include "Arduino.h"
#include <U8g2lib.h>
#include <print_on_oled.h>
#include "../../settings.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void initOled()
{
  Serial.printf("Init Oled.\n");
  u8g2.begin();
  u8g2.setFont(u8g2_font_crox3h_tr);
  #ifdef DEBUG
    u8g2_uint_t dh = u8g2.getDisplayHeight();
    u8g2_uint_t dw = u8g2.getDisplayWidth();
    Serial.printf("Oled size: %d x %d\n", dw, dh);
  #endif

  u8g2.drawStr(10,10,"Init screen!");	// write something to the internal memory
  u8g2.sendBuffer();
  delay(700);
  //u8g2.drawStr(10,10,"01234567890123456789");	// write something to the internal memory
  //u8g2.sendBuffer();
}

void printoled(char text_to_write_oled[100], int x, int y)
{
  #ifdef DEBUG
    Serial.printf("Text to write: %s\n", text_to_write_oled);
  #endif

  u8g2.clearBuffer();

  delay(1000);

  char *token;
  #define MAX_STRING_SIZE 200

  int token_counter=1;
  char ostring[MAX_STRING_SIZE];
  int line_pos=y;
  strcpy(ostring,"\0");

  if (strlen(text_to_write_oled) < 16) {
      #ifdef DEBUG
        printf("A short text(%s),  x=%d y=%d\n", text_to_write_oled, x, y);
      #endif
  
      u8g2.drawStr(x,y,text_to_write_oled);	// x, y, horizontal x, vertical y.
      u8g2.sendBuffer();
      strcpy(ostring,"\0");
      delay(500);
  }
  else {
    token = strtok (text_to_write_oled, " ,.-");    // Split text. 
    strcat(ostring, token);        // Store current token
    strcat(ostring, " ");
      
    #ifdef DEBUG
      printf( "First token: %s\n", ostring );
      int slen = strlen(text_to_write_oled); 
      printf("len: %d", slen);
    #endif
  
    while( token != NULL ) {
      token = strtok(NULL, " ,.-"); // Get next part
      #ifdef DEBUG
        printf("Split\n");
      #endif
      
      if(token!= NULL)
      {
        strcat(ostring, token);        // Store current token
        strcat(ostring, " ");

        if (strlen(ostring) >= 14) {
            #ifdef DEBUG
              printf( "%d - %s\n", token_counter, ostring );
            #endif

            line_pos = token_counter * 16;

            u8g2.drawStr(x,line_pos,ostring);	// x, y, horizontal x, vertical y.
            u8g2.sendBuffer();

            strcpy(ostring,"\0");
            token_counter++;
            delay(2500);
        }       
    //  }    
    }
  }
  }
  #ifdef DEBUG
    Serial.printf("---------------\n");
  #endif
}

void clearOled()
{
  u8g2.clearBuffer();
}
