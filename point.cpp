/*
 *  point.cpp
 *  pin
 *
 *  Created by Abel Gomes on 1/12/13.
 *  Copyright 2013 Universidade da Beira Interior. All rights reserved.
 *
 */

#include "point.h"
#include <GLUT/glut.h>
#include "circle.h"


/*
 //------------------------------------------------------------------------------------------------------
 DESCRIPTION:
 Construtor - initializes instance variables. 
 //------------------------------------------------------------------------------------------------------
 */

Point::Point()
{
	x = 0.0;
	y = 0.0;
//	CONVEXHULLFLAG = false;
//	side = -1;
}


Point::~Point()
{
}



//------------------------------------------------------------------------------------------------------

void Point::drawSquare(float length, Color color)
{
	
	glBegin(GL_QUADS);
	glColor3f(color&4,color&2,color&1);
	glVertex2f(x-length/2, y+length/2);
	glVertex2f(x+length/2, y+length/2);
	glVertex2f(x+length/2, y-length/2);
	glVertex2f(x-length/2, y-length/2);
	glEnd();		
	
}
//------------------------------------------------------------------------------------------------------

void Point::draw(float r, Color color)
{
	Circle c(x,y,r);
	c.draw(color);
}


//------------------------------------------------------------------------------------

void Point::drawPlus (float r, Color color)
{
	
	glBegin(GL_LINES); 
	glColor3f(color&4,color&2,color&1);
	glVertex2f(x-r/2, y);  	
	glVertex2f(x+r/2, y);  	
	glVertex2f(x, y-r/2);  	
	glVertex2f(x, y+r/2);  	
	glEnd(); 
	
}



//------------------------------------------------------------------------------------------------------
// end of file
