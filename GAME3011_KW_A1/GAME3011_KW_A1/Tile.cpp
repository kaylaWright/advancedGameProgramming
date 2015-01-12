#include "Tile.h"

//Constructors/Destructors
#pragma region Cons/Des

	Tile::Tile()
	{
		value = 0;
		x = 0;
		y = 0;
	}

	Tile::Tile(int _x, int _y)
	{
		value = 0;
		x = _x;
		y = _y;
	}

	Tile::Tile(const Tile &_obj)
	{
		value = _obj.GetValue();
		x = _obj.x;
		y = _obj.y; 
	}

	Tile::~Tile()
	{

	}

#pragma endregion

#pragma region tile functions.

	void Tile::Excavate()
	{

	}

	void Tile::Reveal()
	{

	}

	void Tile::Render()
	{
	
		switch(static_cast<int>(value))
		{
		case FULLRESOURCEVALUE:
			glColor3f(0.0f, 1.0f, 0.0f);
			break;
		case (FULLRESOURCEVALUE/2):
			glColor3f(0.2f, 0.8f, 0.2f);
			break;
		case (FULLRESOURCEVALUE/4):
			glColor3f(0.4f, 0.6f, 0.4f);
			break;
		default: 
			glColor3f(0.4f, 0.4f, 0.4f);
			break;
		}

		glBegin(GL_QUADS);
			glVertex3f(20 * x + PADDING, 20 * y + PADDING, 0.0f); // The bottom left corner  
			glVertex3f(20 * x + PADDING, 40 * y - PADDING, 0.0f); // The top left corner  
			glVertex3f(40 * x - PADDING, 40 * y - PADDING, 0.0f); // The top right corner  
			glVertex3f(40 * x - PADDING, 20 * y + PADDING, 0.0f); // The bottom right corner 
		glEnd();
	}


#pragma endregion 

//Get/Sets
#pragma region Get/Sets

	float Tile::GetValue() const
	{
		return value;
	}

	void Tile::SetValue(float _val)
	{
		value = _val;
	}

#pragma endregion 

//KW