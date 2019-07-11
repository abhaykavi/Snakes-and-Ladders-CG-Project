#include <GL/glut.h>
#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
int player_1_position = 0;
int player_2_position = 0;
int turn = 0;
int diceVal;
int rolling = 1;
int victory = 0;

int first,menu,game;
char msg[100] = "Press right arrow to roll dice";
// int array to store snake and ladder positions
int * snakes;
int * ladders;
//Writes number inside a box in the board
void renderNumber(char *text, int length, double x, double y)
{
glColor3f(1, 1, 1);
double x_pos = length/56.0;
glMatrixMode(GL_PROJECTION);
double *matrix = new double[16];
glGetDoublev(GL_PROJECTION_MATRIX, matrix);
glLoadIdentity();
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glPushMatrix();
glLoadIdentity();
glRasterPos2f(x, y);
for(int i=0; i<length; i++)
{
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]); //Draw text by bit map
}
glPopMatrix();
glMatrixMode(GL_PROJECTION);
glLoadMatrixd(matrix);
glMatrixMode(GL_MODELVIEW);
}
void drawNumbers()
{
double x = -.92;
double y = -.9;
int num = 1, length, flag=1;
for(int i = 0; i < 10; i++)
{
x = -0.92;
for(int j = 0; j<10; j++)
{
char buffer[2];
char *text = itoa(num,buffer,10);
renderNumber(text, strlen(text), x, y);
x = x+0.2;
if(flag)
num = num+1;
else
num = num-1;
}
if(flag)
{
num = num+9;
flag = 0;
}
else
{
num = num+11;
flag = 1;
}
length = 2;
y = y+0.16;
}
}
void giveMessage(const char *text, int length,int turn)
{
char *nextmsg;
nextmsg = new char[20];
if(turn == 1)
strcpy(nextmsg,"Player 1 roll the dice");
else
strcpy(nextmsg,"Player 2 roll the dice");
glColor3f(1, 1, 1);
double x_pos = length/56.0;
glMatrixMode(GL_PROJECTION);
double *matrix = new double[16];
glGetDoublev(GL_PROJECTION_MATRIX, matrix);
glLoadIdentity();
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glPushMatrix();
glLoadIdentity();
glRasterPos2f(-x_pos, .8);
for(int i=0; i<length; i++)
{
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
}
glRasterPos2f(-x_pos, .75);
for(int j=0; j<strlen(nextmsg); j++)
{
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)nextmsg[j]);
}
glPopMatrix();
glMatrixMode(GL_PROJECTION);
glLoadMatrixd(matrix);
glMatrixMode(GL_MODELVIEW);
}
void drawGrid()
{
glColor3f(1.0,1.0,1.0);
glLineWidth(3.0);
//This loop renders the horizontal lines of the board.
for(float y = 0.667; y>=-1; y = y-0.163)
{
glBegin(GL_LINES);
glVertex2f(-296, y); glVertex2f(296, y);
glEnd();
}
// This loop renderes the vertical lines of the board
for(float x = -0.995; x<=1; x = x+0.199)
{
glBegin(GL_LINES);
glVertex2f(x, 0.667); glVertex2f(x, -.96);
glEnd();
}
}
void drawLadders()
{
glLineWidth(5.0);
glColor3f(0, 1, 0);
glBegin(GL_LINES);
glVertex2f(-.85, -.84); glVertex2f(-.56, -.4);
glVertex2f(-0.25, -.84); glVertex2f(.23, -.78);
glVertex2f(0.75, -.84); glVertex2f(.86, -.4);
glVertex2f(-0.85, -.5); glVertex2f(-.78, -.28);
glVertex2f(0.45, -.5); glVertex2f(-.23, .38);
glVertex2f(0.85, 0); glVertex2f(.35, 0.05);
glVertex2f(0.73, 0.3); glVertex2f(.82, 0.55);
glVertex2f(-0.85, 0.31); glVertex2f(-.75, 0.55);
glEnd();
}
void drawSnakes()
{
glLineWidth(5.0);
glColor3f(1, 0, 0);
glBegin(GL_LINES);
glVertex2f(-0.65, 0.31); glVertex2f(-.55, 0.55);
glVertex2f(0.15, 0.31); glVertex2f(0.18, 0.55);
glVertex2f(0.55, 0.31); glVertex2f(0.63, 0.55);
glVertex2f(0.35, 0.37); glVertex2f(-0.05, -0.34);
glVertex2f(-0.65, 0.06); glVertex2f(-0.62, -0.68);
glVertex2f(0.25, -0.12); glVertex2f(0.25, -0.38);
glVertex2f(-0.85, -0.03); glVertex2f(-0.35, 0.04);
glVertex2f(-.25, -0.78); glVertex2f(0.25, -0.83);
glEnd();
}
void drawPawn(float cx, float cy)
{
float r = 0.02;
int num_segments = 360;
glLineWidth(3);
glBegin(GL_LINE_LOOP);
for(int ii = 0; ii < num_segments; ii++)
{
float theta = 2.0f * 3.1415926f * float(ii) /
float(num_segments);//get the current angle
float x = r * cosf(theta);//calculate the x component
float y = r * sinf(theta);//calculate the y component
glVertex2f(x + cx, y + cy);//output vertex
}
glEnd();
}
void drawPlayer(int player_id, int position)
{
double x, y, x_increment = 0.199, y_increment = 0.1625, x_pos, y_pos;
if(player_id)
{
glColor3f(0.88, 0.051, 0.79);
x = -0.85;
y = -0.915;
}
else
{
glColor3f(0, 0.0, 1.0);
x = -0.95;
y = -0.836;
}
int units_place = position%10;
int tens_place = position/10;
if((tens_place%2 == 0) && (units_place != 0))
{
x_pos = x+x_increment*(units_place-1);
y_pos = y+y_increment*tens_place;
}
else if((tens_place%2 != 0) && (units_place == 0))
{
x_pos = x+x_increment*9;
y_pos = y+y_increment*(tens_place-1);
}
else if(units_place != 0)
{
x_pos = x+x_increment*(10-(units_place));
y_pos = y+y_increment*tens_place;
}
else
{
x_pos = x;
y_pos = y+y_increment*(tens_place-1);
}
drawPawn(x_pos, y_pos);
}
void movePawn()
{
glutPostRedisplay();
if(diceVal>0)
{
if(turn)
{
player_2_position++;
diceVal--;
_sleep(1000);
}
else
{
player_1_position++;
diceVal--;
_sleep(1000);
}
}
else if(rolling == 0)
{
if(turn)
{
turn = 0;
if(snakes[player_2_position])
{
player_2_position = snakes[player_2_position];
strcpy(msg , "Player 2 gets swallowed by a Snake!!");
}
else if (ladders[player_2_position])
{
player_2_position = ladders[player_2_position];
strcpy(msg , "Player 2 takes a ladder!!");
}
}
else
{
turn = 1;
if(snakes[player_1_position])
{
player_1_position = snakes[player_1_position];
strcpy(msg , "Player 1 gets swallowed by a Snake!!");
}
else if (ladders[player_1_position])
{
player_1_position = ladders[player_1_position];
strcpy(msg , "Player 1 takes a ladder!!");
}
}
rolling = 1;
}
drawPlayer(0, player_1_position);
drawPlayer(1, player_2_position);
}
void getBoard()
{
drawGrid();
drawLadders();
drawSnakes();
if(((player_1_position+diceVal)>100) && turn == 0)
{
diceVal = 0;
}
else if (((player_2_position+diceVal)>100) && turn)
{
diceVal = 0;
}
else if(((player_1_position == 100)|(player_2_position == 100)) &&
!victory)
{
char *turnplusone;
char buffer[2];
turnplusone = (char*)malloc(100*sizeof(char));
turnplusone = itoa(turn+1,buffer,10);
strcpy(msg , "Player ");
strcat(msg , turnplusone);
strcat(msg , " Won!!");
rolling = 0;
victory = 1;
}
else
{
movePawn();
}
drawNumbers();
}
int rollDice()
{
srand(time(NULL));
return rand()%6 + 1;
}
void specifySnakesLadders()
{
snakes = new int[100];
ladders = new int[100];
for(int i=1;i<=100;i++){
snakes[i] = 0;
ladders[i] = 0;
}
ladders[1] = 38;
ladders[4] = 14;
ladders[9] = 31;
ladders[21] = 42;
ladders[28] = 84;
ladders[51] = 67;
ladders[72] = 91;
ladders[80] = 99;
snakes[17] = 7;
snakes[54] = 34;
snakes[62] = 19;
snakes[64] = 60;
snakes[87] = 36;
snakes[92] = 73;
snakes[95] = 75;
snakes[98] = 79;
}
void displayCallback()
{
glClear(GL_COLOR_BUFFER_BIT);
glLoadIdentity();
giveMessage(msg, strlen(msg) , turn+1);
getBoard();
glutSwapBuffers();
}
void reshapeCallback(int w, int h)
{
glViewport(0, 0, (GLfloat)(w), (GLfloat)(h+10));
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0.0, 10, 0.0, 10, 1, 1);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
}
void inputCallback(int key, int x, int y)
{
if(rolling)
{
if(victory)
exit(0);
if(key == GLUT_KEY_RIGHT)
{
diceVal = rollDice();
char *turnplusone;
char buffer1[2];
turnplusone = (char*)malloc(100*sizeof(char));
turnplusone = itoa(turn+1,buffer1,10);
char *dicevalue;
char buffer2[2];
dicevalue = (char*)malloc(100*sizeof(char));
dicevalue= itoa(diceVal,buffer2,10);
strcpy(msg,"Player ");
strcat(msg,turnplusone);
strcat(msg," has rolled ");
strcat(msg,dicevalue);
rolling = 0;
}
}
}
void timerCallback(int x)
{
glutPostRedisplay();
glutTimerFunc(10, timerCallback, 0);
}



