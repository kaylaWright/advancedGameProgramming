// Advanced Game Programming -- Assignment One  
// Kayla Wright, 100853637
// Project Span: 2015.01.12 - 2015.02.01 (~three weeks)

//standard includes.
#include <iostream>
#include <time.h>

//openGL includes.
#include "include\GL\glut.h"

//game-specific includes
#include "Tile.h"

//const definitions.
#define sWIDTH 950
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

//** game functions.
void RestartGame();
void SetupBoard();

bool AddResource(int _x, int _y, int _val);
bool CheckResourceAllocation(int _x, int _y);

bool Scan(int _x, int _y);

bool CheckTileCollision(int _x, int _y, int _tX, int _tY);

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
	srand(time(NULL));
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
	RestartGame();
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
	//converts mouse y to bottom left registration point (matching openGL method of drawing for convenience).
	int mouseY = sHEIGHT - _y;

	//checks to see if a UI element has been selected, responds appropriately. 
	
	//checks to see if a tile has been clicked. 
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			if(CheckTileCollision(_x, mouseY, i, j))
			{
				//unmask tile and adjacent tiles. 
				
				Scan(i, j);

				glutPostRedisplay();

				//decrement the number of surveys one can do. 

			}
		}
	}
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

#pragma region game funcs

//provides first time start up and subsequent restarting of game. 
void RestartGame()
{
	//erases the score. 
	
	//restarts the board. 
	SetupBoard();
}

//initializes board for game play by allocating out values to tiles as needed. 
void SetupBoard()
{
	//starts by setting all tile resource values to zero and remasking them. 
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			grid[i][j].SetValue(0);
			grid[i][j].SetMasked(true);
		}
	}

	int x = rand() % 16;
	int y = rand() % 16;
	for(int k = 0; k < 3; k++)
	{
		//random generation of maximum resource value. 
		//safeguard against accidentally adding two max value resources to the same or even halved value tiles. Only a small chance, but could still happen.
		while(!CheckResourceAllocation(x, y))
		{
			x = rand() % 16;
			y = rand() % 16;
		}
		
		//adding value to the resources. 
		AddResource(x, y, FULLRESOURCEVALUE);
	}
}

//recursish function adding value to resources from randomly generated start point. 
/* PARAMS:
	_x, _y - position of tile.
	_val - value of resource the tile will receive. 
*/
bool AddResource(int _x, int _y, int _val)
{
	//return false if the tiles would go out of bounds.
	if (_x < 0 || _x > 15 || _y < 0 || _y > 15)
	{
		return false;
	}

	//return false if we have reached the end of our valuable ore spots. 
	if(_val < FULLRESOURCEVALUE / 4)
	{
		return false; 
	}

	//adding resource to 'seed' tile.
	if(grid[_x][_y].GetValue() < _val)
	{
		grid[_x][_y].SetValue(_val);
	}

	//adding the resource to adjacent tiles
	//upper level.
	AddResource(_x - 1, _y - 1, _val / 2);
	AddResource(_x, _y - 1, _val / 2);
	AddResource(_x + 1, _y - 1, _val / 2);
	//mid level.
	AddResource(_x - 1, _y, _val / 2);
	AddResource(_x + 1, _y, _val / 2);
	//bottom level
	AddResource(_x - 1, _y + 1, _val / 2);
	AddResource(_x, _y + 1, _val / 2);
	AddResource(_x + 1, _y + 1, _val / 2);

	//ta-da! recursion. sort of. 
	return true; 
}
// KW -- NOTES -- This is the first time I've bothered to use even a semi-recursive function of any sort since learning to program. 

//checks to see if resources can be allocated to one of the randomly generated areas on the grid; prevents it if they are too similar to another generated seed tile. 
/* PARAMS:
	_x, _y - position of desired tile.
*/
bool CheckResourceAllocation(int _x, int _y)
{
	//if it's out of bounds, it's fine to place resources in theory; this will NEVER trigger with the primary resource however, which must be between 0-15 by virtue of how rand() works. 
	if(_x < 0 || _x > 15 || _y < 0 || _y > 15)
	{
		return true;
	}

	//if the value of the tile; if it's too high, return false. 
	if(grid[_x][_y].GetValue() >= FULLRESOURCEVALUE / 2)
		return false;
	else
		return true;
}

// reveals a tile and all it's adjacent neighbours; called in scan mode. 
/* PARAMS:
	_x, _y - position of desired tile.
*/
bool Scan(int _x, int _y)
{


	return true;
}

// calculates whether a tile has been clicked. Could be more generalized, but tiles have a particular offset that requires consideration. 
/* PARAMS:
	_x, _y - position of click.
	_tX, _tY - tile value without offset inclusion.
*/
bool CheckTileCollision(int _x, int _y, int _tX, int _tY)
{
	int tileX = grid[_tX][_tY].adjX;
	int tileY = grid[_tX][_tY].adjY;

	if(_x > tileX && _x < tileX + DIMENSION)
	{
		if(_y > tileY && _y < tileY + DIMENSION)
		{
			return true;
		}
	}
	return false;
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