#include <iostream>
#include <String>
#include <stdlib.h> 
#include <time.h> 
#include <GL/glut.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <fstream>

#include "StdAfx.h"


using namespace std;

int ran_num = 0, game_over = 0;
double x = 300, y = 300, d = 2.0, w_h = 480, w_w = 640;
bool* keySpecialStates = new bool[246];
bool* keyStates = new bool[256];

double b_y = 250, b_x = 200;
double bullet_x[99999999], bullet_y[99999999], ball_center_x[9999], ball_center_y[9999];
int n = 0, shoot = 0, deleted_bullets[9999999], n_o_deleted_bullets = 0, start_bullet = 0, score = 0,ball_state[9999999],ball_origin[9999999];

int num_of_balls = 0, ball_size_index[9999], dead_balls[9999], level = 1, num_of_dead_balls = 0, num_o_ball_to_create = 0;
double path_index_x[9999], path_index_y[9999], color_r[99999], color_g[9999], color_b[9999];


//value for Bitmap Font
float x_bf = 0.12f, y_bf = 0.10f;

//value for menu and other settings
int start_game = 2, start_delay = 3, menu = 1;

//file operation to save high score
ifstream myfile("hs.txt");
string line;
int is_high_score = 0;


//new space ship config
double ss_x[9999], ss_y[9999], ss_path_x[9999], ss_path_y[9999];
int no_of_ss = 0, no_of_dead_ss = 0, dead_ss[9999],moving[9999], deflect[9999];
//enemy ship bullet config
double ss_bullet_x[9999][999], ss_bullet_y[9999][999];
int ss_bullet_no[9999];


void drawBitmapText(char *string, float x, float y, float z)
{
	char *c;
	glRasterPos3f(x, y, z);

	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}

