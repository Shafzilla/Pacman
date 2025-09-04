#include <stm32f031x6.h>
#include <display.h>// communication with display
#include <time.h>	// timer for rand()
#include <serial.h> // serial communication
#include <stdlib.h>	// for random number
#include <sound.h>	// sound
#include <musical_notes.h>	// sound
#include <string.h>	// used for memcpy (copy arrays Ms. Pac-Man)
#include "graphics.h" 	// saving the graphics in an external file


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160

#define WALL1_X 25		// left vertical
#define WALL1_Y 40
#define WALL1_WIDTH 3
#define WALL1_HEIGHT 80

#define WALL2_X 100		// right horizontal
#define WALL2_Y 80
#define WALL2_WIDTH 27
#define WALL2_HEIGHT 3

#define WALL3_X 100		//right vertical
#define WALL3_Y 40
#define WALL3_WIDTH 3
#define WALL3_HEIGHT 80

#define WALL4_X 62		// bottom vertical
#define WALL4_Y 130
#define WALL4_WIDTH 3
#define WALL4_HEIGHT 32

#define WALL5_X 0		// left horizontal
#define WALL5_Y 80
#define WALL5_WIDTH 25
#define WALL5_HEIGHT 3

#define WALL6_X 62		// top vertical
#define WALL6_Y 0
#define WALL6_WIDTH 3
#define WALL6_HEIGHT 32

#define WALL7_X 0 		// top
#define WALL7_Y 0
#define WALL7_WIDTH 127
#define WALL7_HEIGHT 1

#define WALL8_X 127 	// right side
#define WALL8_Y 0
#define WALL8_WIDTH 1
#define WALL8_HEIGHT 160

#define WALL9_X 0		// left side
#define WALL9_Y 0
#define WALL9_WIDTH 1
#define WALL9_HEIGHT 160

#define WALL10_X 0		// bottom
#define WALL10_Y 159
#define WALL10_WIDTH 127
#define WALL10_HEIGHT 1

#define PLAYER_WIDTH 12
#define PLAYER_HEIGHT 12
#define GHOST_WIDTH 12
#define GHOST_HEIGHT 12

#define DOTS_NUM 26
#define TARGET_SCORE 260

// Character positions
uint8_t player_x = 60;			// player starting x position
uint8_t player_y = 50;  		// Player starting y position

uint8_t red_ghost_x = 100;		// red ghost starting x position
uint8_t red_ghost_y = 20;		// red ghost starting y position

uint8_t blue_ghost_x = 75;		// blue ghost starting x position
uint8_t blue_ghost_y = 130;		// blue ghost starting y position

uint8_t orange_ghost_x = 15;	// orange ghost starting x position
uint8_t orange_ghost_y = 130;	// orange ghost starting y position

uint8_t green_ghost_x = 64;		// green ghost starting x position
uint8_t green_ghost_y = 76;		// green ghost starting y position



uint8_t red_ghost_direction = 1;	// red ghost starting direction (1 = right, 0 = left)
uint8_t blue_ghost_direction = 1;	// blue ghost starting direction (1 = right, 0 = left)
uint8_t orange_ghost_direction = 1;	// orange ghost starting direction (1 = right, 0 = left)
uint8_t green_ghost_direction = 1;	// green ghost starting direction (1 = right, 0 = left)


