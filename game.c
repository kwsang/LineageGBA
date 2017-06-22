#include "game.h"
#include "myLib.h"
#include "images.h"
#include <stdlib.h>
#include <stdio.h>

//global variables
int charPos[2] = {0,0};
int exitPos[2] = {12,-1};
int entrancePos[2] = {0,8};
int oldPos[2] = {0,0};
int batPos[2] = {0,0};
int dogPos[2] = {0,0};
int monkeyPos[2] = {0,0};
int keyPos[2] = {0,0};
int snakePos[2] = {0,0};
int keyDown = 0;
char buffer[41];

u16 keyCurr = 0, keyPrev = 0;
volatile unsigned int level = 0;
int skip = 0;

//main game loop
int startGame() {
	int game = 0;
	if (!game)
		{
			generateNewLevel();
			game = 1;
			while (1) {
				waitForVBlank();
				//if char is dead
				if (checkDeath()) {
					//return to main loop
					drawImage3(0,0,240,160,gameover);
					sprintf(buffer, "START to CONTINUE");
					drawString(55, 70, buffer, WHITE);
					sprintf(buffer, "SELECT to RESTART");
					drawString(75, 70, buffer, WHITE);
					return 1;
				}
				// if select pressed
				if (KEY_DOWN_NOW(BUTTON_SELECT)) {
					//return to main loop
					return 0;
				}
				updateChar();
				generateSprites();
				updateEntities();
				
				delay(1);
			}
		}
	return 0;
}

//draws current level at the top of the screen
void drawLevel() {
	sprintf(buffer, "Level: %d", level+1);
	drawString(8, 60, buffer, WHITE);
}

//reset variables
void generateNewLevel() {
	charPos[0] = 0;
	charPos[1] = 7;
	//generate random mob locations
	generateMobLoc(batPos);
	generateMobLoc(dogPos);
	generateMobLoc(monkeyPos);
	generateMobLoc(snakePos);
	generateMobLoc(keyPos);
	generateLevel();
}

//generates a mob location
void generateMobLoc(int pos[2]) {
	pos[0] = rand()%12;
	pos[1] = rand()%7;
	//if mob is near char, move left (no longer using recursion to limit rand use for performance)
	if (pos[0] < 3 && pos[1] > 4) {
		generateMobLoc(pos);
	}
}

//calls necessary level functions
void generateLevel() {
	generateWalls();
	drawLevel();
	generateFloor();
	generateSprites();
	generateExit();
}

//draws mobs in initial positions
void generateSprites() {
	updateSprite(batPos, bat);
	if (level > 2) {
		updateSprite(dogPos, dog);
	}
	if (level > 4) {
		updateSprite(monkeyPos, monkey);
	}
	if (level > 6) {
		updateSprite(snakePos, snake);
	}
}

//generates surrounding walls, static for each level
void generateWalls() {
	//west and east walls
	for (int i = 2; i < 10; i++) {
		drawTile(1,i,wallW);
		drawTile(15,i,wallE);
	}
	//north and south walls
	for (int i = 2; i < 15; i++) {
		drawTile(i,1,wallN);
		drawTile(i,10,wallS);
	}
	//corner walls
	drawTile(1,1,wallNW);
	drawTile(15,1,wallNE);
	drawTile(15,10,wallSE);
	drawTile(1,10,wallSW);
}

//generate floor tiles based on level	
void generateFloor() {
	for (int l = 1; l < 14; l++) {
		for (int h = 1; h < 9; h++) {
			// change floor tile based on level
			switch (level%7) {
				case 0:
					drawImage3(h*16, l*16, 16, 16, grass4);
					break;
				case 1:
					drawImage3(h*16, l*16, 16, 16, grass1);
					break;
				case 2:
					drawImage3(h*16, l*16, 16, 16, grass2);
					break;
				case 3:
					drawImage3(h*16, l*16, 16, 16, grass3);
					break;
				case 4:
					drawImage3(h*16, l*16, 16, 16, floor1);
					break;
				case 5:
					drawImage3(h*16, l*16, 16, 16, floor2);
					break;
				case 6:
					drawImage3(h*16, l*16, 16, 16, floor3);
					break;
			}
		}
	}
}

