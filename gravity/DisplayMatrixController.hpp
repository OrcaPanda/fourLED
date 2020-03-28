//----------------------------------------------------------------------------------------------------
// File: DisplayMatrixController.hpp
// Date: 03/27/2020
// Desc: Header file for the DisplayMatrixController class
//----------------------------------------------------------------------------------------------------
#ifndef DISPLAYMATRIXCONTROLLER_H
#define DISPLAYMATRIXCONTROLLER_H

// Third party library includes
#include <TimerOne.h>
#include <SPI.h>

// Project-specific includes
#include "common.hpp"

//----------------------------------------------------------------------------------------------------
// Definitions
//----------------------------------------------------------------------------------------------------
#define REFRESH_RATE_HZ 100
#define DEFAULT_RED_INTENSITY 3
#define DEFAULT_GREEN_INTENSITY 2

//----------------------------------------------------------------------------------------------------
// Class definition
//----------------------------------------------------------------------------------------------------
class DisplayMatrixController{
public:
	// Static method for obtaining class instance 
	static DisplayMatrixController* getInstance(void);
	
	// Singleton implementation. Don't implement copy constructor and assignment operator
	DisplayMatrixController(DisplayMatrixController const&) = delete;
	DisplayMatrixController& operator=(DisplayMatrixController const&) = delete;

	// Destructor
	~DisplayMatrixController(void);
	
	// Stored in row format
	volatile static uint8_t bitmap_[2][4][8];

	// Color intensity
	void setIntensity(int, uint16_t);
	
private:
	DisplayMatrixController(void);
	static DisplayMatrixController* displayMatrixController_;

	// Static function to use with Timer.attachInterrupt
	volatile static bool colorToDraw_;
	static void refresh(void);

	// Bitmap operations
	void redrawColor(int);

	// Shutdown function
	void shutdown(int, bool);

	// Clear display
	void clearDisplay(int);

	// Set scan limit
	void enableFullScanLimit(void);
};

#endif