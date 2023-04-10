/*
    *  EyesClass.cpp
    *  LCDEyes
    *
    *  By Nischay Joshi
    *   09-04-2023
*/

#include "EyesClass.h"

/*
    * Initialize the eyes
*/
void EyesClass::Initialize_Eyes(){
  //Initialize the eyes
  this->Eye1_centerX = 45;
  this->Eye1_centerY = 64;
  this->Eye2_centerX = 115;
  this->Eye2_centerY = 64;
  this->Eye1_Current_Width = EYE_WIDTH/2;
  this->Eye1_Current_Height = EYE_HEIGHT/2;
  this->Eye2_Current_Width = EYE_WIDTH/2;
  this->Eye2_Current_Height = EYE_HEIGHT/2;
  this->Eye_Color = EYE_COLOR;
  this->Iris_Color = IRIS_COLOR;
  this->Iris_radius = IRIS_RADIUS;
  this->Iris1_centerX = this->Eye1_centerX;
  this->Iris1_centerY = this->Eye1_centerY;
  this->Iris2_centerX = this->Eye2_centerX;
  this->Iris2_centerY = this->Eye2_centerY;
  this->Mood_State = Neutral;
}

/*
    * Function to generate a blink effect
*/
void EyesClass::Animate_Blink(TFT_eSPI &tft){

    //Clear eyes
    this->Clear_Eyes(tft);

    this->Mood_State = Neutral;
    //First close both Eyes
    this->ShrinkEye(0, 100, 2, tft);
    delay(100);

    //Now open the eyes
    this->ShrinkEye(0, 0, 2, tft);
    delay(500);

    //Sqiunt the eyes
    this->ShrinkEye(0, 70, 6, tft);
    //change mmood to happy
    this->Mood_State = Happy;
    delay(2000);

    //move the 2nd eye to the right
    this->Move_Eyes(tft, this->Eye1_centerX, this->Eye1_centerY, this->Eye2_centerX+10, this->Eye2_centerY, 2);
    delay(10);

    //Now open the right eyes
    this->ShrinkEye(0, 12, 2, tft);
    delay(2000);

    //now only close the left eye
    this->ShrinkEye(0, 100, 2, tft);
    //move the 2nd eye to the left
    this->Move_Eyes(tft, this->Eye1_centerX+10, this->Eye1_centerY+12, this->Eye2_centerX-5, this->Eye2_centerY+12, 3);
    delay(10);
    //now open it
    this->ShrinkEye(0, 0, 4, tft);
    delay(1000);
    //move eyes back to original position
    this->Move_Eyes(tft, this->Eye1_centerX-10, this->Eye1_centerY-12, this->Eye2_centerX-5, this->Eye2_centerY-12, 4);
    delay(1500);


}

/*
    * Function to animate chagnging the size of Eye.
    * @param EyeSelect - 1 for left eye, 2 for right eye, 0 for both eyes
    * @param Redction_pct - Percentage of eye closed
    * @param Frames - Number of frames to animate the change
    * @param tft - TFT_eSPI object to draw on
*/
void EyesClass::ShrinkEye(uint8_t EyeSelect, int16_t Redction_pct, uint16_t Frames, TFT_eSPI &tft){
    //Get the current height
    float Eye1_Current_Height = this->Eye1_Current_Height;
    float Eye2_Current_Height = this->Eye2_Current_Height;
    
    //Get the final height from EYE_HEIGHT and Redction_pct
    float Eye1_Final_Height = (EYE_HEIGHT/2 * (100-Redction_pct)) / 100;
    float Eye2_Final_Height = (EYE_HEIGHT/2 * (100-Redction_pct)) / 100;
    
    //Calculate the reduction in size per-frame
    float Eye1_Reduction = (Eye1_Current_Height - Eye1_Final_Height) / Frames;
    float Eye2_Reduction = (Eye2_Current_Height - Eye2_Final_Height) / Frames;
    
    //iterate per frame and draw the eyes
    for(int i = 0; i < Frames; i++){
        //Clear the old eyes
        this->Clear_Eyes(tft);

        //calculate the new height
        Eye1_Current_Height -= Eye1_Reduction;
        Eye2_Current_Height -= Eye2_Reduction;

        //update the current height
        this->Eye1_Current_Height = (EyeSelect == 1 || EyeSelect == 0)?floor(Eye1_Current_Height): this->Eye1_Current_Height;
        this->Eye2_Current_Height = (EyeSelect == 2 || EyeSelect == 0)?floor(Eye2_Current_Height): this->Eye2_Current_Height;
        
        //Now draw the new eyes
        this->Draw_Eyes(tft);
        //delay
        delay(50);
    }
}

