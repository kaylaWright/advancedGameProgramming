//Tiles form the backbone of the game. 

#ifndef _TILE_H_
#define _TILE_H_

//includes
#include <iostream>
#include "include\GL\glut.h"

#define FULLRESOURCEVALUE 100

#define PADDING 1

class Tile 
{

private:
	//the resource value of this given tile. 
	float value;
	// array of neighbors not needed -> can check and reveal neighbours that way.
protected:
public:
	//constructors
	Tile();
	Tile(int _x, int _y);
	//copy constructor.
	Tile(const Tile &_obj);
	//destructor.
	~Tile();

	//position of the tile in the grid (not actual pixel position) allows for easy revealing of neighbours.
	int x, y;

	//practical functions.
	void Excavate();
	void Reveal();
	void Render();

	//get sets.
	// function -> get/set position.
	void SetPosition(int _x, int _y);

	// function -> get/set value.
	float GetValue() const;
	void SetValue (float _val);
};

#endif

//KW