//Tiles form the backbone of the game. Could have subclassed from button, but doesn't for the sake of mouseOver, etc. 

#ifndef _TILE_H_
#define _TILE_H_

//includes
#include <iostream>
#include "include\GL\glut.h"

#define FULLRESOURCEVALUE 100

#define DIMENSION 30
#define SPACING 40
#define PADDING 25

class Tile 
{

private:
	//position of the tile in the grid (not actual pixel position) allows for easy revealing of neighbours.
	int x, y;
	//determines if the tile's value has been revealed to the user or not. 
	bool isMasked;
	//the resource value of this given tile. 
	float value;
	// array of neighbors not needed -> can check and reveal neighbours that way.
protected:
public:
	//adjusted X and adjusted Y values reflect the actual, pixel-based location of the tile, not just the location in the grid. 
	int adjX;
	int adjY; 

	//constructors
	Tile();
	Tile(int _x, int _y);
	//copy constructor.
	Tile(const Tile &_obj);
	//destructor.
	~Tile();

	//practical functions.
	void Render();

	//get sets.
	// function -> get/set position.
	void SetPosition(int _x, int _y);

	// function -> get/set value.
	float GetValue() const;
	void SetValue (float _val);
	bool GetMasked() const; 
	void SetMasked(bool _new);
};

#endif

//KW