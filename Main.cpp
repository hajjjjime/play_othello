#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

int cursorX, cursorY;

enum {
	COLOR_NONE  = -1,
	COLOR_BLACK = 0,
	COLOR_WHITE = 1,
	COLOR_MAX
};

enum {
	DIRECTION_UP,
	DIRECTION_UP_LEFT,
	DIRECTION_LEFT,
	DIRECTION_DOWN_LEFT,
	DIRECTION_DOWN,
	DIRECTION_DOWN_RIGHT,
	DIRECTION_RIGHT,
	DIRECTION_UP_RIGHT,
	DIRECTION_MAX
};

int directions[][2] = {
	{ 0, -1},  //DIRECTION_UP
	{-1, -1}, //DIRECTION_UP_LEFT,
	{-1,  0},  //DIRECTION_LEFT,
	{-1,  1},  //DIRECTION_DOWN_LEFT,
	{ 0,  1}, //DIRECTION_DOWN,
	{ 1,  1}, //DIRECTION_DOWN_RIGHT,
	{ 1,  0}, //DIRECTION_RIGHT,
	{ 1, -1}, //DIRECTION_UP_RIGHT,
};

char colorNames[][8 + 1] = {
	"Black(x)", //COLOR BLACK
	"White(o)", //COLOR WHITE
};

int cells [BOARD_HEIGHT][BOARD_WIDTH];
int turn;

bool checkCanPut(int _color, int _x, int _y, bool _turnOver) {
	if (cells[_y][_x] != COLOR_NONE)
		return false;
	for (int i = 0; i < DIRECTION_MAX; i++) {
		int x = _x, y = _y;
		x += directions[i][0];
		y += directions[i][1];
		if (cells[y][x] != (_color ^ 1))
			continue;
		while (1) {
			x += directions[i][0];
			y += directions[i][1];

			if ((x < 0) || (x >= BOARD_WIDTH) || (y < 0) || (y >= BOARD_HEIGHT))
				break;
			if (cells[y][x] == COLOR_NONE)
				break;
			if (cells[y][x] == _color) {
				if (!_turnOver)
					return true;

				int x2 = _x, y2 = _y;
				while (1) {
					cells[y2][x2] = _color;

					x2 += directions[i][0];
					y2 += directions[i][1];

					if ((x2 == x) && (y2 == y))
						break;
				}
			}
		}
	}
	return false;
}

bool checkCanPutAll(int _color) {
	for (int y = 0; y < BOARD_HEIGHT; y++)
		for (int x = 0; x < BOARD_WIDTH; x++)
			if (checkCanPut(_color, x, y, false))
				return true;
	return false;
}

void drawBoard() {
	system("cls");
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if ((x == cursorX) && (y == cursorY)) {
				printf("c ");
			}
			else {
				switch (cells[y][x]) {
				case COLOR_NONE: printf(". "); break;
				case COLOR_BLACK: printf("x "); break;
				case COLOR_WHITE: printf("o "); break;
				}
			}
		}
		printf("\n");
	}
}

bool if_outside(int cursor) {
	if ((cursor < 0) || (BOARD_WIDTH <= cursor)) return true;
	return false;
}

int main() {
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			cells[y][x] = COLOR_NONE;
		}
	}
	cells[3][3] = cells[4][4] = COLOR_WHITE;
	cells[3][4] = cells[4][3] = COLOR_BLACK;

	bool cantPut = false;
	while (1) {
		drawBoard();
		if (cantPut)
			printf("Can't put!\n");
		else
			printf("%s turn.\n", colorNames[turn]);

		cantPut = false;
		switch (_getch()) {
		case 'w': if(if_outside(--cursorY)) ++cursorY; break;
		case 's': if(if_outside(++cursorY)) --cursorY; break;
		case 'a': if(if_outside(--cursorX)) ++cursorX; break;
		case 'd': if(if_outside(++cursorX)) --cursorX; break;
		default:
			if (!checkCanPut(turn, cursorX, cursorY, false)) {
				cantPut = true;
				break;
			}

			checkCanPut(turn, cursorX, cursorY, true);
			
			cells[cursorY][cursorX] = turn;
			turn ^= 1;
			if (!checkCanPutAll(turn))
				turn ^= 1;
			break;
		}
		if (!checkCanPutAll(COLOR_BLACK) && !checkCanPutAll(COLOR_WHITE)) {
			int count[COLOR_MAX] = {};
			for (int y = 0; y < BOARD_HEIGHT; y++) {
				for (int x = 0; x < BOARD_WIDTH; x++)
					if (cells[y][x] != COLOR_NONE)
						count[cells[y][x]]++;
			}
			drawBoard();
			for (int i = 0; i < COLOR_MAX; i++)
				printf("%s:%d\n", colorNames[i], count[i]);
			if (count[COLOR_BLACK] == count[COLOR_WHITE])
				printf("Draw!\n");
			else {
				if (count[COLOR_BLACK] < count[COLOR_WHITE])
					printf("%s ", colorNames[COLOR_BLACK]);
				else
					printf("%s ", colorNames[COLOR_WHITE]);
				printf("Lose!\n");
			}
			_getch();
			break;
		}
	}
}