// function definitons
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void drawMainMenu(void);
void makePointDot(void);
void makeDot();
void drawDot(uint16_t x, uint16_t y);
void clearDot( uint16_t x, uint16_t y);
void resetDot(void);
int checkDotCollection(uint16_t player_x, uint16_t player_y, uint8_t playerswitch);
int random_number(uint8_t min_num, uint8_t max_num);
void displayscore(int score);
void movePlayer(uint16_t new_x, uint16_t new_y, uint16_t hmoved, uint16_t hinverted, uint16_t vmoved, uint16_t vinverted, uint16_t toggle,uint8_t player_hit);
void makeWall1(void);
void makeWall2(void);
void makeWall3(void);
void makeWall4(void);
void makeWall5(void);
void makeWall6(void);
void makeWall7(void);
void makeWall8(void);
void makeWall9(void);
void makeWall10(void);
void moveRedGhost(void);
void moveBlueGhost(void);
void moveOrangeGhost(void);
void moveGreenGhost(void);
void renderMap(void);
void rollCredits(void);
uint8_t isCollidingWithRedGhost(uint16_t player_x, uint16_t player_y, uint16_t ghost_x, uint16_t ghost_y);
uint8_t isCollidingWithBlueGhost(uint16_t player_x, uint16_t player_y, uint16_t ghost_x, uint16_t ghost_y);
uint8_t isCollidingWithOrangeGhost(uint16_t player_x, uint16_t player_y, uint16_t ghost_x, uint16_t ghost_y);
uint8_t isCollidingWithGreenGhost(uint16_t player_x, uint16_t player_y, uint16_t ghost_x, uint16_t ghost_y);
uint8_t isCollidingWithWall1(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall2(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall3(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall4(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall5(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall6(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall7(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall8(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall9(uint16_t x, uint16_t y);
uint8_t isCollidingWithWall10(uint16_t x, uint16_t y);
void redOn(void);
void redOff(void);
void greenOn(void);
void greenOff(void);
void waka(void);
void death(void);
void death_animation(uint8_t player_x, uint8_t player_y);
const uint16_t *background_tune_notes; 
const uint16_t *background_tune_times;
uint8_t background_tune_notes_count;
uint8_t background_repeat_tune;
const uint16_t *tune_notes;
const uint16_t *tune_times;

volatile uint32_t milliseconds;

const uint16_t notes[]= // notes used in the background music
{
	B4,B5,FS5_Gb5,DS5_Eb5,B5,FS5_Gb5,DS5_Eb5,C5,C6,G5,E5,C6,G5,E5,B4,B5,FS5_Gb5,DS5_Eb5,B5,FS5_Gb5,DS5_Eb5,DS5_Eb5,E5,F5,F5,FS5_Gb5,G5,G5,GS5_Ab5,A5,A5,AS5_Bb5,B5,0
};
const uint16_t times[]= // timing of the background music
{
	200,200,200,200,100,225,300,200,200,200,200,100,225,300,200,200,200,200,100,225,300,75,75,200, 75,75,200, 75,75,200, 75,75,300,5,
};

typedef struct // structure for the point dot
{
	uint8_t x; // x position of the point dot
	uint8_t y; // y position of the point dot
	uint8_t presence; // whether the point dot is visible (1) or not visible (0)
} Dot;
// different positions of the point dots
Dot dots[DOTS_NUM] = 
{
    {12,35,1},
	{12,50,1},
	{12,65,1},
	{12,95,1},
	{12,110,1},
    {12,125,1},
    {40,50,1},
    {85,50,1},
	{12,148,1},
	{114,35,1},
	{114,50,1},
	{114,65,1},
    {114,95,1},
    {114,110,1},
    {114,125,1},
    {114,148,1},
	{114,12,1},
	{97,12,1},
    {80,12,1},
	{97,148,1},
    {79,148,1},
    {46,148,1},
    {29,148,1},
    {40,110,1},
	{62,110,1},
	{85,110,1},

    
};
uint16_t score = 0; 		// setting the score to zero initially
uint8_t player_hit = 0;		// setting player hit to false initially
uint8_t pointeaten = 0;
uint8_t music_on = 1;
uint8_t music_off = 0;
uint8_t choice = 0;
int main() 
{
	uint8_t hinverted = 0;
	uint8_t vinverted = 0;
	uint8_t toggle = 0;
	uint8_t hmoved = 0;
	uint8_t vmoved = 0;
	uint8_t on = 0;
	uint8_t game_on = 1;
	// uint16_t score = 0;
	uint8_t new_x, new_y;
	uint16_t player_1_score;
	uint16_t player_2_score;
	uint8_t playerswitch = 0;
	uint8_t r;
	
	background_tune_notes = notes;
	background_tune_times = times;
	background_tune_notes_count = sizeof(notes)/4;
	background_repeat_tune = 50;

	initClock();
    initSysTick();
    setupIO();
	initSerial();
	initSound();
	srand(time(NULL));  // seed the random number generator

	

	while(on == 0)
    {
		// main menu

		r = 1+rand() % 5; 	// random number
	
		printTextX2("Pacman", 30 ,5, RGBToWord(0xff,0xea,0), 0);
        putImage(58,35,PLAYER_WIDTH,PLAYER_HEIGHT,pacman_open,0,0);
		putImage(78,40,PLAYER_WIDTH,PLAYER_HEIGHT,red_ghost_right,0,0);
		putImage(98,40,PLAYER_WIDTH,PLAYER_HEIGHT,blue_ghost_right,0,0);
		putImage(38,40,PLAYER_WIDTH,PLAYER_HEIGHT,green_ghost_right,0,0);
		putImage(18,40,PLAYER_WIDTH,PLAYER_HEIGHT,orange_ghost_right,0,0);
		
		putImage(85,100,PLAYER_WIDTH,PLAYER_HEIGHT,arrow_right,0,0);	// arrow right
		putImage(105,100,PLAYER_WIDTH,PLAYER_HEIGHT,mute,0,0);			// mute sign
		putImage(30,100,PLAYER_WIDTH,PLAYER_HEIGHT,arrow_right,1,0);	// arrow left
		putImage(10,100,PLAYER_WIDTH,PLAYER_HEIGHT,ms_pacman_open,0,0);	// ms pacman

		putImage(58,70,PLAYER_WIDTH,PLAYER_HEIGHT,arrow_down,0,1); 		// arrow up
		printText("start", 47, 60, RGBToWord(0xff,0xff,0xff), 0);
		putImage(58,130,PLAYER_WIDTH,PLAYER_HEIGHT,arrow_down,0,0);		// arrow down
		printText("credits", 42, 145, RGBToWord(0xff,0xff,0xff), 0);
		// printText("Press UP to start", 7, 80, RGBToWord(0xff,0xff,0xff), 0);
		// printText("Press RIGHT to", 15, 105, RGBToWord(0xff,0xff,0xff), 0);
		// printText("mute", 50, 115, RGBToWord(0xff,0xff,0xff), 0);
       
        // printText("Press DOWN for", 15, 140, RGBToWord(0xff,0xff,0xff), 0);
		// printText("credits", 40, 150, RGBToWord(0xff,0xff,0xff), 0);

        if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
        {
			// credits
			rollCredits();
			game_on = 0;
			break;
        }
        if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
        {
			// game starts
			renderMap(); 	// render map
            on = 1;
			delay(100); 	// to let go of button
            break;
        }
		if ((GPIOB->IDR & (1 << 4)) == 0) // right button pressed
		{
			// turns off music
			music_on = 0;
		}
		if ((GPIOB->IDR & (1 << 5)) == 0) // Left button pressed
		{
			// changes player Model to Ms. Pac-Man
			memcpy(pacman_open, ms_pacman_open, sizeof(pacman_open)); 	// function that overwrites one array with the data from another
			memcpy(pacman_closed, ms_pacman_closed, sizeof(pacman_closed));
			memcpy(pacman_open_down, ms_pacman_open_down, sizeof(pacman_open_down));
		}
    }

	switch (r)
	{
		case 1: // bottom right maze
			player_x = 110;
			player_y = 90;
			break;
		case 2: // origninal spawnpoint
			player_x = 58;
			player_y = 45;
			break;
		case 3: // top right maze
			player_x = 109;
			player_y = 62;
			break;
		case 4: // bottom middle
			player_x = 58;
			player_y = 105;
			break;
		case 5: // bottom left maze
			player_x = 8;
			player_y = 90;
			break;
		}

	while (game_on == 1)
	{

		while (score != TARGET_SCORE && player_hit == 0) // while target score is not met and player is not hit
		{
			hmoved = vmoved = 0;		// to check if player moved horizonally or vertically
			hinverted = vinverted = 0;	// record if player should be inverted horizontally or vertically
			new_x = player_x;			// to check new x and y position of player
			new_y = player_y;
			redOn();


			if ((GPIOB->IDR & (1 << 4)) == 0) // right button pressed
			{
				new_x += 1;		
				hmoved = 1;
				hinverted = 0;
			}
			if ((GPIOB->IDR & (1 << 5)) == 0) // Left button pressed
			{
				new_x -= 1;
				hmoved = 1;
				hinverted = 1;
			}
			if ((GPIOA->IDR & (1 << 11)) == 0) // down button pressed
			{
				new_y += 1;
				vmoved = 1;
				vinverted = 0;
			}
			if ((GPIOA->IDR & (1 << 8)) == 0) // up buttom pressed
			{
				new_y -= 1;
				vmoved = 1;
				vinverted = 1;
			}
			
			moveRedGhost(); 	// moving the red ghost
			moveBlueGhost(); 	// moving the blue ghost
			moveOrangeGhost(); 	// moving the orange ghost
			moveGreenGhost();	// moving the green ghost

			movePlayer(new_x, new_y, hmoved, hinverted, vmoved, vinverted, toggle, player_hit); // calling move player to allow the player to move if there is no collision
			
			score = checkDotCollection(player_x, player_y, playerswitch); 	// checks if the player has collected a dot
			printText("Player:1",2, 20, RGBToWord(0xff, 0x00, 0x00), 0); 	// displays the current player

			delay(50); 
		}
		r = 1+rand() % 5;
		delay(200);
		player_1_score = score;
		score = 0;
		player_hit = 0;
		switch (r)
		{
		case 1: // bottom right maze
			player_x = 110;
			player_y = 90;
			break;
		case 2: // origninal spawnpoint
			player_x = 58;
			player_y = 45;
			break;
		case 3: // top right maze
			player_x = 109;
			player_y = 62;
			break;
		case 4: // bottom middle
			player_x = 58;
			player_y = 105;
			break;
		case 5: // bottom left maze
			player_x = 8;
			player_y = 90;
			break;
		}

		

		fillRectangle(0,0, 130, 160, 0); // clears the screen
		printText("player 2's turn", 10, 75, RGBToWord(0xff, 0xff, 0), 0);
		playerswitch = 1;
		checkDotCollection(player_x, player_y, playerswitch);
		playerswitch = 0;
		delay(2000);
		resetDot();
		renderMap();
		
		putImage(player_x,player_y,PLAYER_WIDTH,PLAYER_HEIGHT,pacman_open,0,0);

		

		while (score != TARGET_SCORE && player_hit == 0) // while target score is not met and player is not hit
		{
			hmoved = vmoved = 0;		//to check if player moved horizonally or vertically
			hinverted = vinverted = 0;	//record if player should be inverted horizontally or vertically
			new_x = player_x;			//to check new x and y position of player
			new_y = player_y;
			redOff();
			greenOn();

			if ((GPIOB->IDR & (1 << 4)) == 0) // right button pressed
			{
				new_x += 1;		
				hmoved = 1;
				hinverted = 0;
			}
			if ((GPIOB->IDR & (1 << 5)) == 0) // Left button pressed
			{
				new_x -= 1;
				hmoved = 1;
				hinverted = 1;
			}
			if ((GPIOA->IDR & (1 << 11)) == 0) // down button pressed
			{
				new_y += 1;
				vmoved = 1;
				vinverted = 0;
			}
			if ((GPIOA->IDR & (1 << 8)) == 0) // up buttom pressed
			{
				new_y -= 1;
				vmoved = 1;
				vinverted = 1;
			}
			
			moveRedGhost(); //moving the red ghost
			moveBlueGhost(); //moving the blue ghost
			moveOrangeGhost(); //moving the orange ghost
			moveGreenGhost();
			movePlayer(new_x, new_y, hmoved, hinverted, vmoved, vinverted, toggle, player_hit); // calling move player to allow the player to move if there is no collision
			
			score = checkDotCollection(player_x, player_y, playerswitch); //checks if the player has collected a dot
			printText("Player:2",2, 20, RGBToWord(0x00, 0x0ff, 0x00), 0);

			delay(50); 
		}
		player_2_score = score;
		greenOff();
		fillRectangle(0,0, 130, 160, 0);

		// winner screen
		char player_1_score_text[10];
		char player_2_score_text[10];
		itoa(player_1_score, player_1_score_text, 10);
		itoa(player_2_score, player_2_score_text, 10);
		printTextX2("scores: ", 25, 5, RGBToWord(0xff, 0xff, 0xff), 0); // display "score"
		printText("Player 1 score: ", 2, 35, RGBToWord(0xff, 0x00, 0x00), 0);
		printText(player_1_score_text, 107, 35, RGBToWord(0xff, 0x00, 0x00), 0);
		printText("Player 2 score: ", 2, 55, RGBToWord(0x00, 0xff, 0x00), 0); // display actual score beside that
		printText(player_2_score_text, 107, 55, RGBToWord(0x00, 0xff, 0x00), 0); // display actual score beside that
		if (player_1_score > player_2_score)
		{
			printTextX2("Winner",27, 80, RGBToWord(0xff, 0xff, 0), 0);
			printText("Player 1", 35, 110, RGBToWord(0xff, 0x00, 0x00), 0);
			drawRectangle(30,105,60,16,RGBToWord(0x00, 0x00, 0xff));
			eputs("  _______\n |       |\n(|WINNER:|)\n |  #1   |\n \\       /\n   `---'\n   _|_|_\n");
		}
		else if (player_1_score == player_2_score)
		{
			printTextX2("DRAW", 40, 90, RGBToWord(0xff, 0xff, 0), 0);
			eputs("\n\n██████╗░██████╗░░█████╗░░██╗░░░░░░░██╗\n██╔══██╗██╔══██╗██╔══██╗░██║░░██╗░░██║\n██║░░██║██████╔╝███████║░╚██╗████╗██╔╝\n██║░░██║██╔══██╗██╔══██║░░████╔═████║░\n██████╔╝██║░░██║██║░░██║░░╚██╔╝░╚██╔╝░\n╚═════╝░╚═╝░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░╚═╝░░\n\n\n");
		}
		else
		{
			printTextX2("Winner", 27, 80, RGBToWord(0xff, 0xff, 0), 0);
			printText("Player 2", 35, 110, RGBToWord(0x00, 0xff, 0x00), 0);
			drawRectangle(30,105,62,16,RGBToWord(0x00, 0x00, 0xff));
			eputs("  _______\n |       |\n(|WINNER:|)\n |  #2   |\n \\       /\n   `---'\n   _|_|_\n");
		}
		printText("Return to menu", 15, 140, RGBToWord(0xff,0xff,0xff), 0);
		printText("press RESET", 30, 150, RGBToWord(0xff,0xff,0xff), 0);
		game_on = 0;
	}
    return 0;
}

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
	static int index = 0;
	static int current_timer = 0;

	if(music_on != 0) // music works as normal
	{
		choice = 1;
	}
	if(music_on == 0) // music doesn´t play
	{
		choice = 2;
	}

	switch (choice) 
	{
		case 1:
		if (background_tune_notes != 0)
		{
			// eputs("case 1\n");
			// if there is a tune...
			if (current_timer == 0)
			{
				// .. move on to the next note (if there is one)
				index++;
				if (index >= background_tune_notes_count)
				{
					// no more notes.
					if (background_repeat_tune == 0)
					{
						// don't repeat
						background_tune_notes = 0;
					}
					else
					{
						// reset to first note and repeat...
						index = 0;
					}
				}
				else
				{
					current_timer = background_tune_times[index];
					playNote(background_tune_notes[index]);
				}

			}
			if (current_timer != 0)
			{
				current_timer -- ;
			}
			break;
		}
		case 2: // no background music
		{
			// eputs("case 2 no sound\n");
			playNote(0);
			break;
		}
		default:
		{
			eputs("default\n");
			playNote(A4);
		}
		break;
	}
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 
        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);
        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py) {
    // checks to see if point px,py is overlapping with the rectangle defined by x,y,w,h
    return (px < x1 + w && px + PLAYER_WIDTH > x1 && py < y1 + h && py + PLAYER_HEIGHT > y1);
}
void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0); // button right
	pinMode(GPIOB,5,0); // button left
	pinMode(GPIOA,8,0); // button up
	pinMode(GPIOA,11,0);// button down
	pinMode(GPIOA,12,1);// blue LED
	pinMode(GPIOB,1,1); // speaker
	// pinMode(GPIOA,10,1);
	pinMode(GPIOA,9,1); // red LED
	// pinMode(GPIOA,2,3); // will use PA2 as an analog input to seed the random number generator
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,8);
	enablePullUp(GPIOA,11);
}
uint8_t isCollidingWithWall1(uint16_t x, uint16_t y)
{ 
	return (x < WALL1_X + WALL1_WIDTH &&  x + PLAYER_WIDTH > WALL1_X && y < WALL1_Y + WALL1_HEIGHT && y + PLAYER_HEIGHT > WALL1_Y); // returns true if there is overlap with wall 1
}
uint8_t isCollidingWithWall2(uint16_t x, uint16_t y)
{
	return (x < WALL2_X + WALL2_WIDTH &&  x + PLAYER_WIDTH > WALL2_X && y < WALL2_Y + WALL2_HEIGHT && y + PLAYER_HEIGHT > WALL2_Y); // returns true if there is overlap with wall 2
}
uint8_t isCollidingWithWall3(uint16_t x, uint16_t y)
{
	return (x < WALL3_X + WALL3_WIDTH &&  x + PLAYER_WIDTH > WALL3_X && y < WALL3_Y + WALL3_HEIGHT && y + PLAYER_HEIGHT > WALL3_Y); // returns true if there is overlap with wall 3
}
uint8_t isCollidingWithWall4(uint16_t x, uint16_t y)
{
	return (x < WALL4_X + WALL4_WIDTH &&  x + PLAYER_WIDTH > WALL4_X && y < WALL4_Y + WALL4_HEIGHT && y + PLAYER_HEIGHT > WALL4_Y); // returns true if there is overlap with wall 4
}
uint8_t isCollidingWithWall5(uint16_t x, uint16_t y)
{
	return (x < WALL5_X + WALL5_WIDTH &&  x + PLAYER_WIDTH > WALL5_X && y < WALL5_Y + WALL5_HEIGHT && y + PLAYER_HEIGHT > WALL5_Y); // returns true if there is overlap with wall 5
}
uint8_t isCollidingWithWall6(uint16_t x, uint16_t y)
{
	return (x < WALL6_X + WALL6_WIDTH &&  x + PLAYER_WIDTH > WALL6_X && y < WALL6_Y + WALL6_HEIGHT && y + PLAYER_HEIGHT > WALL6_Y); // returns true if there is overlap with wall 6
}
uint8_t isCollidingWithWall7(uint16_t x, uint16_t y)
{
	return (x < WALL7_X + WALL7_WIDTH &&  x + PLAYER_WIDTH > WALL7_X && y < WALL7_Y + WALL7_HEIGHT && y + PLAYER_HEIGHT > WALL7_Y); // returns true if there is overlap with wall 6
}
uint8_t isCollidingWithWall8(uint16_t x, uint16_t y)
{
	return (x < WALL8_X + WALL8_WIDTH &&  x + PLAYER_WIDTH > WALL8_X && y < WALL8_Y + WALL8_HEIGHT && y + PLAYER_HEIGHT > WALL8_Y); // returns true if there is overlap with wall 6
}
uint8_t isCollidingWithWall9(uint16_t x, uint16_t y)
{
	return (x < WALL9_X + WALL9_WIDTH &&  x + PLAYER_WIDTH > WALL9_X && y < WALL9_Y + WALL9_HEIGHT && y + PLAYER_HEIGHT > WALL9_Y); // returns true if there is overlap with wall 6
}
uint8_t isCollidingWithWall10(uint16_t x, uint16_t y)
{
	return (x < WALL10_X + WALL10_WIDTH &&  x + PLAYER_WIDTH > WALL10_X && y < WALL10_Y + WALL10_HEIGHT && y + PLAYER_HEIGHT > WALL10_Y); // returns true if there is overlap with wall 6
}
uint8_t isCollidingWithRedGhost(uint16_t player_x, uint16_t player_y, uint16_t ghost_x, uint16_t ghost_y) 
{
    return player_x < ghost_x + GHOST_WIDTH && player_x + PLAYER_WIDTH > ghost_x && player_y < ghost_y + GHOST_HEIGHT && player_y + PLAYER_HEIGHT > ghost_y; // returns true if there is overlap with red ghost
}
uint8_t isCollidingWithBlueGhost(uint16_t player_x, uint16_t player_y, uint16_t ghost_x, uint16_t ghost_y) 
{
    return player_x < ghost_x + GHOST_WIDTH && player_x + PLAYER_WIDTH > ghost_x && player_y < ghost_y + GHOST_HEIGHT && player_y + PLAYER_HEIGHT > ghost_y; // returns true if there is overlap with blue ghost
}
uint8_t isCollidingWithOrangeGhost(uint16_t player_x, uint16_t player_y, uint16_t ghost_x, uint16_t ghost_y) 
{
    return player_x < ghost_x + GHOST_WIDTH && player_x + PLAYER_WIDTH > ghost_x && player_y < ghost_y + GHOST_HEIGHT && player_y + PLAYER_HEIGHT > ghost_y; // returns true if there is overlap with orange ghost
}
uint8_t isCollidingWithGreenGhost(uint16_t player_x, uint16_t player_y, uint16_t ghost_x, uint16_t ghost_y) 
{
    return player_x < ghost_x + GHOST_WIDTH && player_x + PLAYER_WIDTH > ghost_x && player_y < ghost_y + GHOST_HEIGHT && player_y + PLAYER_HEIGHT > ghost_y; // returns true if there is overlap with green ghost
}
void movePlayer(uint16_t new_x, uint16_t new_y, uint16_t hmoved, uint16_t hinverted, uint16_t vmoved, uint16_t vinverted, uint16_t toggle, uint8_t player_hit) 
{
	if(player_hit==0) // checing if player was hit
	{
	// Clears the previous position of the player
		fillRectangle(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, 0);

		// Prevent diagonal movement: check if both horizontal and vertical movement are attempted
		if ((hmoved && vmoved)) 
		{
			putImage(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, pacman_open, 0, 0);
			// If both horizontal and vertical movement are true, do nothing (diagonal is not allowed)
			return; 
		}
		// Check if the player is colliding with anything
		if (!isCollidingWithWall1(new_x, new_y) && !isCollidingWithWall2(new_x, new_y) && !isCollidingWithWall3(new_x, new_y) && !isCollidingWithWall4(new_x, new_y) && !isCollidingWithWall5(new_x, new_y) && !isCollidingWithWall6(new_x, new_y) && !isCollidingWithWall7(new_x, new_y) && !isCollidingWithWall8(new_x, new_y) && !isCollidingWithWall9(new_x, new_y) && !isCollidingWithWall10(new_x, new_y)) 
		{ 
			// If player position doesn't collide with anything, update the position
			player_x = new_x;
			player_y = new_y;

			// If the player is idle (no movement), use open mouth
			if (!hmoved && !vmoved) {
				// Player is idle, mouth should be open
				putImage(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, pacman_open, hinverted, vinverted);
			} 
			else {
				// Player is moving, alternate between open and closed mouth
				if (toggle) 
				{
					// Open mouth
					putImage(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, pacman_open, hinverted, vinverted);
				} else 
				{
					// Closed mouth
					putImage(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, pacman_closed, hinverted, vinverted);
				}
				toggle = toggle ^ 1;  // Toggle between open and closed

				delay(5);  // PACMAN SPEED
			}
		}
		if (vmoved || hmoved) //redraw player if there is horizontal or vertical mpvement only
		{
			if (hmoved) // draw player based on horizontal movement direction
			{
				if (toggle)
				{
					putImage(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, pacman_open, hinverted, 0);
				}
				else
				{
					putImage(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, pacman_closed, hinverted, 0);
				}
				toggle = toggle ^ 1;
			}
			else 
			{
				putImage(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, pacman_open_down, 0, vinverted); //drawing player moving vertically
			}
		}
	}
}
void moveRedGhost() 
{
    static uint8_t toggle = 0;  // toggles different frams (0 is frame 1, 1 is frame 2)
    fillRectangle(red_ghost_x, red_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, 0);

    if (red_ghost_direction == 1) {  // if direction of movement is right
        if (red_ghost_x + GHOST_WIDTH < SCREEN_WIDTH - 1 && !isCollidingWithWall6(red_ghost_x, red_ghost_y)) // if not hitting right screen edge or wall
		{
            red_ghost_x += 1;  // Move right
        } 
		else // if hitting right screen edge or wall
		{
            red_ghost_direction = 0;  // switch direction
			//hinverted = 1;
        }
    } 
	else 
	{  // Moving left
        if (red_ghost_x > 0 && !isCollidingWithWall6(red_ghost_x - 1, red_ghost_y)) // if not hitting left screen edge or wall
		{
            red_ghost_x -= 1;  // Move left 
        }
        else // if hitting left screen edge or wall
		{
           red_ghost_direction = 1;  // switch directions
			//hinverted = 0;
        }
    }

    // to animate ghost
    if (toggle == 0)
	{
        putImage(red_ghost_x, red_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, red_ghost_right, !red_ghost_direction, 0); //draw frame 1
    } 
	else 
	{
        putImage(red_ghost_x, red_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, red_ghost_right_animation, !red_ghost_direction, 0); //draw frame 2 (! to invert number)
    }

    toggle = !toggle;  // toggle goes on and off to switch between frames

	if (isCollidingWithRedGhost(player_x, player_y, red_ghost_x, red_ghost_y))
	{
        player_hit = 1; // tell system player collided with ghost
		//death();
		death_animation(player_x,player_y);
    }
}
void moveBlueGhost() 
{
    static uint8_t toggle = 0;  // toggles different frams (0 is frame 1, 1 is frame 2)
    fillRectangle(blue_ghost_x, blue_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, 0);

    if (blue_ghost_direction == 1) {  // if direction of movement is right
        if (blue_ghost_x + GHOST_WIDTH < SCREEN_WIDTH - 1 && !isCollidingWithWall4(blue_ghost_x, blue_ghost_y)) // if not hitting right screen edge or wall
		{
            blue_ghost_x += 1;  // Move right
        } 
		else // if hitting right screen edge or wall
		{
            blue_ghost_direction = 0;  // switch direction
			//hinverted = 1;
        }
    } 
	else 
	{  // Moving left
        if (!isCollidingWithWall4(blue_ghost_x-1, blue_ghost_y)) // if not hitting left screen edge or wall
		{
            blue_ghost_x -= 1;  // Move left 
        }
        else // if hitting left screen edge or wall
		{
           blue_ghost_direction = 1;  // switch directions
			//hinverted = 0;
        }
    }
    // to animate ghost
    if (toggle == 0)
	{
        putImage(blue_ghost_x, blue_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, blue_ghost_right, !blue_ghost_direction, 0); //draw frame 1
    } 
	else 
	{
        putImage(blue_ghost_x, blue_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, blue_ghost_right_animation, !blue_ghost_direction, 0); //draw frame 2 (! to invert number)
    }

    toggle = !toggle;  // toggle goes on and off to switch between frames

	if (isCollidingWithBlueGhost(player_x, player_y, blue_ghost_x, blue_ghost_y))
	{
        player_hit = 1; // tell system player collided with ghost
		//death();
		death_animation(player_x,player_y);
	}
}
void moveOrangeGhost() // left bottom corner
{
    static uint8_t toggle = 0;  // toggles different frams (0 is frame 1, 1 is frame 2)
    fillRectangle(orange_ghost_x, orange_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, 0);

    if (orange_ghost_direction == 1) {  // if direction of movement is right
        if (!isCollidingWithWall4(orange_ghost_x+1, orange_ghost_y)) // if not hitting wall
		{
            orange_ghost_x += 1;  // Move right
        } 
		else // if hitting right screen edge or wall
		{
            orange_ghost_direction = 0;  // switch direction
			//hinverted = 1;
        }
    } 
	else 
	{  // Moving left
        if (orange_ghost_x > 1) // if not hitting left screen edge or wall
		{
            orange_ghost_x -= 1;  // Move left 
        }
        else // if hitting left screen edge or wall
		{
           orange_ghost_direction = 1;  // switch directions
			//hinverted = 0;
        }
    }
    // to animate ghost
    if (toggle == 0)
	{
        putImage(orange_ghost_x, orange_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, orange_ghost_right, !orange_ghost_direction, 0); //draw frame 1
    } 
	else 
	{
        putImage(orange_ghost_x, orange_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, orange_ghost_right_animation, !orange_ghost_direction, 0); //draw frame 2 (! to invert number)
    }

    toggle = !toggle;  // toggle goes on and off to switch between frames

	if (isCollidingWithOrangeGhost(player_x, player_y, orange_ghost_x, orange_ghost_y))
	{
        player_hit = 1; // tell system player collided with ghost
		// death();
		death_animation(player_x,player_y);
	}
}
void moveGreenGhost() // middle
{
    static uint8_t toggle = 0;  // toggles different frams (0 is frame 1, 1 is frame 2)
    fillRectangle(green_ghost_x, green_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, 0);

    if (green_ghost_direction == 1) {  // if direction of movement is right
        if (!isCollidingWithWall3(green_ghost_x+1, green_ghost_y)) // if not hitting wall
		{
            green_ghost_x += 1;  // Move right
        } 
		else // if hitting right screen edge or wall
		{
            green_ghost_direction = 0;  // switch direction
			//hinverted = 1;
        }
    } 
	else 
	{  // Moving left
        if (!isCollidingWithWall1(green_ghost_x-1, green_ghost_y)) // if not hitting left screen edge or wall
		{
            green_ghost_x -= 1;  // Move left 
        }
        else // if hitting left screen edge or wall
		{
           green_ghost_direction = 1;  // switch directions
			//hinverted = 0;
        }
    }
    // to animate ghost
    if (toggle == 0)
	{
        putImage(green_ghost_x, green_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, green_ghost_right, !green_ghost_direction, 0); //draw frame 1
    } 
	else 
	{
        putImage(green_ghost_x, green_ghost_y, GHOST_WIDTH, GHOST_HEIGHT, green_ghost_right_animation, !green_ghost_direction, 0); //draw frame 2 (! to invert number)
    }

    toggle = !toggle;  // toggle goes on and off to switch between frames

	if (isCollidingWithGreenGhost(player_x, player_y, green_ghost_x, green_ghost_y))
	{
        player_hit = 1; // tell system player collided with ghost
		// death();
		death_animation(player_x,player_y);
	}
}
void makeWall1()
{
	fillRectangle(WALL1_X, WALL1_Y, WALL1_WIDTH, WALL1_HEIGHT ,RGBToWord(0x05, 0x02, 0xff));
}
void makeWall2(void)
{
	fillRectangle(WALL2_X,WALL2_Y, WALL2_WIDTH, WALL2_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void makeWall3(void)
{
	fillRectangle(WALL3_X,WALL3_Y, WALL3_WIDTH, WALL3_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void makeWall4(void)
{
	fillRectangle(WALL4_X,WALL4_Y, WALL4_WIDTH, WALL4_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void makeWall5(void)
{
	fillRectangle(WALL5_X,WALL5_Y, WALL5_WIDTH, WALL5_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void makeWall6(void)
{
	fillRectangle(WALL6_X,WALL6_Y, WALL6_WIDTH, WALL6_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void makeWall7(void)
{
	fillRectangle(WALL7_X,WALL7_Y, WALL7_WIDTH, WALL7_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void makeWall8(void)
{
	fillRectangle(WALL8_X,WALL8_Y, WALL8_WIDTH, WALL8_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void makeWall9(void)
{
	fillRectangle(WALL9_X,WALL9_Y, WALL9_WIDTH, WALL9_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void makeWall10(void)
{
	fillRectangle(WALL10_X,WALL10_Y, WALL10_WIDTH, WALL10_HEIGHT, RGBToWord(0x05, 0x02, 0xff));
}
void renderMap(void)
{
	fillRectangle(0,0, 130, 160, 0);
	makeWall1();
	makeWall2();
	makeWall3();
	makeWall4();
	makeWall5();
	makeWall6();
	makeWall7();
	makeWall8();
	makeWall9();
	makeWall10();
	makeDot();
}
void drawDot(uint16_t x, uint16_t y) // render dot with colour and make visible to the player
{
	fillRectangle(x, y, 3, 3, RGBToWord(0xff, 0xff, 0xff));
}
void makeDot() // to generate specific number of dots at different positions of the screen
{
	for(uint8_t i = 0; i < DOTS_NUM; i++)
	{
		if (dots[i].presence)
		{
			drawDot(dots[i].x, dots[i].y);
		}
	}
}
void clearDot( uint16_t x, uint16_t y) //for remvoving the dot after it has been collected
{
	fillRectangle(x, y, 3, 3, 0);
}
void resetDot(void)
{
	for(uint8_t i =0; i < DOTS_NUM; i++)
	{
		dots[i].presence = 1;
	}
}
int checkDotCollection(uint16_t player_x, uint16_t player_y, uint8_t playerswitch) 
{
	
    for (uint8_t i = 0; i < DOTS_NUM; i++) 
	{
        if (dots[i].presence && isInside(dots[i].x, dots[i].y, 5, 5, player_x, player_y)) {
            dots[i].presence = 0; // Mark the dot as collected
            score += 10; // Increment score
            clearDot(dots[i].x, dots[i].y); // Remove the dot from the display
			if(playerswitch == 0) // check if a switch is happening
			{
				putImage(player_x, player_y, PLAYER_WIDTH, PLAYER_HEIGHT, pacman_closed, 0, 0);
			}
			pointeaten = 1; // for sound effect
			waka();
			waka();
	
        }
    }
	
	if (playerswitch == 1)
	{
		score = 0;
	}
	
    displayscore(score); // Update the score display
	
	return score;
}
void displayscore(int score) //update the score to the player
{
	char scoreText[10];
	itoa(score, scoreText, 10);
	printText("score: ", 2, 5, RGBToWord(0xff, 0xff, 0), 0); // display "score"
	printText(scoreText, 50, 5, RGBToWord(0xff, 0xff, 0), 0); // display actual score beside that
}
void rollCredits(void)
{
	fillRectangle(0, 0, 130, 160, 0);
	printTextX2("Credits", 20, 5, RGBToWord(0xff,0xff,0xff), 0);
	printText("by", 55, 35, RGBToWord(0x47,0xed,0x87), 0);
	printText("Mohammed Mujahed", 5, 60, RGBToWord(0x47,0xed,0x87), 0);
	printText("- Mapdesign", 20, 70, RGBToWord(0x47,0xed,0x87), 0);
	printText("- Multiplayer", 20, 80, RGBToWord(0x47,0xed,0x87), 0);

	printText("Christian", 5, 95, RGBToWord(0x47,0xed,0x87), 0);
	printText("Tauchnitz", 5, 105, RGBToWord(0x47,0xed,0x87), 0);
	printText("- Graphics", 20, 115, RGBToWord(0x47,0xed,0x87), 0);
	printText("- Menu + Sound", 20, 125, RGBToWord(0x47,0xed,0x87), 0);
	printText("Return to menu", 15, 140, RGBToWord(0xff,0xff,0xff), 0);
	printText("press RESET", 30, 150, RGBToWord(0xff,0xff,0xff), 0);
	srand(time(NULL));  // seed the random number generator another time
}
void redOn(void)
{
	GPIOA->ODR|=(1<<9);
}
void redOff(void)
{
	GPIOA->ODR&=~(1<<9);
}
void greenOn(void)
{
	GPIOA->ODR|=(1<<12);
}
void greenOff(void)
{
	GPIOA->ODR&=~(1<<12);
}

uint32_t rnumber()
	{
    uint32_t random_number = rand()%100; // generate a random integer unnecessarily big
	return random_number;
	}
void waka()
{
	playNote(A4);
	delay(20);
	playNote(DS6_Eb6);
	delay(20);

/* playNote(E3); // original waka sound. took too long
delay(10);
playNote(GS3_Ab3);
delay(10);
playNote(DS4_Eb4);
delay(10);
playNote(FS4_Gb4);
delay(10);
playNote(AS4_Bb4);
delay(10);
playNote(0);
delay(30);
playNote(B4);
delay(10);
playNote(G4);
delay(10);
playNote(E4);
delay(10);
playNote(CS4_Db4);
delay(10);
playNote(A3);
delay(10);
playNote(F3);
delay(10);
playNote(0);
delay(30); */
}


void death_animation(uint8_t player_x, uint8_t player_y)
{
	playNote(E5);
	putImage(player_x,player_y,PLAYER_WIDTH,PLAYER_HEIGHT,pacman_death_1,0,0);
	eputs("\n\n\n888888888888888888888888888888888888888888888888888888888888\n888888888888888888888888888888888888888888888888888888888888\n8888888888888888P\"88888P          988888\"9888888888888888888");
	playNote(B4);
	putImage(player_x,player_y,PLAYER_WIDTH,PLAYER_HEIGHT,pacman_death_2,0,0);
	eputs("\n8888888888888888  \"9888            888P\"  888888888888888888\n888888888888888888bo \"9  d8o  o8b  P\" od88888888888888888888\n888888888888888888888bob 98\"  \"8P dod88888888888888888888888");
	playNote(E4);
	putImage(player_x,player_y,PLAYER_WIDTH,PLAYER_HEIGHT,pacman_death_3,0,0);
	eputs("\n888888888888888888888888    db    88888888888888888888888888\n88888888888888888888888888      8888888888888888888888888888\n88888888888888888888888P\"9bo  odP\"98888888888888888888888888\n88888888888888888888P\" od88888888bo \"98888888888888888888888");
	playNote(0);
	putImage(player_x,player_y,PLAYER_WIDTH,PLAYER_HEIGHT,pacman_death_4,0,0);
	eputs("\n888888888888888888   d88888888888888b   88888888888888888888\n8888888888888888888oo8888888888888888oo888888888888888888888\n888888888888888888888888888888888888888888888888888888888888\n\n\n");
	delay(100);
	
}
