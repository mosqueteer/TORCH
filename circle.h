/*
 *  circle.h
 *  pin
 *
 *  Created by Abel Gomes on 1/17/13.
 *  Copyright 2013 Universidade da Beira Interior. All rights reserved.
 *
 */


#ifndef _Circle
#define _Circle

#include "datatypes.h"


class Circle{
public:
	float	X;			// x-center
	float	Y;			// y-center
	float	R;			// radius
	Circle();
	Circle(float, float,float);
	virtual ~Circle();
	
	void draw(Color);
	void drawBoundary(Color);
};

#endif
