// Advanced Game Programming -- Assignment One  
// Kayla Wright, 100853637
// Project Span: 2015.01.12 - 2015.02.01 (~three weeks)

//standard includes.
#include <iostream>
#include <time.h>
#include <map>
#include <string>

//openGL includes.
#include "include\GL\glut.h"

//game-specific includes
#include "Tile.h"
#include "Button.h"
#include "AudioManager.h"

//typedefs. 
typedef std::map<std::string, Button*>::iterator buttonIT;

//const definitions.
//screen size dimensions. 
#define sWIDTH 950
#define sHEIGHT 750
//dimensions of the grid desired.
#define GRIDSIZE 16
#define STARTINGSCANS 6
#define STARTINGEXTRACTIONS 3

//function prototypes.
void Setup();
void InitGL(int _argc, char* _argv);
void Shutdown();

//openGL functions.
//** rendering functions.
void Render();
void Resize(int _w, int _h);
void WriteBitmapString(char *string);

void DrawUI();
void DrawGameObjects();
void DrawEndGame();

//** input functions.
void InputMouse(int _button, int _state, int _x, int _y);
void MoveMouse(int _x, int _y);
void InputKey(unsigned char _key, int _x, int _y);

//** game functions.
void RestartGame();
void SetupBoard();
void ToggleGameMode();
void ToggleAudio();

bool AddResource(int _x, int _y, int _val);
bool CheckResourceAllocation(int _x, int _y);

bool Scan(int _x, int _y);
void Extract(int _x, int _y);

bool CheckTileCollision(int _x, int _y, int _tX, int _tY);
bool CheckButtonCollision(Button* _btn, int _x, int _y);
bool ToggleButtonHover(Button* _btn, int _x, int _y);

//game vars
bool isScan;
int resourceScore; 
int scansRemaining;
int extractionsRemaining; 
Tile grid[GRIDSIZE][GRIDSIZE];
Tile uiTiles[4];
std::map<std::string, Button*> buttonMAP;

//audio 
AudioManager* audioManager;

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

//provides initial, one-time set-up for the game (constsant variable instantiation, etc.)
//for individual game processes, see: RestartGame and SetupBoard.
void Setup()
{
	//open gl set up. 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//audio set up.
	audioManager = new AudioManager();
	audioManager->Init();
	//song from: http://incompetech.com/music/royalty-free/index.html?feels%5B%5D=Calming&feels%5B%5D=Dark
	audioManager->CreateAudioStream("shoegazer.mp3");
	//SFX from freesounds.org. 
	audioManager->CreateSound("sonar.wav");
	audioManager->CreateSound("gravel.wav");

	//game set up. 
	for(int i = 0; i < GRIDSIZE; i++)
	{
		for(int j = 0; j < GRIDSIZE; j++)
		{
			grid[i][j] = Tile(i, j);
		}
	}

	int resource = FULLRESOURCEVALUE * 2;
	for (int i = 0; i < 4; i++)
	{
		uiTiles[i] = Tile(22, i + 2);
		uiTiles[i].SetValue(resource /= 2);
		uiTiles[i].SetMasked(false);
	}
	
	//creating and mapping any game buttons. 
	buttonMAP["mode"] = new Button(&ToggleGameMode, sWIDTH - 265, sHEIGHT - 400, 250, 50);
	buttonMAP["quit"] = new Button(&Shutdown, sWIDTH - 115, sHEIGHT - 60, 100, 50);
	buttonMAP["restart"] = new Button(&RestartGame, sWIDTH - 220, sHEIGHT - 60, 100, 50);
	buttonMAP["mute"] = new Button(&ToggleAudio, sWIDTH - 115, 15, 100, 50);
}

//render function; draws the game objects to scene. 
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//draw tiles and any other game objects. 
	DrawGameObjects();
	//draw UI (buttons, feedback, etc).
	DrawUI();

	if (extractionsRemaining <= 0)
	{
		DrawEndGame();
	}

	//swap out the active buffer. 
	glutSwapBuffers();
}