void drawStrokeText(char*string, int x, int y, int z)
{
	char *c;
	glPushMatrix();
	glTranslatef(x, y + 8, z);
	glScalef(x_bf, y_bf, z);

	for (c = string; *c != '\0'; c++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
	glPopMatrix();
}




void level_init() {
	int j,k=0;
	for (int i = 0; i<level; i++) {
		if (i > 3) continue;
		srand(time(NULL));
		j = ran_num;

		srand(time(NULL));
		ran_num = rand() % 50 + 100;
		if (i == 0) k = ran_num;
		ball_center_x[num_of_balls] = (double)k;
		k = k + 120;
		srand(time(NULL));
		ran_num = rand() % 300 + 100;
		ball_center_y[num_of_balls] = (double)ran_num;

		srand(time(NULL));
		ran_num = rand() % 3 + 3;
		ball_size_index[num_of_balls] = (double)ran_num;
		ball_size_index[num_of_balls] = 5;

		ball_state[num_of_balls] = 1;
		ball_origin[num_of_balls] = 999999;
		srand(time(NULL));
		while (j == ran_num) ran_num = rand() % 4 + 1;

		//if (ran_num == 1) { path_index_x[num_of_balls] = 0; path_index_y[num_of_balls] = 0.01; }
		if (ran_num == 1) { color_r[num_of_balls] = 0.5; color_g[num_of_balls] = 0.8; color_b[num_of_balls] = 0.9; path_index_x[num_of_balls] = 0.05 + (level*0.02); path_index_y[num_of_balls] = 0.05 + (level*0.02); }
		//else if (ran_num == 3) { path_index_x[num_of_balls] = 0.01; path_index_y[num_of_balls] = 0; }
		else if (ran_num == 2) { color_r[num_of_balls] = 0.9; color_g[num_of_balls] = 0.8; color_b[num_of_balls] = 0.5; path_index_x[num_of_balls] = 0.05 + (level*0.02); path_index_y[num_of_balls] = -0.05 - (level*0.02); }
		//else if (ran_num == 5) { path_index_x[num_of_balls] = 0; path_index_y[num_of_balls] = -0.01; }
		else if (ran_num == 3) { color_r[num_of_balls] = 0.7; color_g[num_of_balls] = 0.9; color_b[num_of_balls] = 0.3; path_index_x[num_of_balls] = -0.05 - (level*0.02); path_index_y[num_of_balls] = -0.05 - (level*0.02); }
		//else if (ran_num == 7) { path_index_x[num_of_balls] = -0.01; path_index_y[num_of_balls] = 0; }
		else if (ran_num == 4) { color_r[num_of_balls] = 0.7; color_g[num_of_balls] = 0.4; color_b[num_of_balls] = 0.1; path_index_x[num_of_balls] = -0.05 - (level*0.02); path_index_y[num_of_balls] = 0.05 + (level*0.02); }
		num_of_balls++;
	}
}

void create_new_ball(int ball_i, double x, double y) {
	if (ball_i >1) {
		int size = rand() % (ball_i - 1) + 1;
		int tmp = num_o_ball_to_create;
		while (tmp == num_o_ball_to_create && ball_i != 2) num_o_ball_to_create = rand() % (ball_i - 1) + 1;
		for (int i = 0; i < num_o_ball_to_create; i++) {

			ball_center_x[num_of_balls] = x;
			ball_center_y[num_of_balls] = y;
			int j = size;
			if (size != 1 || size == 2) { while (j == size) size = rand() % (ball_i - 1) + 1; }
			ball_size_index[num_of_balls] = size;
			ball_state[num_of_balls] = 0;
			ball_origin[num_of_balls] = ball_i;
			//srand(time(NULL));
			ran_num = i + 1;
			//if (ran_num == 1) { path_index_x[num_of_balls] = 0; path_index_y[num_of_balls] = 0.01; }
			if (ran_num == 1) { color_r[num_of_balls] = 0.5; color_g[num_of_balls] = 0.8; color_b[num_of_balls] = 0.9; path_index_x[num_of_balls] = 0.05 + (level*0.02); path_index_y[num_of_balls] = 0.05 + (level*0.02); }
			//else if (ran_num == 3) { path_index_x[num_of_balls] = 0.01; path_index_y[num_of_balls] = 0; }
			else if (ran_num == 2) { color_r[num_of_balls] = 0.9; color_g[num_of_balls] = 0.8; color_b[num_of_balls] = 0.5; path_index_x[num_of_balls] = 0.05 + (level*0.02); path_index_y[num_of_balls] = -0.05 - (level*0.02); }
			//else if (ran_num == 5) { path_index_x[num_of_balls] = 0; path_index_y[num_of_balls] = -0.01; }
			else if (ran_num == 3) { color_r[num_of_balls] = 0.7; color_g[num_of_balls] = 0.9; color_b[num_of_balls] = 0.3; path_index_x[num_of_balls] = -0.05 - (level*0.02); path_index_y[num_of_balls] = -0.05 - (level*0.02); }
			//else if (ran_num == 7) { path_index_x[num_of_balls] = -0.01; path_index_y[num_of_balls] = 0; }
			else if (ran_num == 4) { color_r[num_of_balls] = 0.7; color_g[num_of_balls] = 0.4; color_b[num_of_balls] = 0.1; path_index_x[num_of_balls] = -0.05 - (level*0.02); path_index_y[num_of_balls] = 0.05 + (level*0.02); }
			num_of_balls++;
			//if (ball_center_y[num_of_balls - 1] > (double)w_h) { ball_center_y[num_of_balls - 1] = (double)w_h - (double)ball_size_index[num_of_balls - 1] * 6 - 2; }
			//if (ball_center_y[num_of_balls - 1] < 0) { ball_center_y[num_of_balls - 1] = 0 + (double)ball_size_index[num_of_balls - 1] * 6 + 2; }
			//if (ball_center_x[num_of_balls - 1] > (double)w_w) { ball_center_x[num_of_balls - 1] = (double)w_w - (double)ball_size_index[num_of_balls - 1] * 6 - 2; }
			//if (ball_center_x[num_of_balls - 1] > 0) { ball_center_x[num_of_balls - 1] = 0 - (double)ball_size_index[num_of_balls - 1] * 6 + 2; }
		}
	}
}



void keySpecial(int key, int x, int y) {
	keySpecialStates[key] = true;
}

void keySpecialUp(int key, int x, int y) {
	keySpecialStates[key] = false;
}


void keyPressed(unsigned char key, int x, int y) {
	keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

void keySpecialOperations(void) {
	while (true) {
		if (game_over == 0 && start_delay<1) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (keySpecialStates[GLUT_KEY_LEFT] && x - d > 0) {
				x = x - d;
			}
			else if (keySpecialStates[GLUT_KEY_RIGHT] && x + d < w_w - 60) {
				x = x + d;
			}
			else if (keySpecialStates[GLUT_KEY_UP] && y + d < w_h - 20) {
				y = y + d;
			}
			else if (keySpecialStates[GLUT_KEY_DOWN] && y - d > 0) {
				y = y - d;
			}
		}
	}
}

void keyOperations(void) {
	while (true) {
		if (game_over == 0 && start_delay<1 && start_game == 1) {
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			if (keyStates['a']) {
				shoot = 1;
				bullet_x[n] = x + 30;
				bullet_y[n] = y + 20;
				n++;
			}
			else if (keyStates['a'] == false) {
				shoot = 0;
			}
		}
		else if (menu == 1) {
			if (keyStates['s']) {
				level = 1;
				num_of_balls = 0;
				num_of_dead_balls = 0;
				n_o_deleted_bullets = 0;
				start_bullet = 0;
				n = 0;
				level_init();
				start_game = 0;
				start_delay = 3;
				game_over = 0;
				score = 0;
				menu = 0;
				is_high_score = 0;
				no_of_ss = 0;
				no_of_dead_ss = 0;
			}
			if (keyStates['h']) {
				ifstream myfile("hs.txt");
				if (myfile.is_open()) {
					while (getline(myfile, line)) {
						line = line;
					}
					myfile.close();
				}
				menu = 2;
			}
			if (keyStates['e']) {
				exit(0);
			}
		}
		else if (menu == 2) {
			if (keyStates['m']) {
				menu = 1;
			}
		}
		if (game_over == 1) {
			if (keyStates['r']) {
				level = 1;
				num_of_balls = 0;
				num_of_dead_balls = 0;
				n_o_deleted_bullets = 0;
				start_bullet = 0;
				n = 0;
				level_init();
				start_game = 0;
				start_delay = 3;
				game_over = 0;
				score = 0;
				is_high_score = 0;
				no_of_ss = 0;
				no_of_dead_ss = 0;
			}
			if (keyStates['m']) {
				menu = 1;
			}
		}
	}
}


//! Draw Filled Circle to bitmap or passed bitmap 
void doCirclefill(int b_i,double x, double y, double radius)
{
	glEnable(GL_BLEND);
	glColor4f(color_r[b_i], color_g[b_i], color_b[b_i], 0.0);
	double y1 = y;
	double x1 = x;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i <= 360; i++)
	{
		double angle = (float)(((double)i) / 57.29577957795135);
		double x2 = x + (radius*(float)sin((double)angle));
		double y2 = y + (radius*(float)cos((double)angle));
		glVertex2d(x, y);
		glVertex2d(x1, y1);
		glVertex2d(x2, y2);
		y1 = y2;
		x1 = x2;
	}
	glEnd();
	glDisable(GL_BLEND);
}


