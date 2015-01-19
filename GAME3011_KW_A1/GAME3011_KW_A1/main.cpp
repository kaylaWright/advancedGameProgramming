// Advanced Game Programming -- Assignment One  
// Kayla Wright, 100853637
// Project Span: 2015.01.12 - 2015.02.01 (~three weeks)

//standard includes.
#include <iostream>

//openGL includes.
#include "include\GL\glut.h"

//game-specific includes
#include "Tile.h"

//const definitions.
#define sWIDTH 750
#define sHEIGHT 750

//function prototypes.
void Setup();
void InitGL(int _argc, char* _argv);
void Shutdown();

//openGL functions.
//** rendering functions.
void Render();
void Resize(int _w, int _h);

void DrawUI();
void DrawTiles();
void DrawCursor();

//** input functions.
void InputMouse(int _button, int _state, int _x, int _y);
void MoveMouse(int _x, int _y);
void InputKey(unsigned char _key, int _x, int _y);

//** other functions.
void MainLoop();

//global variables.
bool isQuit;

//TEST.
Tile grid[16][16];

//simple game state manager; anything more complex is unnecessary in the scope of this game. 
enum GameState { START_GAME, SCAN, EXCAVATE, END_GAME };
GameState currentState;

//PROGRAM START. 
int main(int _argc, char* _argv)
{
	InitGL(_argc, _argv);

	return 0;
}

#pragma region init

void InitGL(int _argc, char* _argv)
{
	//initializing openGL.
	glutInit(&_argc, &_argv);
	glutInitDisplayMode(GLUT_DOUBLE);

	//window set-up.
	glutInitWindowSize(sWIDTH, sHEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Minescanner");

	//resizing window.
	glutReshapeFunc(Resize);

	//render function
	glutDisplayFunc(Render);

	//input 
	glutMouseFunc(InputMouse);
	glutPassiveMotionFunc(MoveMouse);
	glutKeyboardFunc(InputKey);

	Setup();
	Resize(sWIDTH, sHEIGHT);
	
	//main loop: GO. 
	glutMainLoop();
}

#pragma endregion

#pragma region rendering/window

void Setup()
{
	//open gl set up. 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//game set up. 
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			grid[i][j] = Tile(i, j);
		}
	}
}

//render function; draws the game objects to scene. 
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//draw UI. 

	//draw tiles.
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			grid[i][j].Render();
		}
	}

	//draw cursor. 

	glutSwapBuffers();
}

//reshape function allows users to adjust size of window, without affecting view necessarily. 
/* PARAMS:
	_w, _h - dimensions of the newly changed window. 
*/
void Resize(int _w, int _h)
{
	glMatrixMode(GL_PROJECTION);              // setup viewing projection 
	glLoadIdentity();  
	glOrtho(0, sWIDTH, 0, sHEIGHT, 0, 100);

	glutPostRedisplay();
}

#pragma endregion 

#pragma region input handling

//handles mouse input from users. 
/* PARAMS:
	_button - which button is pressed.
	_state - which state it is in (down, up, held).
	_x, _y - position when clicked.
*/
void InputMouse(int _button, int _state, int _x, int _y)
{
	std::cout << _x << " " << _y << std::endl;
}

//handles mouse motion, allows for cursor following. 
/* PARAMS:
	_x, _y - position of mouse currently. 
*/
void MoveMouse(int _x, int _y)
{

}

//handles keyboard input from users.
/* PARAMS:
	_key - which button is pressed.
	_x, _y - position when clicked.
*/
void InputKey(unsigned char _key, int _x, int _y)
{
	std::cout << "Key."; 

	switch(_key) 
	{
	//quit game 
	case 27:
		Shutdown();
		break;
	default:
		break;
	}

}

#pragma endregion

#pragma region shutdown

void Shutdown()
{
	glutDestroyWindow(1);
	exit(0);
}

#pragma endregion

//KW 