void updateChar() {
	//if no movement key pressed, allow for key press
	if (!KEY_DOWN_NOW(BUTTON_LEFT)
		&& !KEY_DOWN_NOW(BUTTON_RIGHT)
		&& !KEY_DOWN_NOW(BUTTON_UP)
		&& !KEY_DOWN_NOW(BUTTON_DOWN)) {
		keyDown = 0;
	}
	//if left button pressed
	if (!keyDown && KEY_DOWN_NOW(BUTTON_LEFT)) {
		//if character is not on far left side
		if (charPos[0] > 0) {
			//move character position left
			oldPos[0] = charPos[0];
			oldPos[1] = charPos[1];
			charPos[0] -= 1;
		}
		//if character is not in the same position
		if (!(charPos[0] == oldPos[0] && charPos[1] == oldPos[1])) {
			//retile old character position
			eraseLastMove(oldPos[0], oldPos[1]);
		}
		//disable continous key press
		keyDown = 1;
		//if up key is pressed
	} else if (!keyDown && KEY_DOWN_NOW(BUTTON_RIGHT)) {
		if (charPos[0] < 12) {
			oldPos[0] = charPos[0];
			oldPos[1] = charPos[1];
			charPos[0] += 1;
		}
		//if char is not in same pos
		if (!(charPos[0] == oldPos[0] && charPos[1] == oldPos[1])) {
			eraseLastMove(oldPos[0], oldPos[1]);
		}
		//disable continuous press
		keyDown = 1;
		//if up key is pressed
	} else if (!keyDown && KEY_DOWN_NOW(BUTTON_UP)) {
		keyDown = 1;
		//if char is not too far up or is at exit location
		if (charPos[1] > 0 || (charPos[0] == 12 && charPos[1] > -1 && keyPos[1] == -1)) {
			oldPos[0] = charPos[0];
			oldPos[1] = charPos[1];
			charPos[1] -= 1;
		}
		// check if char can exit level
		checkExit();
		//if char is not in same pos, clear last tile
		if (!(charPos[0] == oldPos[0] && charPos[1] == oldPos[1])) {
			eraseLastMove(oldPos[0], oldPos[1]);
		}
		//if down key is pressed
	} else if (!keyDown && KEY_DOWN_NOW(BUTTON_DOWN)) {
		//if char is not too far down (out of bounds)
		if (charPos[1] < 7) {
			oldPos[0] = charPos[0];
			oldPos[1] = charPos[1];
			charPos[1] = charPos[1] + 1;
			//if char is not in the same pos
			if (!(charPos[0] == oldPos[0] && charPos[1] == oldPos[1])) {
				eraseLastMove(oldPos[0], oldPos[1]);
			}
		}
		keyDown = 1;
	}
	//draw char at new pos
	drawSprite3(charPos[0],charPos[1],char1);
	//if char has obtained key
	if (charPos[0] == keyPos[0] && charPos[1] == keyPos[1]) {
		keyPos[0] = 0;
		keyPos[1] = -1;
		//draw key in top left, open door
		drawSprite3(keyPos[0],keyPos[1],key);
		drawSprite3(12,-1,door_open);
	}
}

//check if key is obtained and char exits
void checkExit() {
	if (charPos[0] == 12 && charPos[1] == -1 && keyPos[0] == 0 && keyPos[1] == -1) {
		level += 1;
		generateNewLevel();
	}
}