string num_to_str(int num) {
	string s = "";
	do {
		int mod = num % 10;
		if (mod == 1) {
			s = '1' + s;
		}
		else if (mod == 2) {
			s = '2' + s;
		}
		else if (mod == 3) {
			s = '3' + s;
		}
		else if (mod == 4) {
			s = '4' + s;
		}
		else if (mod == 5) {
			s = '5' + s;
		}
		else if (mod == 6) {
			s = '6' + s;
		}
		else if (mod == 7) {
			s = '7' + s;
		}
		else if (mod == 8) {
			s = '8' + s;
		}
		else if (mod == 9) {
			s = '9' + s;
		}
		else if (mod == 0) {
			s = '0' + s;
		}
		num = num / 10;
	} while (num != 0);
	return s;
}


int string_to_num(string s) {
	int n = 0;
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '0') {
			n = n + 0;
		}
		else if (s[i] == '1') {
			n = n + 1;
		}
		else if (s[i] == '2') {
			n = n + 2;
		}
		else if (s[i] == '3') {
			n = n + 3;
		}
		else if (s[i] == '4') {
			n = n + 4;
		}
		else if (s[i] == '5') {
			n = n + 5;
		}
		else if (s[i] == '6') {
			n = n + 6;
		}
		else if (s[i] == '7') {
			n = n + 7;
		}
		else if (s[i] == '8') {
			n = n + 8;
		}
		else if (s[i] == '9') {
			n = n + 9;
		}
		else {
			return 0;
		}
		n = n * 10;
	}
	n = n / 10;
	return n;
}


void check_high_score() {
	ifstream myfile("hs.txt");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			//line = line;
			break;
		}
		myfile.close();
	}
	if (string_to_num(line) < score) {
		is_high_score = 1;
		ofstream myfile1("hs.txt");
		if (myfile1.is_open()) {
			myfile1 << num_to_str(score);
			myfile1.close();
		}
	}
}


