typedef struct
{
	int x;
	int y;
} XY;

typedef struct
{
	const unsigned short* sprite;
	volatile int pos[];
} Sprite;

#define TRUE 1;
#define FALSE 0;

extern volatile unsigned int level;




void generateNewLevel();
void generateLevel();
void setPos(int x, int y, const unsigned int* image);
void updateChar();
void generateFloor();
void generateWalls();
void generateExit();
void updateEntities();
void eraseLastMove(int oldX, int oldY);
int checkDeath();
void checkExit();
void drawLevel();
void generateMobLoc(int pos[2]);
void targetedMove(int pos[2], int target[2], const unsigned short* sprite);
void randMove(int pos[2], const unsigned short* sprite);
int collideCheck(int pos[2]);
void generateKey();
void generateSprites();
void updateSprite(int pos[2], const unsigned short* sprite);
int startGame();


