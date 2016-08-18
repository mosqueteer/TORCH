#include <glut/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "datatypes.h"
#include "point.h"


//------------------------------------------------------------------------------------------------------

void drawSquare(long double cx, long double cy, long double length, Color color)
{
		
	glBegin(GL_QUADS);
	glColor3f(color&4,color&2,color&1);
	glVertex2f(cx-length, cy+length);
	glVertex2f(cx+length, cy+length);
	glVertex2f(cx+length, cy-length);
	glVertex2f(cx-length, cy-length);
	glEnd();		
	
}
//------------------------------------------------------------------------------------------------------

void drawIsolatedPoint(long double x, long double y, long double radius, Color color)
{
	const long double DEG2RAD = M_PI/180;
	long double X, Y;
	
	glColor3f(color&4,color&2,color&1);
	
	glBegin(GL_POLYGON);
	for (int i=0; i < 360; i+=15)
	{
		long double angle = i * DEG2RAD;
		
		X = x + radius * cosl(angle);
		Y = y + radius * sinl(angle);
		
		glVertex2d(X,Y);
	}
	glEnd();
	
}


//------------------------------------------------------------------------------------

void drawPoint (Point *P, Color color)
{
	
	glBegin(GL_POINTS); 
		glColor3f(color&4,color&2,color&1);
		glVertex2d((*P).x, (*P).y);  	
	glEnd(); 
	
	drawCircleForScanlineFilling(P, 0.02);
	
}

//------------------------------------------------------------------------------------

void drawPlusPoint (Point *P, Color color)
{
	
	glBegin(GL_LINES); 
	glColor3f(color&4,color&2,color&1);
	glVertex2f((*P).x-0.075, (*P).y);  	
	glVertex2f((*P).x+0.075, (*P).y);  	
	glVertex2f((*P).x, (*P).y-0.075);  	
	glVertex2f((*P).x, (*P).y+0.075);  	
	glEnd(); 
		
}


//------------------------------------------------------------------------------------

void drawMinusPoint (Point *P, Color color)
{
	
	glBegin(GL_LINES); 
	glColor3f(color&4,color&2,color&1);
	glVertex2f((*P).x-0.075, (*P).y);  	
	glVertex2f((*P).x+0.075, (*P).y);  	
	//	glVertex2d((*P).x, (*P).y-0.2);  	
	//	glVertex2d((*P).x, (*P).y+0.2);  	
	glEnd(); 
	
	//drawCircle(P, 0.02);
	
}


//------------------------------------------------------------------------------------------------------

void XYAxesDrawing()
{
	glBegin(GL_LINES);
	glVertex2f(Xmin,0.0f);
	glVertex2f(Xmax,0.0f);
	glEnd(); 
	
	glBegin(GL_LINES);
	glVertex2f(0.0f,Ymin);
	glVertex2f(0.0f,Ymax);
	glEnd();
	
}


//------------------------------------------------------------------------------------------------------
// end of file