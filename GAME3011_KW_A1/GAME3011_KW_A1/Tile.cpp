#include "Tile.h"

//Constructors/Destructors
#pragma region Cons/Des

Tile::Tile() : value(0), x(0), y(0), isMasked(true)
	{
		adjX = PADDING + x * SPACING;
		adjY = PADDING + y * SPACING;
	}

	Tile::Tile(int _x, int _y) : value(0), x(_x), y(_y), isMasked(true)
	{
		adjX = PADDING + x * SPACING;
		adjY = PADDING + y * SPACING;
	}

	Tile::Tile(const Tile &_obj)
	{
		value = _obj.GetValue();
		x = _obj.x;
		y = _obj.y; 
		isMasked = _obj.GetMasked();

		adjX = PADDING + x * SPACING;
		adjY = PADDING + y * SPACING;
	}

	Tile::~Tile()
	{

	}

#pragma endregion

#pragma region tile functions.

	void Tile::Excavate()
	{

	}

	void Tile::Render()
	{
		switch(static_cast<int>(value))
		{
			//i like goooold 218,165,32
		case FULLRESOURCEVALUE:
			glColor3f(0.85f, 0.65f, 0.13f);
			break;
		case (FULLRESOURCEVALUE/2):
			glColor3f(0.5f, 0.5f, 0.0f);
			break;
		case (FULLRESOURCEVALUE/4):
			glColor3f(0.4f, 0.6f, 0.4f);
			break;
		default: 
			glColor3f(0.4f, 0.4f, 0.4f);
			break;
		}

		if(isMasked)
		{
			glColor3f(0.2f, 0.2f, 0.2f);
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

	bool Tile::GetMasked() const 
	{
		return isMasked;
	}

	void Tile::SetMasked(bool _new)
	{
		isMasked = _new;
	}

#pragma endregion 

//KW