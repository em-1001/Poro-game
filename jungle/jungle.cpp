#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include <bangtal.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define ANIMATION_STEP		5

clock_t starttime, endtime;

SceneID scene1;

ObjectID player, home, l_crab[3], r_crab[3], l_wolf, r_wolf[2], l_bird[2], r_bird, cookie[3], red, blue;
ObjectID startButton, endButton;

TimerID player_timer, game_timer;
TimerID l_crab_timer[3], r_crab_timer[3], l_wolf_timer, r_wolf_timer[2], l_bird_timer[2], r_bird_timer, red_timer, blue_timer;
Second start_time = 36000, end_time;

int player_x = 610, player_y = 20;
int player_dx = 0, player_dy = 0;
int l_crab_y = 110, l_crab_x[3] = { 1370, 1370, 1370 };
int r_crab_y = 417, r_crab_x[3] = { -100, -100, -100 };
int r_wolf_y = 200, r_wolf_x[2] = { -300, -300 };
int l_wolf_y = 315, l_wolf_x = 1370;
int l_bird_y = 315, l_bird_x[2] = { 1370, 1370 };
int r_bird_y = 200, r_bird_x = -300;
int red_y = 600, red_x = 770;
int blue_y = 600, blue_x = 470;
int cookie_x[3], cookie_y[3];


bool ate[3] = { false, false, false };
bool keyboard = true;


//판정
bool crab_checkin(int x, int y, int cx, int cy, int r)
{
	return (x > cx - r - 5 && x < cx + r + 5 && y > cy - r + 15 && y < cy + r + 40);
}
bool wolf_checkin(int x, int y, int cx, int cy, int r)
{
	return (x > cx - r + 10 && x < cx + r + 35 && y > cy - r + 30 && y < cy + r + 50);
}
bool bird_checkin(int x, int y, int cx, int cy, int r)
{
	return (x > cx - r + 10 && x < cx + r + 210 && y > cy - r + 40 && y < cy + r + 50);
}
bool cookie_checkin(int x, int y, int cx, int cy, int r)
{
	return (x > cx - r && x < cx + r && y > cy - r && y < cy + r);
}
bool bluered_checkin(int x, int y, int cx, int cy, int r)
{
	return (x > cx - r - 5 && x < cx + r + 5 && y > cy - r && y < cy + r + 20);
}

ObjectID makeObject(const char* image, SceneID scene, int x, int y, bool shown)
{
	ObjectID object = createObject(image);

	locateObject(object, scene, x, y);

	if (shown)
	{
		showObject(object);
	}
	return object;
}

void startGame()
{
	srand(time(NULL));

	hideObject(startButton);
	hideObject(endButton);
	showObject(red);
	showObject(blue);
	for (int i = 0; i < 3; i++)
	{
		cookie_x[i] = rand() % 900 + 200;
		cookie_y[i] = rand() % 250 + 150;
		cookie[i] = makeObject("food.png", scene1, cookie_x[i], cookie_y[i], true);
		scaleObject(cookie[i], 0.5f);
	}

	player = makeObject("poro.png", scene1, player_x, player_y, true);
	scaleObject(player, 0.4f);

	for (int i = 0; i < 3; i++)
	{

		l_crab[i] = makeObject("left_crab.png", scene1, l_crab_x[i], l_crab_y, true);
		scaleObject(l_crab[i], 0.6f);
		r_crab[i] = makeObject("right_crab.png", scene1, r_crab_x[i], r_crab_y, true);
		scaleObject(r_crab[i], 0.6f);

	}

	for (int j = 0; j < 2; j++)
	{
		r_wolf[j] = makeObject("right_wolf.png", scene1, r_wolf_x[j], r_wolf_y, true);
		scaleObject(r_wolf[j], 0.7f);

		l_bird[j] = makeObject("left_bird.png", scene1, l_bird_x[j], l_bird_y, true);
		scaleObject(l_bird[j], 0.7f);
	}
	l_wolf = makeObject("left_wolf.png", scene1, l_wolf_x, l_wolf_y, true);
	scaleObject(l_wolf, 0.7f);

	r_bird = makeObject("right_bird.png", scene1, r_bird_x, r_bird_y, true);
	scaleObject(r_bird, 0.7f);

	
	game_timer = createTimer(start_time);
	startTimer(game_timer);
	

	l_crab_timer[0] = createTimer(0.05f);
	startTimer(l_crab_timer[0]);
	l_crab_timer[1] = createTimer(5.05f);
	startTimer(l_crab_timer[1]);
	l_crab_timer[2] = createTimer(11.05f);
	startTimer(l_crab_timer[2]);

	r_crab_timer[0] = createTimer(0.05f);
	startTimer(r_crab_timer[0]);
	r_crab_timer[1] = createTimer(5.05f);
	startTimer(r_crab_timer[1]);
	r_crab_timer[2] = createTimer(11.05f);
	startTimer(r_crab_timer[2]);

	l_wolf_timer = createTimer(3.55f);
	startTimer(l_wolf_timer);

	r_wolf_timer[0] = createTimer(0.05f);
	startTimer(r_wolf_timer[0]);
	r_wolf_timer[1] = createTimer(7.05f);
	startTimer(r_wolf_timer[1]);

	l_bird_timer[0] = createTimer(0.05f);
	startTimer(l_bird_timer[0]);
	l_bird_timer[1] = createTimer(6.05f);
	startTimer(l_bird_timer[1]);

	r_bird_timer = createTimer(4.05f);
	startTimer(r_bird_timer);

	red_timer = createTimer(0.05);
	startTimer(red_timer);

	blue_timer = createTimer(0.05);
	startTimer(blue_timer);

	keyboard = true;

	player_timer = createTimer(0.05f);
	startTimer(player_timer);

}

