#include "Button.h"

#pragma region cons/des

Button::Button(ButtonCallback _function, int _x, int _y, int _w, int _h) : onClickFunction(_function), x(_x), y(_y), w(_w), h(_h), isHoveredOver(false)
{

}

Button::~Button()
{
	onClickFunction = NULL;
}

#pragma endregion 

#pragma region button functions

void Button::Render()
{
	//change colour if the mouse is hovering over the button. 
	if (isHoveredOver)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	else
	{
		glColor3f(0.5f, 0.0f, 0.0f);
	}

	//draw the button. 
	glRectf(x, y, x + w, y + h);
}

#pragma region 

#pragma region get/set

//primarily for toggling hover effect of a given button.
bool Button::GetHover()
{
	return isHoveredOver;
}

//changing the hover effect of a given button. 
void Button::SetHover(bool _new)
{
	isHoveredOver = _new;
}

#pragma endregion

//KW