/*
    * Clear the eye place
    * @param tft - TFT_eSPI object to draw on
*/
void EyesClass::Clear_Eyes(TFT_eSPI &tft){
    if(RECTANGULAR_EYES){
        //Clear the old eyes
        tft.fillRect(this->Eye1_centerX - this->Eye1_Current_Width-EYECLEAR_EXTRA, this->Eye1_centerY - this->Eye1_Current_Height-EYECLEAR_EXTRA, this->Eye1_Current_Width * 2+EYECLEAR_EXTRA*2, this->Eye1_Current_Height * 2+EYECLEAR_EXTRA*2, BG_COLOR);
        tft.fillRect(this->Eye2_centerX - this->Eye2_Current_Width-EYECLEAR_EXTRA, this->Eye2_centerY - this->Eye2_Current_Height-EYECLEAR_EXTRA, this->Eye2_Current_Width * 2+EYECLEAR_EXTRA*2, this->Eye2_Current_Height * 2+EYECLEAR_EXTRA*2, BG_COLOR);

    }
    else{
        //Clear the old eyes
        tft.fillEllipse(this->Eye1_centerX, this->Eye1_centerY, this->Eye1_Current_Width, this->Eye1_Current_Height, BG_COLOR);
        tft.fillEllipse(this->Eye2_centerX, this->Eye2_centerY, this->Eye2_Current_Width, this->Eye2_Current_Height, BG_COLOR);
    }
}

/*
    * Draw the eyes
    * @param tft - TFT_eSPI object to draw on
*/
void EyesClass::Draw_Eyes(TFT_eSPI &tft){
  //If the Eye width/hieghts are 0, then just make a black box to clear the eyes
  if(this->Eye1_Current_Width == 0 || this->Eye1_Current_Height == 0){
      tft.fillRect(this->Eye1_centerX - EYE_WIDTH/2, this->Eye1_centerY - EYE_HEIGHT, EYE_WIDTH, EYE_HEIGHT, BG_COLOR);
  }
  else{
    if(RECTANGULAR_EYES){
    //Clear the old eyes
        tft.fillRect(this->Eye1_centerX - this->Eye1_Current_Width, this->Eye1_centerY - this->Eye1_Current_Height, this->Eye1_Current_Width * 2, this->Eye1_Current_Height * 2, this->Eye_Color);
    }
    else{
        //Clear the old eyes
        tft.fillEllipse(this->Eye1_centerX, this->Eye1_centerY, this->Eye1_Current_Width, this->Eye1_Current_Height, this->Eye_Color);
    }
    //Draw the iris 1
    if(DRAWIRIS){
        tft.fillCircle(this->Iris1_centerX, this->Iris1_centerY, this->Iris_radius, this->Iris_Color);
    }
    //Draw the expression based on the mood state
    switch(this->Mood_State){
        case Happy:
            //First cut the bottom of the eye
            tft.fillRect(this->Eye1_centerX - this->Eye1_Current_Width, this->Eye1_centerY, this->Eye1_Current_Width*2, this->Eye1_Current_Height, BG_COLOR);
            //Draw the happy expression
            tft.fillRoundRect(this->Eye1_centerX - this->Eye1_Current_Width, this->Eye1_centerY - this->Eye1_Current_Height - HAPPY_EYE_RADIUS, this->Eye1_Current_Width*2, this->Eye1_Current_Height/2, HAPPY_EYE_RADIUS, this->Eye_Color);
            //make a black box to clear the bottom of the eye
            tft.fillEllipse(this->Eye1_centerX-HAPPY_EYE_RADIUS, this->Eye1_centerY+this->Eye1_Current_Height/2, this->Eye1_Current_Width, this->Eye1_Current_Height+3, BG_COLOR);
            break;
        case Sad:
            //Draw the sad expression
            tft.fillCircle(this->Eye1_centerX - EYE_WIDTH/4, this->Eye1_centerY + EYE_HEIGHT/4, EYE_WIDTH/8, this->Eye_Color);
            break;
        case Angry:
            //Draw the angry expression
            tft.fillCircle(this->Eye1_centerX - EYE_WIDTH/4, this->Eye1_centerY - EYE_HEIGHT/4, EYE_WIDTH/8, this->Eye_Color);
            break;
        default:
            break;
    }
  }
  
  if(this->Eye2_Current_Width == 0 || this->Eye2_Current_Height == 0){
      tft.fillRect(this->Eye2_centerX - EYE_WIDTH/2, this->Eye2_centerY - EYE_HEIGHT, EYE_WIDTH, EYE_HEIGHT, BG_COLOR);
  }
  else{
    if(RECTANGULAR_EYES){
    //Clear the old eyes
        tft.fillRect(this->Eye2_centerX - this->Eye2_Current_Width, this->Eye2_centerY - this->Eye2_Current_Height, this->Eye2_Current_Width * 2, this->Eye2_Current_Height * 2, this->Eye_Color);
    }
    else{
        //Clear the old eyes
        tft.fillEllipse(this->Eye2_centerX, this->Eye2_centerY, this->Eye2_Current_Width, this->Eye2_Current_Height, this->Eye_Color);
    }
    //Draw the iris 2
    if(DRAWIRIS){
        tft.fillCircle(this->Iris2_centerX, this->Iris2_centerY, this->Iris_radius, this->Iris_Color);
    }

    //Draw the expression based on the mood state
    switch(this->Mood_State){
        case Happy:
            //First cut the bottom of the eye
            tft.fillRect(this->Eye2_centerX - this->Eye2_Current_Width, this->Eye2_centerY, this->Eye2_Current_Width*2, this->Eye2_Current_Height, BG_COLOR);
            //Draw the happy expression
            tft.fillRoundRect(this->Eye2_centerX - this->Eye2_Current_Width, this->Eye2_centerY - this->Eye2_Current_Height - HAPPY_EYE_RADIUS, this->Eye2_Current_Width*2, this->Eye2_Current_Height/2, HAPPY_EYE_RADIUS, this->Eye_Color);
            //make a black box to clear the bottom of the eye
            tft.fillEllipse(this->Eye2_centerX+HAPPY_EYE_RADIUS, this->Eye2_centerY+this->Eye2_Current_Height/2, this->Eye2_Current_Width, this->Eye2_Current_Height+3, BG_COLOR);

            break;
        case Sad:
            //Draw the sad expression
            tft.fillCircle(this->Eye2_centerX - EYE_WIDTH/4, this->Eye2_centerY + EYE_HEIGHT/4, EYE_WIDTH/8, this->Eye_Color);
            break;
        case Angry:
            //Draw the angry expression
            tft.fillCircle(this->Eye2_centerX - EYE_WIDTH/4, this->Eye2_centerY - EYE_HEIGHT/4, EYE_WIDTH/8, this->Eye_Color);
            break;
        default:
            break;
    }
  }
}