void closeGame(bool success)
{
	keyboard = false;

	if (success && (ate[0] == true && ate[1] == true && ate[2] == true))
	{
		end_time = getTimer(game_timer);
		Second time = start_time - end_time;
		char temp[50];
		sprintf(temp, "성공!!!\n걸린시간 %.f초", time);

		showMessage(temp);
	}
	else if (success && !(ate[0] == true && ate[1] == true && ate[2] == true))
	{
		showMessage("실패!!!\n쿠키를 다 모으지 못했군요!");
	}
	else
	{
		showMessage("실패!!!\n몬스터에게 공격당했습니다!");
	}
	

	
	

	setObjectImage(startButton, "restart.png");
	showObject(startButton);
	showObject(endButton);
	hideObject(player);
	hideObject(red);
	hideObject(blue);
	for (int i = 0; i < 3; i++)
	{
		hideObject(cookie[i]);
	}

	player_x = 610, player_y = 20;
	player_dx = 0, player_dy = 0;
	l_crab_y = 110, l_crab_x[0] = 1370, l_crab_x[1] = 1370, l_crab_x[2] = 1370;
	r_crab_y = 417, r_crab_x[0] = -100, r_crab_x[1] = -100, r_crab_x[2] = -100;
	r_wolf_y = 200, r_wolf_x[0] = -300, r_wolf_x[1] = -300;
	l_wolf_y = 315, l_wolf_x = 1370;
	l_bird_y = 315, l_bird_x[0] = 1370, l_bird_x[1] = 1370;
	r_bird_y = 200, r_bird_x = -300;
	red_y = 600, red_x = 770;
	blue_y = 600, blue_x = 470;
	ate[0] = false, ate[1] = false, ate[2] = false;

	for (int k = 0; k < 3; k++)
	{
		stopTimer(l_crab_timer[k]);
		stopTimer(r_crab_timer[k]);

		locateObject(l_crab[k], scene1, l_crab_x[k], l_crab_y);
		locateObject(r_crab[k], scene1, r_crab_x[k], r_crab_y);

	}
	for (int t = 0; t < 2; t++)
	{
		stopTimer(r_wolf_timer[t]);
		stopTimer(l_bird_timer[t]);

		locateObject(r_wolf[t], scene1, r_wolf_x[t], r_wolf_y);
		locateObject(l_bird[t], scene1, l_bird_x[t], l_bird_y);
	}

	stopTimer(l_wolf_timer);
	stopTimer(r_bird_timer);
	stopTimer(red_timer);
	stopTimer(blue_timer);
	stopTimer(player_timer);
	

	locateObject(l_wolf, scene1, l_wolf_x, l_wolf_y);
	locateObject(r_bird, scene1, r_bird_x, r_bird_y);
	locateObject(red, scene1, red_x, red_y);
	locateObject(blue, scene1, blue_x, blue_y);
	locateObject(player, scene1, player_x, player_y);

	
}




void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (object == endButton)
	{
		endGame();
	}
	else if (object == startButton)
	{
		
		startGame();

	}
}

void keyboardCallback(KeyCode code, KeyState state)
{
	if (keyboard)
	{
		if (code == KeyCode::KEY_UP_ARROW) {			// UP
			player_dy += (state == KeyState::KEY_PRESSED ? ANIMATION_STEP : -ANIMATION_STEP);
		}
		else if (code == KeyCode::KEY_DOWN_ARROW) {		// DOWN
			player_dy -= (state == KeyState::KEY_PRESSED ? ANIMATION_STEP : -ANIMATION_STEP);
		}
		else if (code == KeyCode::KEY_RIGHT_ARROW) {		// RIGHT
			player_dx += (state == KeyState::KEY_PRESSED ? ANIMATION_STEP : -ANIMATION_STEP);
		}
		else if (code == KeyCode::KEY_LEFT_ARROW) {		// LEFT
			player_dx -= (state == KeyState::KEY_PRESSED ? ANIMATION_STEP : -ANIMATION_STEP);
		}
	}
}