//draws the user interface elements for Minescanner; specifically, buttons and any rasterized text. 
void DrawUI()
{
	//renders all buttons. 
	for (buttonIT it = buttonMAP.begin(); it != buttonMAP.end(); ++it)
	{
		//could be more efficient. 
		if (it->first == "quitEND" || it->first == "restartEND")
		{
			continue;
		}

		it->second->Render();
	}

	//renders all text. 
	char temp[56];
	glColor3f(1.0, 0, 0);

	//display the current score. 
	glRasterPos3f(25, sHEIGHT - 25, 0.0f);

	strcpy_s(temp, "Collected Resources: ");
	std::string s = std::to_string(resourceScore);
	strcat_s(temp, s.c_str());
	WriteBitmapString(temp);

	//display remaining scans. 
	glRasterPos3f(25, sHEIGHT - 50, 0.0f);

	strcpy_s(temp, "Scans remaining: ");
	s = std::to_string(scansRemaining);
	strcat_s(temp, s.c_str());
	WriteBitmapString(temp);

	//display remaining extractions
	glRasterPos3f(25, sHEIGHT - 75, 0.0f);

	strcpy_s(temp, "Extractions remaining: ");
	s = std::to_string(extractionsRemaining);
	strcat_s(temp, s.c_str());
	WriteBitmapString(temp);

	//instructions.
	glColor3f(0.85f, 0.55f, 0.15f);

	glRasterPos3f(sWIDTH - 280, sHEIGHT - 285, 0.0f);
	WriteBitmapString("SCAN mode surveys resources.");
	glRasterPos3f(sWIDTH - 285, sHEIGHT - 310, 0.0f);
	WriteBitmapString("EXTRACT mode mines resources.");
	glRasterPos3f(sWIDTH - 280, sHEIGHT - 335, 0.0f);
	WriteBitmapString("Better resources? Better score.");

	glRasterPos3f(sWIDTH - 245, sHEIGHT - 425, 0.0f);
	WriteBitmapString("Click to toggle between");
	glRasterPos3f(sWIDTH - 235, sHEIGHT - 450, 0.0f);
	WriteBitmapString("SCAN and EXTRACT.");

	//resource values. 
	glRasterPos3f(sWIDTH - 287, sHEIGHT - 515, 0.0f);
	WriteBitmapString("No resources to extract: 0pts");
	glRasterPos3f(sWIDTH - 250, sHEIGHT - 555, 0.0f);
	WriteBitmapString("Minimal resources: 25pts");
	glRasterPos3f(sWIDTH - 260, sHEIGHT - 595, 0.0f);
	WriteBitmapString("Half resource value: 50pts");
	glRasterPos3f(sWIDTH - 285, sHEIGHT - 635, 0.0f);
	WriteBitmapString("Max resource power! 100pts");
	
	//button text. 
	glColor3f(1.0f, 1.0f, 1.0f);

	//display the current game mode.
	strcpy_s(temp, "Current mode: ");
	if (isScan)
	{
		strcat_s(temp, "SCAN");
		glRasterPos3f(sWIDTH - 235, sHEIGHT - 380, 0.0f);
	}
	else
	{
		strcat_s(temp, "EXTRACT");
		glRasterPos3f(sWIDTH - 245, sHEIGHT - 380, 0.0f);
	}
	WriteBitmapString(temp);

	//quit button. 
	glRasterPos3f(sWIDTH - 80, sHEIGHT - 40, 0.0f);
	WriteBitmapString("Quit");
	//restart button.
	glRasterPos3f(sWIDTH - 200, sHEIGHT - 40, 0.0f);
	WriteBitmapString("Restart");
	//mute button
	glRasterPos3f(sWIDTH - 85, 35, 0.0f);
	WriteBitmapString("Mute");
}

//renders all of the game objects in the render function.
void DrawGameObjects()
{
	for (int i = 0; i < GRIDSIZE; i++)
	{
		for (int j = 0; j < GRIDSIZE; j++)
		{
			grid[i][j].Render();
		}
	}

	for (int i = 0; i < 4; i++)
	{
		uiTiles[i].Render();
	}
}

