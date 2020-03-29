//----------------------------------------------------------------------------------------------------
// File: PushButtonController.cpp
// Date: 03/28/2020
// Desc: Source file for the PushButtonController class
//----------------------------------------------------------------------------------------------------
#include "PushButtonController.hpp"

//----------------------------------------------------------------------------------------------------
// Function: getInstance
// Desc: constructs the PushButtonController object if it does not exist, otherwise return the instance
//----------------------------------------------------------------------------------------------------
PushButtonController* PushButtonController::pushButtonController_ = nullptr;

PushButtonController* PushButtonController::getInstance(void){
    if(!PushButtonController::pushButtonController_){
        PushButtonController::pushButtonController_ = new PushButtonController();
    }
    return PushButtonController::pushButtonController_;
}

//----------------------------------------------------------------------------------------------------
// Function: PushButtonController (constructor)
// Desc: constructs the PushButtonController object
//----------------------------------------------------------------------------------------------------
PushButtonController::PushButtonController(void){
    // Set the buttons as inputs
    pinMode(PB_OUT_1, INPUT);
    pinMode(PB_OUT_2, INPUT);
    pinMode(PB_OUT_3, INPUT);
    pinMode(PB_OUT_4, INPUT);
    pinMode(PB_OUT_5, INPUT);
    pinMode(PB_OUT_6, INPUT);

    // Enable the push buttons
    pinMode(PB_EN, OUTPUT);
    digitalWrite(PB_EN, HIGH);

    // Enable the push button interrupts
    pinMode(PB_INT, INPUT);
    attachInterrupt(digitalPinToInterrupt(PB_INT), &PushButtonController::interruptHandler, RISING);
}

//----------------------------------------------------------------------------------------------------
// Function: ~PushButtonController (destructor)
// Desc: destructs the PushButtonController object
//----------------------------------------------------------------------------------------------------
PushButtonController::~PushButtonController(void){
    // Unregisters the push button interrupts
    detachInterrupt(digitalPinToInterrupt(PB_INT));

    // Disables the push buttons
    digitalWrite(PB_EN, LOW);
    pinMode(PB_EN, INPUT);

    // Removes the instance pointer from the class
    PushButtonController::pushButtonController_ = nullptr;
}

//----------------------------------------------------------------------------------------------------
// Function: readButtons
// Desc: reads the state of the buttons
//----------------------------------------------------------------------------------------------------
uint8_t PushButtonController::readButtons(void){
    uint8_t state = 0;
    state |= (uint8_t)digitalRead(PB_OUT_1);
    state |= (uint8_t)digitalRead(PB_OUT_2) << 1;
    state |= (uint8_t)digitalRead(PB_OUT_3) << 2;
    state |= (uint8_t)digitalRead(PB_OUT_4) << 3;
    state |= (uint8_t)digitalRead(PB_OUT_5) << 4;
    state |= (uint8_t)digitalRead(PB_OUT_6) << 5;
    return state;
}

//----------------------------------------------------------------------------------------------------
// Function: registerInterrupt
// Desc: adds one more interrupt to the interrupt vector
//----------------------------------------------------------------------------------------------------
void PushButtonController::registerInterrupt(std::function<void(uint8_t)> interrupt){
    interruptVector_.push_back(interrupt);
}

//----------------------------------------------------------------------------------------------------
// Function: interruptHandler
// Desc: static interrupt handler
//----------------------------------------------------------------------------------------------------
void PushButtonController::interruptHandler(void){
    uint8_t buttonStates = PushButtonController::pushButtonController_->readButtons();
    std::for_each(PushButtonController::pushButtonController_->interruptVector_.begin(), PushButtonController::pushButtonController_->interruptVector_.end(), [=](std::function<void(uint8_t)> interrupt){
        interrupt(buttonStates);
    });
}