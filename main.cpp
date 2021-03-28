#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

SceneID scene, scene2;
ObjectID gameBoard[9], initBoard[9];
ObjectID startBtn;
TimerID timer;

clock_t timeStart, timeEnd;

int blank = 8;
bool game = false;
int mixCount = 100;
float timerSpeed = 0.05f;
char sec[20];

int indexToX(int i) {
	return 180 + 192 * (i % 3); 
}

int indexToY(int i) {
	return 350 - 154 * (i / 3);
}

int gameIndex(ObjectID object) {
	for (int i = 0; i < 9; i++) {
		if (gameBoard[i] == object) return i;
	}
	return -1;
}

void gameMove(int i) {
	ObjectID object = gameBoard[i];
	gameBoard[i] = gameBoard[blank];
	locateObject(gameBoard[i], scene2, indexToX(i), indexToY(i));
	gameBoard[blank] = object;
	locateObject(gameBoard[blank], scene2, indexToX(blank), indexToY(blank));

	blank = i;
}

void gamePrint() {
	for (int i = 0; i < 9; i++) {
		printf("%d ", gameBoard[i]);
		if ((i + 1) % 3 == 0) printf("\n");
	}
	printf("\n");
}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);

	if (shown) {
		showObject(object);
	}

	return object;
}


void gameInit() {
	srand(static_cast<unsigned int>(time(NULL)));

	scene = createScene("picture", "Images/gyuri.png");
	scene2 = createScene("puzzle", "Images/black.jpg");

	gameBoard[0] = createObject("Images/1.png", scene2, indexToX(0), indexToY(0), true);
	initBoard[0] = gameBoard[0];
	gameBoard[1] = createObject("Images/2.png", scene2, indexToX(1), indexToY(1), true);
	initBoard[1] = gameBoard[1];
	gameBoard[2] = createObject("Images/3.png", scene2, indexToX(2), indexToY(2), true);
	initBoard[2] = gameBoard[2];
	gameBoard[3] = createObject("Images/4.png", scene2, indexToX(3), indexToY(3), true);
	initBoard[3] = gameBoard[3];
	gameBoard[4] = createObject("Images/5.png", scene2, indexToX(4), indexToY(4), true);
	initBoard[4] = gameBoard[4];
	gameBoard[5] = createObject("Images/6.png", scene2, indexToX(5), indexToY(5), true);
	initBoard[5] = gameBoard[5];
	gameBoard[6] = createObject("Images/7.png", scene2, indexToX(6), indexToY(6), true);
	initBoard[6] = gameBoard[6];
	gameBoard[7] = createObject("Images/8.png", scene2, indexToX(7), indexToY(7), true);
	initBoard[7] = gameBoard[7];
	gameBoard[8] = createObject("Images/9.png", scene2, indexToX(8), indexToY(8), true);
	initBoard[8] = gameBoard[8];
	/*
	char buf[50];

	for (int i = 0; i < 9; i++) {
		sprintf(buf, "Images/%d.jpg", i + 1);
		initBoard[i] = createObject(buf, scene2, indexToX(i), indexToY(i), true);
		gameBoard[i] = initBoard[i];

	}
	* 왜인지는 모르겠으나 위의 코드로 해도 화면에 object들이 보이지 않았습니다.
	*/
	startBtn = createObject("Images/start.jpg", scene2, 400, 0, true);

	timer = createTimer(0.01f);

}

bool isPossible(int i) {
	if ((i % 3 > 0 && blank == i - 1) || 
		(i % 3 < 2 && blank == i + 1) || 
		(blank == i - 3) || 
		(blank == i + 3)) {
		return true;
	}
	return false;
}

int randomMix() {
	int i = -1;

	while (i == -1) {
		switch (rand() % 3) { // 0, 1, 2
			case 0: if (blank % 3 > 0) i = blank - 1;  //left
				break;
			case 1: if (blank % 3 < 2) i = blank + 1;  //right 
				break;
			case 2: if (blank / 3 > 0) i = blank - 3;  //down
				break;
			case 3: if (blank / 3 < 2) i = blank + 3;  //up
				break;
		}
	}

	return i;
}

bool isGameEnd() {
	for (int i = 0; i < 9; i++) {
		if (gameBoard[i] != initBoard[i]) return false;
	}
	return true;
}

void gameStart() {
	timeStart = clock();
	blank = 8;
	setObjectImage(gameBoard[blank], "Images/10.png");
	showObject(gameBoard[blank]);

	setTimer(timer, timerSpeed);
	startTimer(timer);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (game) {
		int i = gameIndex(object);
		if (isPossible(i)) {
			gameMove(i);

			if (isGameEnd()) {
				game = false;
				timeEnd = clock();
				setObjectImage(startBtn, "Images/reset.jpg");
				setObjectImage(gameBoard[blank], "Images/9.jpg");
				showObject(startBtn);
				sprintf(sec, "성공!! 시간 : %.0lf초", (double)(timeEnd - timeStart) / CLOCKS_PER_SEC);
				showMessage(sec);
			}
		}
		gamePrint();
	}
	else {
		if (object == startBtn) {
			gameStart();
			
		}
	}
}

void timerCallback(TimerID timer) {
	mixCount--;
	gameMove(randomMix());

	if (mixCount > 0) {
		setTimer(timer, timerSpeed);
		startTimer(timer);
	}
	else {
		game = true;
		hideObject(startBtn);
	}
	
}

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	gameInit();

	startGame(scene2);
}