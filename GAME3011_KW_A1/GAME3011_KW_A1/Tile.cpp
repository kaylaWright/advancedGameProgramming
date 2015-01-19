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

		glRectf(PADDING + x * SPACING, PADDING + y * SPACING, PADDING + x * SPACING + DIMENSION, PADDING + y * SPACING + DIMENSION);
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