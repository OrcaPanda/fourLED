//----------------------------------------------------------------------------------------------------
// File: DisplayMatrixController.cpp
// Date: 03/27/2020
// Desc: Source file for the DisplayMatrixController class
//----------------------------------------------------------------------------------------------------
#include "DisplayMatrixController.hpp"

//----------------------------------------------------------------------------------------------------
// Function: getInstance
// Desc: constructs the DisplayController object if it does not exist, otherwise return the instance
//----------------------------------------------------------------------------------------------------
DisplayMatrixController* DisplayMatrixController::displayMatrixController_ = nullptr;

DisplayMatrixController* DisplayMatrixController::getInstance(void){
    if(!DisplayMatrixController::displayMatrixController_){
        DisplayMatrixController::displayMatrixController_ = new DisplayMatrixController();
    }
    return DisplayMatrixController::displayMatrixController_;
}

//----------------------------------------------------------------------------------------------------
// Function: DisplayMatrixController (constructor)
// Desc: constructs the DisplayMatrixController object
//----------------------------------------------------------------------------------------------------
DisplayMatrixController::DisplayMatrixController(){
    // Initialize the SPI bus
    pinMode(SPI_CS_R, OUTPUT);
    digitalWrite(SPI_CS_R, HIGH);
    pinMode(SPI_CS_G, OUTPUT);
    digitalWrite(SPI_CS_G, HIGH);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);

    // Clear the display, set the scan limit, and set the intensity
    this->clearDisplay(RED);
    this->clearDisplay(GREEN);
    this->setIntensity(RED, DEFAULT_RED_INTENSITY);
    this->setIntensity(GREEN, DEFAULT_GREEN_INTENSITY);
    this->enableFullScanLimit();

    // Setup the refresh timer interrupt and attach the refresh subroutine
    Timer1.initialize((unsigned long)((1000000 / 2) / REFRESH_RATE_HZ));
    Timer1.attachInterrupt(&DisplayMatrixController::refresh);
}

//----------------------------------------------------------------------------------------------------
// Function: ~DisplayMatrixController (destructor)
// Desc: destructs the DisplayMatrixController object
//----------------------------------------------------------------------------------------------------
DisplayMatrixController::~DisplayMatrixController(){
    // Disables the timer interrupts
    Timer1.stop();
    Timer1.detachInterrupt();
    
    // Disables the SPI bus
    SPI.end();
    pinMode(SPI_CS_G, INPUT);
    pinMode(SPI_CS_R, INPUT);
}

//----------------------------------------------------------------------------------------------------
// Function: refresh
// Desc: destructs the DisplayMatrixController object
//----------------------------------------------------------------------------------------------------
volatile bool DisplayMatrixController::colorToDraw_ = (bool)RED;

void DisplayMatrixController::refresh(void){
    // Return if the display controller does not exist
    if(!DisplayMatrixController::displayMatrixController_){
        return;
    }

    // Update the display for the next color
    if(DisplayMatrixController::colorToDraw_ == (bool)RED){
        DisplayMatrixController::displayMatrixController_->redrawColor(RED);
        DisplayMatrixController::displayMatrixController_->shutdown(GREEN, true);
        DisplayMatrixController::displayMatrixController_->shutdown(RED, false);
    } else{
        DisplayMatrixController::displayMatrixController_->redrawColor(GREEN);
        DisplayMatrixController::displayMatrixController_->shutdown(RED, true);
        DisplayMatrixController::displayMatrixController_->shutdown(GREEN, false);
    }

    // Toggle the next color to draw
    DisplayMatrixController::colorToDraw_^= 1;
}

//----------------------------------------------------------------------------------------------------
// Function: redraw
// Desc: redraws the stored bitmap to the LED matrix controllers
//----------------------------------------------------------------------------------------------------
volatile uint8_t DisplayMatrixController::bitmap_[2][4][8] = {0};

void DisplayMatrixController::redrawColor(int color){
    for(uint16_t row = 0x01; row <= 0x08; row++){
        digitalWrite(color == RED ? SPI_CS_R : SPI_CS_G, LOW);
        for(int i = 3; i >= 0; i--){
            // Correct for the DP segment being in the MSB position by barrel shifting right 1 bit
            uint8_t correctedByte = (DisplayMatrixController::bitmap_[color][i][row-1] << 7) | (DisplayMatrixController::bitmap_[color][i][row-1] >> 1);
            SPI.transfer16(row << 8 | (uint16_t)correctedByte);
        }
        digitalWrite(color == RED ? SPI_CS_R : SPI_CS_G, HIGH);
    }
}

//----------------------------------------------------------------------------------------------------
// Function: shutdown
// Desc: shuts down or enables the specified display color
//----------------------------------------------------------------------------------------------------
void DisplayMatrixController::shutdown(int color, bool state){
    digitalWrite(color == RED ? SPI_CS_R : SPI_CS_G, LOW);
    uint16_t command = state ? 0x0C00 : 0x0C01;
    for(int i = 0; i < 4; i++){
        SPI.transfer16(command);
    }
    digitalWrite(color == RED ? SPI_CS_R : SPI_CS_G, HIGH);
}

//----------------------------------------------------------------------------------------------------
// Function: clearDisplay
// Desc: clears the display for the specified display color
//----------------------------------------------------------------------------------------------------
void DisplayMatrixController::clearDisplay(int color){
    for(uint16_t row = 1; row <= 8; row++){
        digitalWrite(color == RED ? SPI_CS_R : SPI_CS_G, LOW);
        for(uint8_t i = 0; i < 4; i++){
            // Write zeros to all the digit registers
            SPI.transfer16(row << 8);
        }
        digitalWrite(color == RED ? SPI_CS_R : SPI_CS_G, HIGH);
    }
}

//----------------------------------------------------------------------------------------------------
// Function: enableFullScanLimit
// Desc: enables all rows in the scan
//----------------------------------------------------------------------------------------------------
void DisplayMatrixController::enableFullScanLimit(void){
    // Red LEDs
    digitalWrite(SPI_CS_R, LOW);
    for(uint8_t i = 0; i < 4; i++){
        SPI.transfer16((uint16_t)0x0B07);
    }
    digitalWrite(SPI_CS_R, HIGH);

    // Green LEDs
    digitalWrite(SPI_CS_G, LOW);
    for(uint8_t i = 0; i < 4; i++){
        SPI.transfer16((uint16_t)0x0B07);
    }
    digitalWrite(SPI_CS_G, HIGH);
}

//----------------------------------------------------------------------------------------------------
// Function: setIntensity
// Desc: sets the color to the specified intensity
//----------------------------------------------------------------------------------------------------
void DisplayMatrixController::setIntensity(int color, uint16_t intensity){
    intensity = intensity > 15 ? 15 : intensity;

    digitalWrite(color == RED ? SPI_CS_R : SPI_CS_G, LOW);
    for(uint8_t i = 0; i < 4; i++){
        SPI.transfer16((uint16_t)0x0A00 | intensity);
    }
    digitalWrite(color == RED ? SPI_CS_R : SPI_CS_G, HIGH);
}