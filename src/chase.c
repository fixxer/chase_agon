#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "mos.h"

#define gotoxy(x, y) putchar(31);putchar(x);putchar(y)
#define clrscr() putchar(12)
#define setfgcolor(color) putchar(17);putchar(color)
#define cursor_off() putchar(23);putchar(1);putchar(0)
#define cursor_on() putchar(23);putchar(1);putchar(1)

#define BOARD_WIDTH 40
#define BOARD_HEIGHT 20
#define BOT_COUNT 10

char board[BOARD_HEIGHT][BOARD_WIDTH];
int posX, posY;
int robotX[BOT_COUNT];
int robotY[BOT_COUNT];
short seed;
unsigned int robotCount;
unsigned char color = 0;

static unsigned long int next = 1;

int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
    next = seed;
}

void drawBoard() {
	int i;
	int j;
	
	for (i=0;i<BOARD_HEIGHT;i++) {
		for(j=0;j<BOARD_WIDTH;j++) {
			gotoxy(j + 5, i + 1);
			switch(board[i][j]) {
				case 'X':
					setfgcolor(6);
					printf("X");
					break;
				case '*':
					setfgcolor(2);
					printf("*");
					break;
				case '+':
					setfgcolor(3);
					printf("+");
					break;
				case ' ':
					printf(" ");
					break;
			}
		}
	}
}

void setupBoard() {
	int i;
	int j;
	
	robotCount = BOT_COUNT;
	
	for (i=0;i<BOARD_HEIGHT;i++) {
		for (j=0;j<BOARD_WIDTH;j++) {
			if (rand() % 20 == 5) {
				board[i][j] = 'X';
			} else {
				board[i][j] = ' ';
			}
		}
	}
	
	for (i=0;i<BOARD_HEIGHT;i++) {
		board[i][0] = 'X';
		board[i][BOARD_WIDTH-1] = 'X';
	}
	for (i=0;i<BOARD_WIDTH;i++) {
		board[0][i] = 'X';
		board[BOARD_HEIGHT-1][i] = 'X';
	}
	
	// add player
	
	do {
		posX = rand() % (BOARD_WIDTH - 2) + 1;
		posY = rand() % (BOARD_HEIGHT - 2) + 1;
	} while (board[posY][posX] == 'X');
	
	board[posY][posX] = '*';
	
	// add robots
	
	for (i=0;i<BOT_COUNT;i++) {
		do {
			robotX[i] = rand() %  (BOARD_WIDTH - 2)  + 1;
			robotY[i] = rand() %  (BOARD_HEIGHT - 2)  + 1;
		} while (board[robotY[i]][robotX[i]] != ' ');
		
		board[robotY[i]][robotX[i]] = '+';
	}
}

int robotMove() {
	int i;
	int j;
	
	int newX;
	int newY;
	
	for (i=0;i<BOT_COUNT;i++) {
		if (robotX[i] == -1 && robotY[i] == -1) continue;
		if (robotX[i] - posX > 0) {
			newX = robotX[i]-1;
		} else if (robotX[i] - posX < 0) {
			newX = robotX[i]+ 1;
		} else {
			newX = robotX[i];
		}
		
		if (robotY[i] - posY > 0) {
			newY = robotY[i]-1;
		} else if (robotY[i] - posY < 0) {
			newY = robotY[i]+ 1;
		} else {
			newY = robotY[i];
		}
		
		if (board[newY][newX] == 'X') {
			robotCount--;
			gotoxy(robotX[i] + 5, robotY[i] + 1);
			printf(" ");
			board[robotY[i]][robotX[i]] = ' ';
			robotY[i] = -1;
			robotX[i] = -1;
		} else if (board[newY][newX] == '+') {
			// destroy both robots;
			for (j=0;j<BOT_COUNT;j++) {
				if (robotX[j] == newX && robotY[j] == newY) {
					robotX[j] = -1;
					robotY[j] = -1;
				}
			}
			gotoxy(robotX[i] + 5, robotY[i] + 1);
			printf(" ");
			board[robotY[i]][robotX[i]] = ' ';
			robotX[i] = -1;
			robotY[i] = -1;
			robotCount -= 2;
			board[newY][newX] = ' ';
			gotoxy(newX + 5, newY + 1);
			printf(" ");
		} else if (board[newY][newX] == '*') {
			// uh oh
			gotoxy(robotX[i] + 5, robotY[i] + 1);
			printf(" ");
			gotoxy(newX + 5, newY + 1);
			setfgcolor(3);			
			printf("+");
			gotoxy(0, BOARD_HEIGHT + 2);
			setfgcolor(1);
			printf("     Executed by a Robot! You're Dead. Press a key to Exit");
			return 1;
		} else {
			board[robotY[i]][robotX[i]] = ' ';
			board[newY][newX] = '+';
			gotoxy(robotX[i] + 5, robotY[i] + 1);
			printf(" ");
			gotoxy(newX + 5, newY + 1);
			setfgcolor(3);			
			printf("+");
			
			robotY[i] = newY;
			robotX[i] = newX;
		}
	}
	if (robotCount == 0) {
		gotoxy(0, BOARD_HEIGHT + 2);
		setfgcolor(2);
		printf("     You win! Press a key to Exit");
		setfgcolor(7);
		return 1;
	}
	return 0;
}

