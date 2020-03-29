//----------------------------------------------------------------------------------------------------
// File: PushButtonController.hpp
// Date: 03/28/2020
// Desc: Header file for the PushButtonController class
//----------------------------------------------------------------------------------------------------
#ifndef PUSHBUTTONCONTROLLER_H
#define PUSHBUTTONCONTROLLER_H

// Third party library includes
#include <ArduinoSTL.h>

// Standard library includes
#include <functional>
#include <algorithm>
#include <vector>

// Project-specific includes
#include "common.hpp"

//----------------------------------------------------------------------------------------------------
// Class definition
//----------------------------------------------------------------------------------------------------
class PushButtonController{
public:
	// Static method for obtaining class instance 
	static PushButtonController* getInstance(void);
	
	// Singleton implementation. Don't implement copy constructor and assignment operator
	PushButtonController(PushButtonController const&) = delete;
	PushButtonController& operator=(PushButtonController const&) = delete;

	// Destructor
	~PushButtonController(void);

	// Reading buttons
	uint8_t readButtons(void);

	// Interrupt handler
	static void interruptHandler(void);

	// Registering interrupts
	void registerInterrupt(std::function<void(uint8_t)>);

private:
	PushButtonController(void);
	static PushButtonController* pushButtonController_;

	std::vector<std::function<void(uint8_t)>> interruptVector_;
};

#endif