void mydisplay(void)
{

	char *buffer = new char[50];

	int i=0;
	
	glClearColor(0.2, 0.1, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
	glColor3f(0, 0, 1);

	
	glRasterPos2f(380, 800);

	strcpy(buffer,"JAWAHARLAL NEHRU NATIONAL COLLEGE OF ENGINEERING ");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.6, 0, 1);
	glRasterPos2f(380, 750);
	strcpy(buffer,"DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING ");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 0, 0);
	glRasterPos2f(600, 650);
	strcpy(buffer,"A MINI PROJECT ON :");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.5);
	glRasterPos2f(600, 600);
	strcpy(buffer,"SNAKES AND LADDERS");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRasterPos2f(50, 400);
	strcpy(buffer,"BY: ");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 0);
	glRasterPos2f(50, 350);
	strcpy(buffer,"ABHAY.K.P (4JN16CS001)");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glRasterPos2f(50, 300);
	strcpy(buffer,"K SUHAS BHARADWAJ (4JN16CS103)");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glRasterPos2f(800, 400);
	strcpy(buffer,"GUIDED BY :");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.6, 0.7, 0.4);
	glRasterPos2f(800, 350);
	strcpy(buffer,"Mrs. SUSHMA R.B");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}	
	glRasterPos2f(1100, 350);
	strcpy(buffer,"B.E , MTECH");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glRasterPos2f(850, 330);
	strcpy(buffer,"ASST. PROFESSOR DEPT. OF CSE");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	
	glRasterPos2f(800, 290);
	strcpy(buffer,"Mrs. AYESHA SIDDIQA");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}	
	glRasterPos2f(1100, 290);
	strcpy(buffer,"B.E , MTECH");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glRasterPos2f(850, 270);
	strcpy(buffer,"ASST. PROFESSOR DEPT. OF CSE");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}

	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 0, 0);
	glRasterPos2f(400, 100);
	strcpy(buffer,"-----CLICK HERE TO CONTINUE-----");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();


	glFlush();
}

