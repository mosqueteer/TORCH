/*
 *  circle.cpp
 *  pin
 *
 *  Created by Abel Gomes on 1/17/13.
 *  Copyright 2013 Universidade da Beira Interior. All rights reserved.
 *
 */

#include "circle.h"
#include <math.h>
#include <GLUT/glut.h>


/*
 //------------------------------------------------------------------------------------------------------
 DESCRIPTION:
 Construtor - initializes instance variables. 
 //------------------------------------------------------------------------------------------------------
 */

Circle::Circle()
{
	X = 0.0;
	Y = 0.0;
	R = 0.0;
}

Circle::Circle(float x, float y, float r)
{
	X = x;
	Y = y;
	R = r;
}

//------------------------------------------------------------------------------------------------------

Circle::~Circle()
{
}




//------------------------------------------------------------------------------------------------------

void Circle::draw(Color color)
{
	const float DEG2RAD = M_PI/180;
	float angle;
	float x, y;
	
	glBegin(GL_POLYGON);
	glColor3f(color&4,color&2,color&1);
	for (int i=0; i < 360; i+=10)
	{
		angle = i * DEG2RAD;
		
		x = X + R * cosf(angle);
		y = Y + R * sinf(angle);
		
		glVertex2f(x,y);
	}
	glEnd();
	
}

//------------------------------------------------------------------------------------------------------

void Circle::drawBoundary(Color color)
{
	const float DEG2RAD = M_PI/180;
	float x, y;
	float angle;

	glBegin(GL_LINES);
	glColor3f(color&4,color&2,color&1);
	for (int i=0; i < 360; i+=10)
	{
		angle = i * DEG2RAD;
		
		x = X + R * cosf(angle);
		y = Y + R * sinf(angle);
		
		glVertex2f(x,y);
	}
	glEnd();
	
}

//------------------------------------------------------------------------------------------------------
// end of file