//renders all end-game objects (namely, a listing of the score and another resetart/quit button)
void DrawEndGame()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glRectf(50.0f, 50.0f, sWIDTH - 50.0f, sHEIGHT - 50.0f);
	
	glColor3f(0.85f, 0.55f, 0.15f);
	glRasterPos3f(sWIDTH  / 2 - 200, sHEIGHT / 2, 0.0f);
	WriteBitmapString("Thus ends the excavation. You scored:");

	char temp[56];
	glColor3f(1.0, 0, 0);

	//display the current score. 
	glRasterPos3f(sWIDTH / 2 - 50, sHEIGHT / 2 - 25, 0.0f);

	std::string s = std::to_string(resourceScore);
	strcpy_s(temp, s.c_str());
	strcat_s(temp, " points.");
	WriteBitmapString(temp);

	glColor3f(0.85f, 0.55f, 0.15f);
	glRasterPos3f(sWIDTH / 2 - 350, sHEIGHT / 2 - 50, 0.0f);
	WriteBitmapString("And it was only at the cost of thousands indiginous life forms. Not bad.");

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos3f(sWIDTH / 2 - 200, sHEIGHT / 2 - 75, 0.0f);
	WriteBitmapString("Click here to restart.               Click here to quit.");

	buttonMAP["quitEND"] = new Button(&Shutdown, sWIDTH / 2 + 60, sHEIGHT / 2 - 150, 100, 50);
	buttonMAP["quitEND"]->Render();
	buttonMAP["restartEND"] = new Button(&RestartGame, sWIDTH / 2 - 160, sHEIGHT / 2 - 150, 100, 50);
	buttonMAP["restartEND"]->Render();

	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos3f(sWIDTH / 2 - 145, sHEIGHT / 2 - 125, 0.0f);
	WriteBitmapString("Restart");

	glRasterPos3f(sWIDTH / 2 + 90, sHEIGHT / 2 - 125, 0.0f);
	WriteBitmapString("Quit");
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

