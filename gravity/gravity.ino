#include "DisplayMatrixController.hpp"

//----------------------------------------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------------------------------------
DisplayMatrixController* displayMatrixController;

//----------------------------------------------------------------------------------------------------
// Setup
//----------------------------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  displayMatrixController = DisplayMatrixController::getInstance();
  
}

//----------------------------------------------------------------------------------------------------
// Loop
//----------------------------------------------------------------------------------------------------
void loop() {
  delay(500);
  DisplayMatrixController::bitmap_[RED][2][0] = 0x81;
  DisplayMatrixController::bitmap_[GREEN][2][0] = 0x40;
  delay(500);
  DisplayMatrixController::bitmap_[RED][2][0] = 0x01;
  DisplayMatrixController::bitmap_[GREEN][2][0] = 0x00;
} 