void myDisplay(void)
{
	if (num_of_balls == num_of_dead_balls) {
		level++;
		num_of_balls = 0;
		num_of_dead_balls = 0;
		n_o_deleted_bullets = 0;
		start_bullet = 0;
		n = 0;
		level_init();
		start_game = 0;
		start_delay = 3;

		no_of_ss = 0;
		no_of_dead_ss = 0;
	}
	//keyOperations();
	//keySpecialOperations();
	glClear(GL_COLOR_BUFFER_BIT);
	if (game_over == 0) {
		glColor3f(1.0, 1.0, 1.0);
	}
	else {
		glColor3f(1.0, 0.0, 0.0);
	}
	if (menu == 1) {
		x_bf = 0.38f; y_bf = 0.36f;
		glColor3f(0, 1, 0);
		drawStrokeText("SHOOT THE BUBBLE", 80, 400, 0);
		x_bf = 0.32f; y_bf = 0.30f;
		glColor3f(0, 1, 0);
		drawStrokeText("Main Menu", 200, 350, 0);
		x_bf = 0.12f; y_bf = 0.10f;
		drawStrokeText("1. Start Game 'S'", 230, 310, 0);
		drawStrokeText("2. High Score 'H'", 230, 290, 0);
		drawStrokeText("3. Exit Game 'E'", 230, 270, 0);

		//string ttd;
		//ttd = "Starting " + num_to_str(start_delay);
		//char *a = &ttd[0];
		//drawStrokeText(a, 200, 220, 0);
		x_bf = 0.12f; y_bf = 0.10f;
	}
	else if (menu == 2) {
		x_bf = 0.38f; y_bf = 0.36f;
		glColor3f(0, 1, 0);
		drawStrokeText("SHOOT THE BUBBLE", 80, 400, 0);
		x_bf = 0.32f; y_bf = 0.30f;
		glColor3f(0, 0, 1);
		string ttd;
		ttd = "High Score: " + line;
		char *a = &ttd[0];
		drawStrokeText(a, 170, 250, 0);
		glColor3f(0, 1, 0);
		x_bf = 0.12f; y_bf = 0.10f;
		drawStrokeText("Press \"M\" to go Main Menu ", 200, 220, 0);
		x_bf = 0.12f; y_bf = 0.10f;
	}
	else if (menu == 0) {
		glPointSize(2.0);
		glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + 30, y + 20);
		glVertex2i(x + 30, y + 10);
		glVertex2i(x, y);

		glVertex2i(x + 30, y + 20);
		glVertex2i(x + 60, y);
		glVertex2i(x + 30, y + 10);
		glVertex2i(x + 30, y + 20);


		glColor3f(1.0, 0.3, 0.4);
		for (int i = 0; i < no_of_ss; i++) {
			int is_dead_ss = 0;
			for (int k = 0; k < no_of_dead_ss; k++) {
				if (dead_ss[k] == i) { is_dead_ss = 1; break; }
			}
			if (is_dead_ss == 0) {
				if (ss_y[i] + ss_path_y[i] < 0) { dead_ss[no_of_dead_ss] = i; no_of_dead_ss++; continue; }
				if (ss_x[i] + ss_path_x[i] > deflect[i] + 80 || ss_x[i] + ss_path_x[i] < deflect[i] - 80) {
					ss_path_x[i] = -ss_path_x[i];
				}
				if (game_over == 0 && start_delay < 1) {
					ss_x[i] = ss_x[i] + ss_path_x[i];
					ss_y[i] = ss_y[i] + ss_path_y[i];
				}
				
				glVertex2i(ss_x[i], ss_y[i]);
				glVertex2i(ss_x[i] - 30, ss_y[i] - 20);
				glVertex2i(ss_x[i] - 30, ss_y[i] - 10);
				glVertex2i(ss_x[i], ss_y[i]);
				
				glVertex2i(ss_x[i] - 30, ss_y[i] - 20);
				glVertex2i(ss_x[i] - 60, ss_y[i]);
				glVertex2i(ss_x[i] - 30, ss_y[i] - 10);
				glVertex2i(ss_x[i] - 30, ss_y[i] - 20);
			}
		}

		glEnd();

		for (int i = 0; i < num_of_balls; i++) {
			int is_dead_ball = 0;
			for (int k = 0; k < num_of_dead_balls; k++) {
				if (dead_balls[k] == i) { is_dead_ball = 1; break; }
			}
			if (is_dead_ball == 0) {
				if (ball_center_y[i] - ball_size_index[i] * 6 + path_index_y[i] <= 0.0 || ball_center_y[i] + ball_size_index[i] * 6 + path_index_y[i] >(double)w_h) {
					path_index_y[i] = -path_index_y[i];
				}
				if (ball_center_x[i] - ball_size_index[i] * 6 + path_index_x[i] <= 0.0 || ball_center_x[i] + ball_size_index[i] * 6 + path_index_x[i] >(double)w_w) {
					path_index_x[i] = -path_index_x[i];
				}
				if (game_over == 0 && start_delay < 1) {
					for (int h = 0; h < num_of_balls; h++) {
						int is_dead_ball_h = 0;
						for (int k = 0; k < num_of_dead_balls; k++) {
							if (dead_balls[k] == h) { is_dead_ball_h = 1; break; }
						}
						if (i != h && is_dead_ball_h == 0) {
							if (sqrt(pow((ball_center_x[i] - ball_center_x[h]), 2) + pow((ball_center_y[i] - ball_center_y[h]), 2))<ball_size_index[i] * 6 + ball_size_index[h] * 6) {
								if (ball_state[i] == 0 || ball_state[h] == 0) {
									if (ball_origin[i]==ball_origin[h]) {
										//cout << "i: " << i << " h: " << h << endl;
										//cout << "rad act: " << sqrt(pow((ball_center_x[i] - ball_center_x[h]), 2) + pow((ball_center_y[i] - ball_center_y[h]), 2)) << " rad  h: " << ball_size_index[h] * 6 + ball_size_index[i] * 6 << endl;
									}
								}
								else {
									if (path_index_x[i] == path_index_x[h] && path_index_y[i] != path_index_y[h]) {
										path_index_y[h] = -path_index_y[h];
										path_index_y[i] = -path_index_y[i];

									}
									else if (path_index_x[i] != path_index_x[h] && path_index_y[i] == path_index_y[h]) {
										path_index_x[h] = -path_index_x[h];
										path_index_x[i] = -path_index_x[i];

									}
									else {
										path_index_x[h] = -path_index_x[h];
										path_index_x[i] = -path_index_x[i];
										path_index_y[h] = -path_index_y[h];
										path_index_y[i] = -path_index_y[i];

									}
									if (ball_center_y[i] - ball_size_index[i] * 6 + path_index_y[i] <= 0.0 || ball_center_y[i] + ball_size_index[i] * 6 + path_index_y[i] >(double)w_h) {
										path_index_y[i] = -path_index_y[i];
									}
									if (ball_center_x[i] - ball_size_index[i] * 6 + path_index_x[i] <= 0.0 || ball_center_x[i] + ball_size_index[i] * 6 + path_index_x[i] >(double)w_w) {
										path_index_x[i] = -path_index_x[i];
									}
									if (ball_center_y[h] - ball_size_index[h] * 6 + path_index_y[h] <= 0.0 || ball_center_y[h] + ball_size_index[h] * 6 + path_index_y[h] >(double)w_h) {
										path_index_y[h] = -path_index_y[h];
									}
									if (ball_center_x[h] - ball_size_index[h] * 6 + path_index_x[h] <= 0.0 || ball_center_x[h] + ball_size_index[h] * 6 + path_index_x[h] >(double)w_w) {
										path_index_x[h] = -path_index_x[h];
									}
								}
							}
								//if (sqrt(pow((ball_center_x[i] - ball_center_x[h]), 2) + pow((ball_center_y[i] - ball_center_y[h]), 2)) < ball_size_index[i] * 6 + ball_size_index[h] * 6) {
									
								//}
							if (ball_state[i] == 0 || ball_state[h] == 0) {
								if (ball_origin[i] == ball_origin[h]) {
									//cout << "i: " << i << " h: " << h << endl;
									//cout << "rad act: " << sqrt(pow((ball_center_x[i] - ball_center_x[h]), 2) + pow((ball_center_y[i] - ball_center_y[h]), 2)) << " rad  h: " << ball_size_index[h] * 6 + ball_size_index[i] * 6 << endl;
									while (sqrt(pow((ball_center_x[i] - ball_center_x[h]), 2) + pow((ball_center_y[i] - ball_center_y[h]), 2)) < ball_size_index[i] * 6 + ball_size_index[h] * 6) {
										//if (ball_origin[i] == 999999 && ball_origin[h] == 999999) break;
										ball_center_x[i]+= path_index_x[i];
										ball_center_x[h] += path_index_x[h];
										ball_center_y[i] += path_index_y[i];
										ball_center_y[h] += path_index_y[h];
									}
									ball_state[i] = 1;
								}
							}
						}
					}
					ball_center_y[i] = ball_center_y[i] + path_index_y[i];
					ball_center_x[i] = ball_center_x[i] + path_index_x[i];
				}
				doCirclefill(i,ball_center_x[i], ball_center_y[i], ball_size_index[i] * 6);
				glEnd();
			}
		}

		glEnd();

		glBegin(GL_POINTS);
		glColor3f(1.0, 1.0, 0.0);
		glPointSize(10.0);

		/*if (n == 0 && shoot == 1) {
		bullet_x[n] = x + 30;
		bullet_y[n] = y + 20;
		n++;
		}
		else if (shoot == 1 && bullet_y[n - 1]>y + 60) {
		bullet_x[n] = x + 30;
		bullet_y[n] = y + 20;
		n++;
		}
		else if (shoot == 1) {
		int dd = 0;
		for (int j = 0; j < n_o_deleted_bullets; j++) {
		if (deleted_bullets[j] == n - 1) { dd = 1; break; }
		}
		if (dd != 0) {
		bullet_x[n] = x + 30;
		bullet_y[n] = y + 20;
		n++;
		}
		}*/

		for (int i = start_bullet; i < n; i++) {
			int dd = 0;
			for (int j = 0; j < n_o_deleted_bullets; j++) {
				if (deleted_bullets[j] == i) { dd = 1; break; }
			}
			if (dd == 0) {
				glVertex2i(bullet_x[i], bullet_y[i]);
			}
			if (game_over == 0 && start_delay < 1) {
				bullet_y[i] = bullet_y[i] + 0.1;
			}
		}
		glPointSize(15.0);
		glColor3f(0.6, 1.0, 0.8);
		for (int i = 0; i < no_of_ss; i++) {
			for (int j = 0; j < ss_bullet_no[i]; j++) {
				if (ss_bullet_y[i][ss_bullet_no[i] - 1]<0) { break; }
				if (game_over == 0 && start_delay < 1) {
					ss_bullet_y[i][j] = ss_bullet_y[i][j] - 0.07;
				}
				glVertex2i(ss_bullet_x[i][j], ss_bullet_y[i][j]);
			}
		}

		for (int i = start_bullet; i < n; i++) {
			if (bullet_y[i]>w_h) {
				start_bullet = i;
			}
		}

		glEnd();

		glColor3f(0, 1, 0);
		string ttd = " Level: " + num_to_str(level);
		ttd = ttd + " - Time: 0 - Score: " + num_to_str(score);
		char *a;
		a = &ttd[0];
		drawStrokeText(a, 180, 450, 0);

		if (game_over == 1) {
			x_bf = 0.32f; y_bf = 0.30f;
			glColor3f(1, 0, 0);
			drawStrokeText("Game Over", 200, 240, 0);
			glColor3f(0, 1, 0);
			string ttd;
			if (is_high_score == 0) {
				x_bf = 0.27f; y_bf = 0.25f;
				ttd = "Score: " + num_to_str(score);
				char *a;
				a = &ttd[0];
				drawStrokeText(a, 230, 200, 0);
			}
			else {
				x_bf = 0.27f; y_bf = 0.25f;
				ttd = "New High Score: " + num_to_str(score);
				char *a;
				a = &ttd[0];
				drawStrokeText(a, 150, 200, 0);
			}
			x_bf = 0.12f; y_bf = 0.10f;
			drawStrokeText("Press \"R\" Button to Restart", 200, 170, 0);
			drawStrokeText("Press \"M\" to go Main Menu ", 200, 150, 0);
		}
		if (start_delay > 0) {
			x_bf = 0.32f; y_bf = 0.30f;
			glColor3f(1, 0, 0);
			ttd = "Starting " + num_to_str(start_delay);
			a = &ttd[0];
			drawStrokeText(a, 200, 220, 0);
			x_bf = 0.12f; y_bf = 0.10f;
		}
	}
	glFlush();
	glutPostRedisplay();
}