//draws a bitmap character string. 
/* PARAMS:
	*string - a pointer to the string that is to be rendered. 
*/
void WriteBitmapString(char* _string)
{
	char *c;
	for (c = _string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
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

	//checks to see if a UI element has been selected, responds according to button's specific callback function. 
	for (buttonIT it = buttonMAP.begin(); it != buttonMAP.end(); ++it)
	{
		if (_state == 1 && CheckButtonCollision(it->second, _x, mouseY))
		{
			it->second->onClickFunction();
		}
	}
	
	//checks to see if a tile has been clicked, but only on the release of the click. 
	if (_state == GLUT_UP)
	{
		for (int i = 0; i < GRIDSIZE; i++)
		{
			for (int j = 0; j < GRIDSIZE; j++)
			{
				if (CheckTileCollision(_x, mouseY, i, j))
				{
					//react differently based off of the game mode. 
					if (grid[i][j].GetMasked() && isScan && scansRemaining > 0)
					{
						//unmask tile and adjacent tiles.
						Scan(i, j);
						//decrement the number of surveys/scans one can do. 
						scansRemaining--;

						
						audioManager->PlaySFX("gravel.wav");
					}
					
					if (!isScan && extractionsRemaining > 0)
					{
						Extract(i, j);
						extractionsRemaining--; 

						audioManager->PlaySFX("sonar.wav");
					}

					//redraw the scene. 
					glutPostRedisplay();
				}
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
	bool isChanged = false;
	//convert the mouse coordinates to openGL coordinates. 
	int mouseY = sHEIGHT - _y;

	//see if the mode button is overlapped. 
	for (buttonIT it = buttonMAP.begin(); it != buttonMAP.end(); ++it)
	{
		if (ToggleButtonHover(it->second, _x, mouseY))
		{
			isChanged = true;
		}
	}

	//only redisplay if one of the buttons is changed because redisplaying is costly and the passive mouse function always triggers. 
	if (isChanged)
	{
		glutPostRedisplay();
	}
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
	resourceScore = 0;

	//resets game variables. 
	isScan = true;
	scansRemaining = STARTINGSCANS;
	extractionsRemaining = STARTINGEXTRACTIONS;

	//restarts the board. 
	SetupBoard();

	//redisplay for UI elements. 
	glutPostRedisplay();
}

//initializes board for game play by allocating out values to tiles as needed. 
void SetupBoard()
{
	//starts by setting all tile resource values to zero and remasking them. 
	for(int i = 0; i < GRIDSIZE; i++)
	{
		for(int j = 0; j < GRIDSIZE; j++)
		{
			grid[i][j].SetValue(0);
			grid[i][j].SetMasked(true);
		}
	}

	int x = rand() % GRIDSIZE;
	int y = rand() % GRIDSIZE;
	for(int k = 0; k < 5; k++)
	{
		//random generation of maximum resource value. 
		//safeguard against accidentally adding two max value resources to the same or even halved value tiles. Only a small chance, but could still happen.
		while(!CheckResourceAllocation(x, y))
		{
			x = rand() % GRIDSIZE;
			y = rand() % GRIDSIZE;
		}
		
		//adding value to the resources. 
		AddResource(x, y, FULLRESOURCEVALUE);
	}
}

//toggles the game mode between scan and extract
//NOTE: modeButton callback function.
void ToggleGameMode()
{
	isScan = !isScan;
	//redisplay for feedback's sake. 
	glutPostRedisplay();
}

//toggles muting of sounds. 
void ToggleAudio()
{
	audioManager->StopAllSounds();
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

	//{ { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 } };
	//adding the resource to adjacent tiles
	for (int xVal = -1; xVal <= 1; xVal++)
	{
		for (int yVal = -1; yVal <= 1; yVal++)
		{
			int tempX = _x + xVal;
			int tempY = _y + yVal;

			AddResource(tempX, tempY, _val/2);
		}
	}

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
	//an array containing the direction for all adjacent tiles defined above for reuse. 
	//{ { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 } };

	for (int xVal = -1; xVal <= 1; xVal++)
	{
		for (int yVal = -1; yVal <= 1; yVal++)
		{
			int tempX = _x + xVal;
			int tempY = _y + yVal;

			//if this pairing goes outside the span of the grid in either direction, don't proceed.
			if (tempX < 0 || tempX > 15 || tempY < 0 || tempY > 15)
			{
				continue;
			}

			//anything that gets here is an adjacent tile. Reveal it!
			grid[tempX][tempY].SetMasked(false);
		}
	}
	
	return true;
}

// reveals a tile and all it's adjacent neighbours; called in scan mode. 
/* PARAMS:
_x, _y - position of desired tile.
*/
void Extract(int _x, int _y)
{
	resourceScore += grid[_x][_y].GetValue();
	grid[_x][_y].SetValue(0);

	for (int xVal = -2; xVal <= 2; xVal++)
	{
		for (int yVal = -2; yVal <= 2; yVal++)
		{
			int tempX = _x + xVal;
			int tempY = _y + yVal;

			//if this pairing goes outside the span of the grid in either direction, don't proceed.
			if (tempX < 0 || tempX > 15 || tempY < 0 || tempY > 15)
			{
				continue;
			}

			//reveal the two rings around the center of the extractiong point for sure. 
			grid[tempX][tempY].SetMasked(false);

			//if the tile isn't worth a quarter of the max resource value, move along. 
			if (grid[tempX][tempY].GetValue() < FULLRESOURCEVALUE / 4)
			{
				continue;
			}

			//only allocate score now that you know the value is at least 25. 
			resourceScore += grid[tempX][tempY].GetValue();
			grid[tempX][tempY].SetValue(grid[tempX][tempY].GetValue() / 2);
		}
	}
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

// calculates whether the mouse is colliding with a given button. 
/* PARAMS:
	_btn - the button in question. 
	_x, _y - position of click.
*/
bool CheckButtonCollision(Button* _btn, int _x, int _y)
{
	if (_x > _btn->x && _x < _btn->x + _btn->w)
	{
		if (_y > _btn->y && _y < _btn->y + _btn->h)
		{
			return true;
		}
	}

	return false;
}

// toggles the hover state of the button. 
/* PARAMS:
	_btn - the button in question.
	_x, _y - position of the mouse. 
*/
bool ToggleButtonHover(Button* _btn, int _x, int _y)
{
	if (CheckButtonCollision(_btn, _x, _y))
	{
		if (_btn->GetHover() == false)
		{
			_btn->SetHover(true);
			return true;
		}
	}
	else
	{
		if (_btn->GetHover() == true)
		{
			_btn->SetHover(false);
			return true;
		}
	}
		
	return false;
}

#pragma endregion 

#pragma region shutdown

//END PROG
void Shutdown()
{
	audioManager->Shutdown();
	glutDestroyWindow(1);
	exit(0);
}

#pragma endregion

//KW 