/*
    * Move the eyes
    * @param Eye1_x - X coordinate of the first eye
    * @param Eye1_y - Y coordinate of the first eye
    * @param Eye2_x - X coordinate of the second eye
    * @param Eye2_y - Y coordinate of the second eye
*/
void EyesClass::Move_Eyes(int Eye1_x, int Eye1_y, int Eye2_x, int Eye2_y){
  //Move the eyes
  this->Eye1_centerX = Eye1_x;
  this->Eye1_centerY = Eye1_y;
  this->Eye2_centerX = Eye2_x;
  this->Eye2_centerY = Eye2_y;
}


/*
    *Function to Animate moving the eyes
    * @param tft - reference to the tft object
    * @param Eye1_x - Final X coordinate of the first eye
    * @param Eye1_y - Final Y coordinate of the first eye
    * @param Eye2_x - Final X coordinate of the second eye
    * @param Eye2_y - Final Y coordinate of the second eye
    * @param Frames - Number of frames to move the eyes in
*/
void EyesClass::Move_Eyes(TFT_eSPI &tft, int Eye1_x, int Eye1_y, int Eye2_x, int Eye2_y, uint16_t Frames){
    //Move the eyes
    float Eye1_X_Diff = (Eye1_x - this->Eye1_centerX)/(Frames-1);
    float Eye1_Y_Diff = (Eye1_y - this->Eye1_centerY)/(Frames-1);
    float Eye2_X_Diff = (Eye2_x - this->Eye2_centerX)/(Frames-1);
    float Eye2_Y_Diff = (Eye2_y - this->Eye2_centerY)/(Frames-1);
    for(int i = 0; i < Frames-1; i++){
        this->Clear_Eyes(tft);
        this->Move_Eyes(floor(this->Eye1_centerX + Eye1_X_Diff), floor(this->Eye1_centerY + Eye1_Y_Diff), floor(this->Eye2_centerX + Eye2_X_Diff), floor(this->Eye2_centerY + Eye2_Y_Diff));
        this->Draw_Eyes(tft);
        delay(ANIMATION_DELAY);
    }
    this->Move_Eyes(Eye1_x, Eye1_y, Eye2_x, Eye2_y);
    this->Draw_Eyes(tft);
    delay(ANIMATION_DELAY);
}