void detect_bullet_ball() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (menu == 0) {
			for (int i = start_bullet; i < n; i++) {
				int dd = 0;
				for (int j = 0; j < n_o_deleted_bullets; j++) {
					if (deleted_bullets[j] == i) { dd = 1; break; }
				}
				if (dd == 0) {
					for (int m = 0; m < num_of_balls; m++) {
						int is_dead_ball = 0;
						for (int k = 0; k < num_of_dead_balls; k++) {
							if (dead_balls[k] == m) { is_dead_ball = 1; break; }
						}
						if (is_dead_ball == 0) {
							if (ball_center_y[m] - ball_size_index[m] * 6 < bullet_y[i] + 0.05 && ball_center_y[m] + ball_size_index[m] * 6 > bullet_y[i] + 0.05 && ball_center_x[m] - ball_size_index[m] * 6 < bullet_x[i] && ball_center_x[m] + ball_size_index[m] * 6 > bullet_x[i]) {
								create_new_ball(ball_size_index[m], ball_center_x[m], ball_center_y[m]);
								deleted_bullets[n_o_deleted_bullets] = i;
								n_o_deleted_bullets++;
								dead_balls[num_of_dead_balls] = m;
								num_of_dead_balls++;
								score++;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void detect_ball_ship_collision() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (menu == 0) {
			for (int i = 0; i < num_of_balls; i++) {
				if (game_over == 1) { break; }
				int is_dead_ball = 0;
				for (int k = 0; k < num_of_dead_balls; k++) {
					if (dead_balls[k] == i) { is_dead_ball = 1; break; }
				}
				if (is_dead_ball == 0) {
					double inc = 30 / 20;
					double x_x = x, y_y = y;
					for (double h = 0; h < 20; h++) {
						if (sqrt(pow((x_x - ball_center_x[i]), 2) + pow((y_y - ball_center_y[i]), 2)) < ball_size_index[i] * 3) {
							game_over = 1;
							check_high_score();
						}
						x_x = x_x + inc;
						y_y = y_y + 1;
					}
					x_x = x + 30, y_y = y + 20;
					for (double h = 0; h < 20; h++) {
						if (sqrt(pow((x_x - ball_center_x[i]), 2) + pow((y_y - ball_center_y[i]), 2)) < ball_size_index[i] * 6) {
							game_over = 1;
							check_high_score();
						}
						x_x = x_x + inc;
						y_y = y_y - 1;
					}
					inc = 30 / 10;
					x_x = x, y_y = y;
					for (double h = 0; h < 10; h++) {
						if (sqrt(pow((x_x - ball_center_x[i]), 2) + pow((y_y - ball_center_y[i]), 2)) < ball_size_index[i] * 6) {
							game_over = 1;
							check_high_score();
						}
						x_x = x_x + inc;
						y_y = y_y + 1;
					}
					inc = 30 / 10;
					x_x = x + 30, y_y = y + 10;
					for (double h = 0; h < 10; h++) {
						if (sqrt(pow((x_x - ball_center_x[i]), 2) + pow((y_y - ball_center_y[i]), 2)) < ball_size_index[i] * 6) {
							game_over = 1;
							check_high_score();
						}
						x_x = x_x + inc;
						y_y = y_y - 1;
					}
					/*if (x<ball_center_x[i] && x + 60>ball_center_x[i] && y < ball_center_y[i] && y + 20 > ball_center_y[i]) {
					if (game_over == 0) game_over = 1; else game_over = 0;
					}*/
				}
			}
		}
	}
}

void level_start_delay() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (start_game == 0) {
			if (start_delay == 1) { start_game = 1; }
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			start_delay--;
		}
	}
}
void generate_enemy_ss() {
	int j1 = 0,j2=0, ran_num1 = 0;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (menu == 0 && start_delay<1 && game_over==0) {
			if (no_of_ss==0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			}
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(10000));
			}
			if (game_over == 1) continue;
			srand(time(NULL));
			ran_num1 = rand() % 3 + 1;
			while (j2 == ran_num1) { ran_num1 = rand() % 2 + 1; }
			int no_ss = ran_num1, j1 = 0; ran_num1 = 0;
			j2 = no_ss;
			//cout << "no of ss: "<<j2<<endl;

			for (int m = 0; m < no_ss; m++) {
				//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				srand(time(NULL));
				if (m==0)ran_num1 = rand() % 100 + 100;
				while (j1==ran_num1 && m==0) { srand(time(NULL)); ran_num1 = rand() % 150 + 100; }
				if (m == 0) j1 = ran_num1;
				ss_x[no_of_ss] = j1+130.00f;
				ss_y[no_of_ss] = w_h;
				ss_path_x[no_of_ss] = 0.0;
				ss_path_y[no_of_ss] = -0.01;

				srand(time(NULL)); int ran_num2 = rand() % 2 + 1, ran_num3 = rand() % 2 + 4;
				if (ran_num2 == 1 && ran_num3==4) { moving[no_of_ss] = 0; }
				else { moving[no_of_ss] = 1; deflect[no_of_ss] = ss_x[no_of_ss]; ss_path_x[no_of_ss] = 0.02; }
				j1=j1 + 100;
				ss_bullet_no[no_of_ss] = 0;
				no_of_ss++;
			}
			j1 = 0, ran_num1 = 0;
		}
	}
}
void generate_enemy_ss_bullet() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (menu == 0 && start_delay < 1) {
			for (int i = 0; i < no_of_ss; i++) {
				int is_dead_ss = 0;
				for (int k = 0; k < no_of_dead_ss; k++) {
					if (dead_ss[k] == i) { is_dead_ss = 1; break; }
				}
				if (is_dead_ss == 0) {
					ss_bullet_x[i][ss_bullet_no[i]] = ss_x[i]-30;
					ss_bullet_y[i][ss_bullet_no[i]] = ss_y[i]-20;
					ss_bullet_no[i]++;
				}
			}
		}
	}
}