void init()
{
	glClearColor(0.8, 0.6, 0.25, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 1600.0, 0.0, 1100.0);
}


void Mouse(int btn, int state, int x, int y)

{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{


		glutHideWindow();
		glutSetWindow(menu);
		glutShowWindow();
	}

}

void dispmenu()
{

	char *buffer = new char[70];
	int i=0;

	glClearColor(0.1, 0, 0, 1);
	glutSetWindow(menu);
	glutShowWindow();

	glClear(GL_COLOR_BUFFER_BIT);



	glPushMatrix();
	glColor3f(0, 1, 1);
	glRasterPos2f(370, 800);
	strcpy(buffer,"INSTRUCTIONS");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.7, 0.6, 0.8);
	glRasterPos2f(50, 700);
	strcpy(buffer,"The purpose of the game is to throw a die and reach the final square before the other player");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	
	glRasterPos2f(50, 600);
	strcpy(buffer,"Player needs to press right arrow to roll the dice");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 1, 1);
	glRasterPos2f(300, 200);
	strcpy(buffer,"PRESS ENTER TO CONTINUE");
	for(i=0;i<strlen(buffer);i++){
	
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)buffer[i]);
	
	}
	glPopMatrix();
	glFlush();
}		



void menukey(unsigned char key, int x, int y)
{
	switch (key)
	{

	case 13:  
		glutHideWindow();
		glutSetWindow(game);
		glutShowWindow();
		break; 


	}
}

int main(int argc, char**argv)
{

	glutInit(&argc, argv);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(1024, 700);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	first = glutCreateWindow("FIRST");
	glutDisplayFunc(mydisplay);
	glutMouseFunc(Mouse);
	glutSetWindow(first);
	init();

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 700);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	menu = glutCreateWindow("MENU");
	glutDisplayFunc(dispmenu);
	glutKeyboardFunc(menukey);
	glutSetWindow(menu);
	glutHideWindow();
	init();

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(550, 600);
	game = glutCreateWindow("Snakes and Ladders");
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutSpecialFunc(inputCallback);
	glutTimerFunc(10, timerCallback, 0);
	glutHideWindow();
	glutSetWindow(game);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	specifySnakesLadders();
	glutMainLoop();
	return 0;
}