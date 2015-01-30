//A standard button class. Responds when hovered, clicked, etc. 
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <iostream>
#include "include\GL\glut.h"

//decided to work with this as a callback function so the function could be external to the button class, as needed. 
typedef void (*ButtonCallback)();

class Button
{
private:
	//allows for highlighting when the button is hovered over. 
	bool isHoveredOver;
	
protected:
public:
	//button dimensions.
	int x, y, w, h;

	//function callback.
	ButtonCallback onClickFunction;

	//constructor.
	Button(ButtonCallback _function, int _x, int _y, int _w, int _h);
	//destructor.
	~Button();

	//render 
	void Render();

	//get sets.
	bool GetHover();
	void SetHover(bool _new);
};

#endif

//KW 