//erase last tile
void eraseLastMove(int oldX, int oldY) {
	switch (level%7) {
		case 0:
			drawSprite3(oldX,oldY,grass4);
			break;
		case 1:
			drawSprite3(oldX,oldY,grass1);
			break;
		case 2:
			drawSprite3(oldX,oldY,grass2);
			break;
		case 3:
			drawSprite3(oldX,oldY,grass3);
			break;
		case 4:
			drawSprite3(oldX,oldY,floor1);
			break;
		case 5:
			drawSprite3(oldX,oldY,floor2);
			break;
		case 6:
			drawSprite3(oldX,oldY,floor3);
			break;
		}
	if (key[0] == oldX && key[1] == oldY) {
		drawSprite3(oldX, oldY, key);
	}
}

//creates exit in top right corner
void generateExit() {
	drawSprite3(exitPos[0],exitPos[1],door_closed);
}

//allows entities to move
void updateEntities() {
	drawSprite3(keyPos[0], keyPos[1], key);
	if (rand()%9 == 2 || (level > 10 && rand()%8 == 2)) {
		randMove(batPos, bat);
	}
	if ((level > 2 && rand()%8 == 2) || (level > 12 && rand()% 8 == 2)) {
		randMove(dogPos, dog);
	}
	if ((level > 4 && rand()%12 == 2) || (level > 16 && rand()% 9 == 2)) {
		randMove(monkeyPos, monkey);
	}
	if ((level > 6 && rand()%14 == 2) || (level > 20 && rand()% 9 == 2)) {
		targetedMove(snakePos, charPos, snake);
	}
}

//randomly allow mob to move
void randMove(int pos[2], const unsigned short* sprite) {
	oldPos[0] = pos[0];
		oldPos[1] = pos[1];
		int move = rand()%4;
		switch (move) {
			case 0:
				if (pos[0] > 0) {
					pos[0] = pos[0] - 1;
					
				}
				break;
			case 1:
				if (pos[0] < 12) {
					pos[0] = pos[0] + 1;
				}
				break;
			case 2:
				if (pos[1] > 0) {
					pos[1] = pos[1]- 1;
				}
				break;
			case 3:
				if (pos[1] < 7) {
					pos[1] = pos[1] + 1;
				}
				break;
		}
		updateSprite(pos, sprite);
}

//make mob move in a targeted manner
void targetedMove(int pos[2], int target[2], const unsigned short* sprite) {
	int moveSpd = rand()%4;
	if (level > 8) {
		moveSpd = rand()%2;
	}
	if (moveSpd == 1) {
		oldPos[0] = pos[0];
		oldPos[1] = pos[1];
		int diffX = pos[0] - target[0];
		int diffY = pos[1] - target[1];
		if (abs(diffX) > abs(diffY)) {
			if (diffX > 0) {
				if (pos[0] > 0) {pos[0] = pos[0] -1;}
			} else {
				if (pos[0] < 12) {pos[0] = pos[0] +1;}
			}
		} else {
			if (diffY > 0) {
				if (pos[1] > 0) {pos[1] = pos[1] -1;}
			} else {
				if (pos[1] < 7) {pos[1] = pos[1] +1;}
			}
		}
		updateSprite(pos, sprite);
	}
}

//check if char has died
int checkDeath() {
	if (collideCheck(batPos)) {
		return 1;
	} else if (level > 2 && collideCheck(dogPos)) {
		return 1;
	} else if (level > 4 && collideCheck(monkeyPos)) {
		return 1;
	} else if (level > 6 && collideCheck(snakePos)) {
		return 1;
	} else {
		return 0;
	}
}

//check if char has collited with a certain position
int collideCheck(int pos[2]) {
	if (charPos[0] == pos[0] && charPos[1] == pos[1]) {
		return 1;
	} else {
		return 0;
	}
}

//update sprite location, clears previous tile
void updateSprite(int pos[2], const unsigned short* sprite) {
	if (!(oldPos[0] == pos[0] && oldPos[1] == pos[1])) {
		drawSprite3(pos[0],pos[1],sprite);
		eraseLastMove(oldPos[0], oldPos[1]);
	}
}