/*
        ////////////////OLD CODE NOT USED/////////////////////
    //Draw the 1st iris 
    if(this->Eye1_Current_Height < this->Iris_radius && this->Eye1_Current_Width < this->Iris_radius){ //check is both height and width are less than the radius
        tft.fillCircle(this->Iris1_centerX, this->Iris1_centerY, this->Iris_radius, this->Iris_Color);
        //now cover the exceeding part with background color
        tft.fillRect(this->Iris1_centerX - this->Iris_radius, this->Iris1_centerY - this->Iris_radius, this->Iris_radius - this->Eye1_Current_Width, this->Iris_radius - this->Eye1_Current_Height, BG_COLOR);
    }
    else if(this->Eye1_Current_Height < this->Iris_radius){ //check if the height is less than the radius
        tft.fillCircle(this->Iris1_centerX, this->Iris1_centerY, this->Iris_radius, this->Iris_Color);
        //now cover the exceeding part with background color
        tft.fillRect(this->Iris1_centerX - this->Iris_radius, this->Iris1_centerY - this->Iris_radius, this->Iris_radius * 2, this->Iris_radius - this->Eye1_Current_Height, BG_COLOR);
        tft.fillRect(this->Iris1_centerX - this->Iris_radius, this->Iris1_centerY + this->Iris_radius, this->Iris_radius * 2, this->Iris_radius - this->Eye1_Current_Height, BG_COLOR);
    }
    else if(this->Eye1_Current_Width < this->Iris_radius){ //check if the width is less than the radius
        tft.fillCircle(this->Iris1_centerX, this->Iris1_centerY, this->Iris_radius, this->Iris_Color);
        //now cover the exceeding part with background color
        tft.fillRect(this->Iris1_centerX - this->Iris_radius, this->Iris1_centerY - this->Iris_radius, this->Iris_radius - this->Eye1_Current_Width, this->Iris_radius * 2, BG_COLOR);
    }
    else{   //if both height and width are greater than the radius
        tft.fillCircle(this->Iris1_centerX, this->Iris1_centerY, this->Iris_radius, this->Iris_Color);
    }

    //Draw the 2nd iris
    if(this->Eye2_Current_Height < this->Iris_radius && this->Eye2_Current_Width < this->Iris_radius){ //check is both height and width are less than the radius
        tft.fillCircle(this->Iris2_centerX, this->Iris2_centerY, this->Iris_radius, this->Iris_Color);
        //now cover the exceeding part with background color
        tft.fillRect(this->Iris2_centerX - this->Iris_radius, this->Iris2_centerY - this->Iris_radius, this->Iris_radius - this->Eye2_Current_Width, this->Iris_radius - this->Eye2_Current_Height, BG_COLOR);
    }
    else if(this->Eye2_Current_Height < this->Iris_radius){ //check if the height is less than the radius
        tft.fillCircle(this->Iris2_centerX, this->Iris2_centerY, this->Iris_radius, this->Iris_Color);
        //now cover the exceeding part with background color
        tft.fillRect(this->Iris2_centerX - this->Iris_radius, this->Iris2_centerY - this->Iris_radius, this->Iris_radius * 2, this->Iris_radius - this->Eye2_Current_Height, BG_COLOR);
        tft.fillRect(this->Iris2_centerX - this->Iris_radius, this->Iris2_centerY + this->Iris_radius, this->Iris_radius * 2, this->Iris_radius - this->Eye2_Current_Height, BG_COLOR);
    }
    else if(this->Eye2_Current_Width < this->Iris_radius){ //check if the width is less than the radius
        tft.fillCircle(this->Iris2_centerX, this->Iris2_centerY, this->Iris_radius, this->Iris_Color);
        //now cover the exceeding part with background color
        tft.fillRect(this->Iris2_centerX - this->Iris_radius, this->Iris2_centerY - this->Iris_radius, this->Iris_radius - this->Eye2_Current_Width, this->Iris_radius * 2, BG_COLOR);
    }
    else{   //if both height and width are greater than the radius
        tft.fillCircle(this->Iris2_centerX, this->Iris2_centerY, this->Iris_radius, this->Iris_Color);
    }    

*/