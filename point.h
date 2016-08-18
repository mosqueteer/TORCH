/*
 *  point.h
 *  pin
 *
 *  Created by Abel Gomes on 1/12/13.
 *  Copyright 2013 Universidade da Beira Interior. All rights reserved.
 *
 */


#ifndef _Point
#define _Point

#include "datatypes.h"

class Point{
public:
	float	x;
	float	y;
//	bool	CONVEXHULLFLAG;
//	int		idx;			// its index in vector
//	int		side;			// none = -1, bottom=0, right=1, top=2, left=3
	
	Point();
	virtual ~Point();
	
	void drawSquare(float, Color);
	void draw(float, Color);
	void drawPlus(float, Color);

};

#endif
