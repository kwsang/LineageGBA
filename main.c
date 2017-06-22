//main.c
//Name: Quang Vo

#include "myLib.h"
#include "images.h"
#include "game.h"
#include <stdlib.h> //required for rand
#include <stdio.h>


char buffer[41];

//main 
int main() {
	REG_DISPCNT = MODE3 | BG2_ENABLE;
 	int start = 0;
 	//draw start menu
 	drawImage3(0,0,TITLE_WIDTH,TITLE_HEIGHT,title);
	while(1)
	{

		// if the game has not started
		if (!start)
		{
			
			//if start pressed
			if (KEY_DOWN_NOW(BUTTON_START))
			{
				//set game state to 1
				start = 1;
				//start game, waits for gameover val
				if (startGame()) {
					//draw gameover screen
					
				} else {
					//draw title screen
					drawImage3(0,0,240,160,title);
				}
				start = 0;
			}
			//if select pressed, set game state to 0
			if (KEY_DOWN_NOW(BUTTON_SELECT)) {
				drawImage3(0,0,TITLE_WIDTH,TITLE_HEIGHT,title);
				level = 0;
				start = 0;
			}
		}
		
	}
	return 0;
}