int playerMove() {
	char key;
	int newX;
	int newY;
	
	int done = 0;

	while (!done) {
		key = getchar();
		done = 1;
		switch (key) {
			case 'w':
				newX = posX - 1;
				newY = posY - 1;
				break;
			case 'e':
				newY = posY - 1;
				newX = posX;
				break;
			case 'r':
				newY = posY - 1;
				newX = posX + 1;
				break;
			case 's':
				newX = posX - 1;
				newY = posY;
				break;
			case 'f':
				newX = posX + 1;
				newY = posY;
				break;
			case 'x':
				newX = posX - 1;
				newY = posY + 1;
				break;
			case 'c':
				newY = posY + 1;
				newX = posX;
				break;
			case 'v':
				newY = posY + 1;
				newX = posX + 1;
				break;
			case '0':
				return 0;
			case 'j':
				newX = rand() % (BOARD_WIDTH -2) + 1;
				newY = rand() % (BOARD_HEIGHT -2) + 1;
				break;
			case 'q':
				done = 1;
				break;
			default:
				done = 0;
				break;
		}
	}
	if (board[newY][newX] == ' ') {
		// safe move
		board[newY][newX] = '*';
		board[posY][posX] = ' ';
		gotoxy(posX + 5, posY + 1);
		printf(" ");
		gotoxy(newX + 5, newY + 1);		
		setfgcolor(2);
		printf("*");
	
		posY = newY;
		posX = newX;
		return 0;
	}
	else {
		if (board[newY][newX] == 'X') {
			// electrocuted
			gotoxy(0, BOARD_HEIGHT + 2);
			setfgcolor(1);
			printf("     High Voltage! ZAPP!! You're Dead. Press a key to Exit");
			setfgcolor(7);
		} else {
			// robot got you!
			gotoxy(0, BOARD_HEIGHT + 2);
			setfgcolor(1);
			printf("     Executed by a Robot! You're Dead. Press a key to Exit");
			setfgcolor(7);
		}
		return 1;
	}
}

void main() {
	cursor_off();
  int done = 0;
	int quit = 0;
	seed = 9374;

	srand(seed);
	
	while (quit==0) {
	  clrscr();	
		setfgcolor(5);
		printf(" ___     _         _      ___ _                 \r\n");
		printf("| _ \\___| |__  ___| |_   / __| |_  __ _ ___ ___ \r\n");
		printf("|   / _ \\ '_ \\/ _ \\  _| | (__| ' \\/ _` (_-</ -_)\r\n");
		printf("|_|_\\___/_.__/\\___/\\__|  \\___|_||_\\__,_/__/\\___|\r\n\r\n");
		setfgcolor(2);
		printf("Based on the game \"CHASE\" appearing in the book\r\n");
		printf("    \"More BASIC Computer Games\" by David Ahl\r\n");

		setfgcolor(6);printf("P");setfgcolor(7);printf(" - Play\r\n");
		setfgcolor(6);printf("I");setfgcolor(7);printf(" - Print Instructions\r\n");
		setfgcolor(6);printf("S");setfgcolor(7);printf(" - Enter Random Seed (%u)\r\n", seed);
		setfgcolor(6);printf("Q");setfgcolor(7);printf(" - Quit\r\n");

		char c = getchar();
		switch(c) {
			case 'p':
				done = 0;
				clrscr();
				setupBoard();
				gotoxy(0, BOARD_HEIGHT + 4);
				setfgcolor(5);
				printf("     R O B O T\r\n");
				printf("     C H A S E\r\n");                                              
				printf("     \r\n");
				setfgcolor(7);
				printf("     W   E   R  J - Jump\r\n");     
				printf("       \\ | /    0 - Pass\r\n");      
				printf("     S - * - F\r\n");
				printf("       / | \\\r\n");
				printf("     X   C   V  Q - Quit\r\n");
				printf("\r\n");
				setfgcolor(2);printf("     *");setfgcolor(7);printf(" - You\r\n");    
				setfgcolor(6);printf("     X");setfgcolor(7);printf(" - High Voltage Line\r\n");
				setfgcolor(3);printf("     +");setfgcolor(7);printf(" - Killer Robot\r\n");
				drawBoard();
				while (!done) {
					gotoxy(79, 23);
					if (playerMove()) break;
					done = robotMove();
				}
				getchar();
				break;
			case 'i':
				clrscr();

				setfgcolor(11);
				printf("You are trapped in a maze of high voltage power\r\n");
				printf("fences and posts. There are also killer robots\r\n");
				printf("trying to destroy you.\r\n\r\n");
																
				printf("Your only hope is to lure the robots into a fence\r\n");
				printf("or each other to destroy them.\r\n\r\n");
																
				printf("If things get hopeless, you can do a mega jump,\r\n");
				printf("but you could end up anywhere, including on top\r\n");
				printf("of a power fence or a robot!\r\n");

				setfgcolor(7);
				printf("\r\nPress a key to continue!\r\n");
				getchar();
				break;
			case 's':
				cursor_on();
				printf("\r\nPlease enter a seed: ");
				char * buf = "0000";
				mos_editline(buf, 5, 0);
				seed = atoi(buf);
				srand(seed);
				cursor_off();
				break;
			case 'q':
				quit = 1;
				cursor_on();
				setfgcolor(15);
				break;
		}
	}
	clrscr();
	printf("Thanks for playing Robot Chase!\r\n");
}