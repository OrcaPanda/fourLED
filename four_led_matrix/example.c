#include "fourLED.h"
#include "example.h"

volatile int state = 0;

void my_example(void){
	// This example alternates between red and green depending on a button push
	int row, col;
	char colour;
	while(1){
		if(state){
			colour = 'r';	
		} else {
			colour = 'g';
		}
		for(row = 0; row < 16; row++){
			for(col = 0; col < 16; col++){
				//M is a global variable representing what should be written to the display
				//M can take on values of 0, 'r', 'g', for off, red, and green, respectively
				//M is written asynchronously, with each colour channel updated once every 20ms
				M[row][col] = colour;
			}
		}
	}	
}

void PB_A_my_example(void){
	// Callback for Button A
	state ^= 1;
}
void PB_B_my_example(void){
	// Callback for Button B
}
void PB_1_my_example(void){
	// Callback for Button 1
}
void PB_2_my_example(void){
	// Callback for Button 2
}
void PB_3_my_example(void){
	// Callback for Button 3
}
void PB_4_my_example(void){
	// Callback for Button 4
}

