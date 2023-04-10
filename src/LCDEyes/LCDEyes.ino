//           frames++;
//       if(millis() - ttime > 5000){
//         fps = frames*1.0/5.0;  
//         ttime = millis();          
//         frames = 0;
//       }

// float frames = 0;
// float fps = 0;
// unsigned long ttime = 0;

/*
  LCDEyes.ino - Simple Project that animates robot eyes on an LCD screen.
  Created by Nischay Joshi, 09-05-2023
*/

// Load TFT driver library
#include <TFT_eSPI.h>
#include <SPI.h>
#include "EyesClass.h"

TFT_eSPI tft = TFT_eSPI();  
EyesClass Eyes;

void EyeScroll();

void setup()
{
  Serial.begin(115200);
  tft.init();            // initialize LCD
  tft.setRotation(1);
  //We want black background
  tft.fillScreen(BG_COLOR);
  Eyes.Initialize_Eyes();
}


void loop()
{
  Eyes.Animate_Blink(tft);
  delay(500);
}

void EyeScroll(){
  //Draw the eyes
  Eyes.Draw_Eyes(tft);
  delay(1000);
  for(int i = 0; i < 10; i++){
    //Clear the old eyes
    Eyes.Clear_Eyes(tft);
    //Move the eyes
    Eyes.Move_Eyes(2, 2, 2, 2);
    //Draw the new eyes
    Eyes.Draw_Eyes(tft);
    delay(50);
  }
  //Now go back to the original position
  for(int i = 0; i < 10; i++){
    //Clear the old eyes
    Eyes.Clear_Eyes(tft);
    //Move the eyes
    Eyes.Move_Eyes(-2, -2, -2, -2);
    //Draw the new eyes
    Eyes.Draw_Eyes(tft);
    delay(50);
  }
}