void enemy_bullet_to_me() {
	while (true) {
		if (menu == 0 && start_delay < 1) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			for (int i = 0; i < no_of_ss; i++) {
				for (int j = 0; j < ss_bullet_no[i]; j++) {
					if (ss_bullet_y[i][ss_bullet_no[i] - 1]<0) { break; }
					if (x<ss_bullet_x[i][j] && x + 60>ss_bullet_x[i][j] && y<ss_bullet_y[i][j] && y + 20>ss_bullet_y[i][j]) {
						game_over = 1;
						check_high_score();
					}
				}
			}
		}
	}
}

void destroy_enemy_ss() {
	while (true) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (menu == 0 && start_delay < 1) {
			for (int i = 0; i < no_of_ss; i++) {
				int is_dead_ss = 0;
				for (int k = 0; k < no_of_dead_ss; k++) {
					if (dead_ss[k] == i) { is_dead_ss = 1; break;}
				}
				if (is_dead_ss == 0) {
					for (int f = start_bullet; f < n; f++) {
						int dd = 0;
						for (int j = 0; j < n_o_deleted_bullets; j++) {
							if (deleted_bullets[j] == f) { dd = 1; break; }
						}
						if (dd == 0) {
							if (ss_x[i]>bullet_x[f] && ss_x[i] - 60<bullet_x[f] && ss_y[i]>bullet_y[f] && ss_y[i] - 20<bullet_y[f]) {
								score = score+5;
								dead_ss[no_of_dead_ss] = i;
								deleted_bullets[n_o_deleted_bullets] = f;
								n_o_deleted_bullets++;
								no_of_dead_ss++;
								//cout << "dead: "<<i<< endl;
							}
						}
					}
				}
			}



			//ball to ball
			/*for (int i = 0; i < num_of_balls; i++) {
				if (game_over == 1) { break; }
				int is_dead_ball = 0;
				for (int k = 0; k < num_of_dead_balls; k++) {
					if (dead_balls[k] == i) { is_dead_ball = 1; break; }
				}
				for (int h = 0; h < num_of_balls; h++) {
					int is_dead_ball_h = 0;
					for (int k = 0; k < num_of_dead_balls; k++) {
						if (dead_balls[k] == i) { is_dead_ball_h = 1; break; }
					}
					if (is_dead_ball == 0 && i != h && is_dead_ball_h == 0) {
						if (sqrt(pow((ball_center_x[i] - ball_center_x[h]), 2) + pow((ball_center_y[i] - ball_center_y[h]), 2))<ball_size_index[i]*6 + ball_size_index[i]*6) {
							path_index_x[h] = -path_index_x[h];
							path_index_x[i] = -path_index_x[i];
						}
					}
				}
			}*/



		}
	}
}