void timerCallback(TimerID timer)
{
	//플레이어 이동에 따른 이벤트
	if (timer == player_timer)
	{
		player_x += player_dx;
		player_y += player_dy;
		locateObject(player, scene1, player_x, player_y);

		//맵 경계
		if (player_x > 1280)
		{
			player_x = 1280;
			locateObject(player, scene1, player_x, player_y);
		}
		else if (player_x < 0)
		{
			player_x = 0;
			locateObject(player, scene1, player_x, player_y);
		}
		else if (player_y > 685)
		{
			player_y = 685;
			locateObject(player, scene1, player_x, player_y);
		}
		else if (player_y < 0)
		{
			player_y = 0;
			locateObject(player, scene1, player_x, player_y);
		}
		//울타리
		if (player_x > 110 && player_y > 480 && player_y < 490)
		{
			player_y = 480;
			locateObject(player, scene1, player_x, player_y);
		}
		if (player_x > 110 && player_y > 545 && player_y < 554)
		{
			player_y = 554;
			locateObject(player, scene1, player_x, player_y);
		}
		if (player_x > 100 && player_x <= 130 && player_y < 554 && player_y>490)
		{
			player_x = 100;
			locateObject(player, scene1, player_x, player_y);
		}
		//집 도착 
		if (player_x > 1080 && player_y > 540)
		{
			closeGame(true);
		}

		
			

		for (int i = 0; i < 3; i++)
		{
			if (crab_checkin(player_x, player_y, l_crab_x[i], l_crab_y, 40) || crab_checkin(player_x, player_y, r_crab_x[i], r_crab_y, 40))
			{
				closeGame(false);
			}
			//쿠키를 모두 먹었는지 확인
			if (cookie_checkin(player_x, player_y, cookie_x[i], cookie_y[i], 30))
			{
				hideObject(cookie[i]);
				ate[i] = true;
			}
		}
		for (int j = 0; j < 2; j++)
		{
			if (wolf_checkin(player_x, player_y, r_wolf_x[j], r_wolf_y, 50))
			{
				closeGame(false);
			}
			if (bird_checkin(player_x, player_y, l_bird_x[j], l_bird_y, 50))
			{
				closeGame(false);
			}
		}
		if (wolf_checkin(player_x, player_y, l_wolf_x, l_wolf_y, 50))
		{
			closeGame(false);
		}
		if (bird_checkin(player_x, player_y, r_bird_x, r_bird_y, 50))
		{
			closeGame(false);
		}
		if (bluered_checkin(player_x, player_y, red_x, red_y, 40) || bluered_checkin(player_x, player_y, blue_x, blue_y, 40))
		{
			closeGame(false);
		}

		

		setTimer(player_timer, 0.03f);
		startTimer(player_timer);
	}

	//몬스터 이동 
	for (int i = 0; i < 3; i++)
	{
		if (timer == l_crab_timer[i])
		{
			l_crab_x[i] -= 5;
			locateObject(l_crab[i], scene1, l_crab_x[i], l_crab_y);
			if (l_crab_x[i] < -100)
			{
				l_crab_x[i] = 1370;
				locateObject(l_crab[i], scene1, l_crab_x[i], l_crab_y);
			}
			setTimer(l_crab_timer[i], 0.02);
			startTimer(l_crab_timer[i]);
		}
		if (timer == r_crab_timer[i])
		{
			r_crab_x[i] += 5;
			locateObject(r_crab[i], scene1, r_crab_x[i], r_crab_y);
			if (r_crab_x[i] > 1370)
			{
				r_crab_x[i] = -100;
				locateObject(r_crab[i], scene1, r_crab_x[i], r_crab_y);
			}
			setTimer(r_crab_timer[i], 0.02);
			startTimer(r_crab_timer[i]);
		}
	}

	if (timer == l_wolf_timer)
	{
		l_wolf_x -= 5;
		locateObject(l_wolf, scene1, l_wolf_x, l_wolf_y);
		if (l_wolf_x < -300)
		{
			l_wolf_x = 1370;
			locateObject(l_wolf, scene1, l_wolf_x, l_wolf_y);
		}
		setTimer(l_wolf_timer, 0.02);
		startTimer(l_wolf_timer);
	}

	if (timer == r_bird_timer)
	{
		r_bird_x += 5;
		locateObject(r_bird, scene1, r_bird_x, r_bird_y);
		if (r_bird_x > 1370)
		{
			r_bird_x = -300;
			locateObject(r_bird, scene1, r_bird_x, r_bird_y);
		}
		setTimer(r_bird_timer, 0.02);
		startTimer(r_bird_timer);
	}

	for (int j = 0; j < 2; j++)
	{
		if (timer == r_wolf_timer[j])
		{
			r_wolf_x[j] += 5;
			locateObject(r_wolf[j], scene1, r_wolf_x[j], r_wolf_y);
			if (r_wolf_x[j] > 1370)
			{
				r_wolf_x[j] = -300;
				locateObject(r_wolf[j], scene1, r_wolf_x[j], r_wolf_y);
			}
			setTimer(r_wolf_timer[j], 0.02);
			startTimer(r_wolf_timer[j]);
		}

		if (timer == l_bird_timer[j])
		{
			l_bird_x[j] -= 5;
			locateObject(l_bird[j], scene1, l_bird_x[j], l_bird_y);
			if (l_bird_x[j] < -300)
			{
				l_bird_x[j] = 1370;
				locateObject(l_bird[j], scene1, l_bird_x[j], l_bird_y);
			}
			setTimer(l_bird_timer[j], 0.02);
			startTimer(l_bird_timer[j]);
		}

	}

	//레드 움직임
	if (timer == red_timer)
	{
		if (rand() % 4 == 0)
		{
			red_x += 20;
			locateObject(red, scene1, red_x, red_y);
			if (red_x > 1000)
			{
				red_x = 1000;
				locateObject(red, scene1, red_x, red_y);
			}
			setTimer(red_timer, 0.2);
			startTimer(red_timer);
		}
		else if (rand() % 4 == 1)
		{
			red_x -= 20;
			locateObject(red, scene1, red_x, red_y);
			if (red_x < 510)
			{
				red_x = 510;
				locateObject(red, scene1, red_x, red_y);
			}
			setTimer(red_timer, 0.2);
			startTimer(red_timer);
		}
		else if (rand() % 4 == 2)
		{
			red_y += 20;
			locateObject(red, scene1, red_x, red_y);
			if (red_y > 700)
			{
				red_y = 700;
				locateObject(red, scene1, red_x, red_y);
			}
			setTimer(red_timer, 0.2);
			startTimer(red_timer);
		}
		else
		{
			red_y -= 20;
			locateObject(red, scene1, red_x, red_y);
			if (red_y < 560)
			{
				red_y = 560;
				locateObject(red, scene1, red_x, red_y);
			}
			setTimer(red_timer, 0.2);
			startTimer(red_timer);
		}


		
	}
	//블루 움직임
	if (timer == blue_timer)
	{
		if (rand() % 4 == 0)
		{
			blue_x += 20;
			locateObject(blue, scene1, blue_x, blue_y);
			if (blue_x > 500)
			{
				blue_x = 500;
				locateObject(blue, scene1, blue_x, blue_y);
			}
			setTimer(blue_timer, 0.2);
			startTimer(blue_timer);
		}
		else if (rand() % 4 == 1)
		{
			blue_x -= 20;
			locateObject(blue, scene1, blue_x, blue_y);
			if (blue_x < 110)
			{
				blue_x = 110;
				locateObject(blue, scene1, blue_x, blue_y);
			}
			setTimer(blue_timer, 0.2);
			startTimer(blue_timer);
		}
		else if (rand() % 4 == 2)
		{
			blue_y += 20;
			locateObject(blue, scene1, blue_x, blue_y);
			if (blue_y > 700)
			{
				blue_y = 700;
				locateObject(blue, scene1, blue_x, blue_y);
			}
			setTimer(blue_timer, 0.2);
			startTimer(blue_timer);
		}
		else
		{
			blue_y -= 20;
			locateObject(blue, scene1, blue_x, blue_y);
			if (blue_y < 600)
			{
				blue_y = 600;
				locateObject(blue, scene1, blue_x, blue_y);
			}
			setTimer(blue_timer, 0.2);
			startTimer(blue_timer);
		}

	}
	
}


int main()
{
	srand(time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);
	setMouseCallback(mouseCallback);

	scene1 = createScene("배경1", "scene1.png");

	startButton = makeObject("start.png", scene1, 590, 50, true);
	endButton = makeObject("end.png", scene1, 740, 50, true);

	red = makeObject("red.png", scene1, red_x, red_y, false);
	scaleObject(red, 0.5f);

	blue = makeObject("blue.png", scene1, blue_x, blue_y, false);
	scaleObject(blue, 0.5f);

	home = makeObject("home.png", scene1, 1080, 550, true);


	startGame(scene1);

}