void detect_ss_me_collision() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (menu == 0 && start_delay < 1) {
			for (int i = 0; i < no_of_ss; i++) {
				int is_dead_ss = 0;
				for (int k = 0; k < no_of_dead_ss; k++) {
					if (dead_ss[k] == i) { is_dead_ss = 1; break; }
				}
				if (is_dead_ss == 0) {
					if (ss_x[i] - 30 > x && ss_x[i] - 30 < x + 60 && ss_y[i] - 20 > y && ss_y[i] - 20 < y + 20) {
						game_over = 1;
						check_high_score();
					}
					if (ss_x[i] > x && ss_x[i] < x + 60 && ss_y[i] > y && ss_y[i] < y + 20) {
						game_over = 1;
						check_high_score();
					}
					if (ss_x[i] - 60 > x && ss_x[i] - 60 < x + 60 && ss_y[i] > y && ss_y[i] < y + 20) {
						game_over = 1;
						check_high_score();
					}
				}
			}
		}
	}
}

void myInit(void)
{
	keySpecialStates[GLUT_KEY_LEFT] = false;
	keySpecialStates[GLUT_KEY_RIGHT] = false;
	keySpecialStates[GLUT_KEY_UP] = false;
	keySpecialStates[GLUT_KEY_DOWN] = false;
	keyStates['a'] = false;
	keyStates['r'] = false;
	keyStates['s'] = false;
	keyStates['h'] = false;
	keyStates['e'] = false;
	keyStates['m'] = false;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);

	glutSpecialFunc(keySpecial);
	glutSpecialUpFunc(keySpecialUp);

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);

}

void main(int argc, char** argv)
{
	srand(time(NULL));
	ran_num = rand() % 400 + 100;
	b_x = (double)ran_num;
	printf("%lf", b_x);
	level_init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(160, 150);
	glutCreateWindow("Shoot The Bubble");
	glutDisplayFunc(myDisplay);
	myInit();
	thread t1(keySpecialOperations);
	thread t2(keyOperations);
	thread t3(detect_bullet_ball);
	thread t4(detect_ball_ship_collision);
	thread t5(level_start_delay);
	thread t6(generate_enemy_ss);
	thread t7(generate_enemy_ss_bullet);
	thread t8(enemy_bullet_to_me);
	thread t9(destroy_enemy_ss);
	thread t10(detect_ss_me_collision);
	glutMainLoop();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	t10.join();
}