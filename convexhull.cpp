/*
 *  convexhull.cpp
 *  TORCH Project (Total Order-Based Convex Hull)
 *
 *  Created by Abel Gomes on 1/12/13.
 *  Copyright 2013 Universidade da Beira Interior. All rights reserved.
 *
 */

#include <math.h>
#include "point.h"
#include "circle.h"
#include "convexhull.h"
#include "glut/glut.h"


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stack>


    // for getcwd function
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

extern bool SORTFLAG;

stack<Point> S;

//------------------------------------------------------------------------------------------------------

void drawCircle(float cx, float cy, float radius, Color color)
{
	const float DEG2RAD = M_PI/180;
	float x, y;
	
	
	glBegin(GL_POLYGON);
	glColor3f(color&4,color&2,color&1);

	for (int i=0; i < 360; i+=10)
	{
		float angle = i * DEG2RAD;
		
		x = cx + radius * cosf(angle);
		y = cy + radius * sinf(angle);
		
		glVertex2f(x,y);
	}
	glEnd();
	
}

/*
 ----------------------------------------------------------------------------
 DESCRIPTION:
 Construtor - initializes instance variables. 
 ----------------------------------------------------------------------------
 */

ConvexHull::ConvexHull()
{
	vop = new vector<Point>;
	
	SE = new vector<int>;
	SW = new vector<int>;
	NE = new vector<int>;
	NW = new vector<int>;
	cycle = new vector<Point>;
	
}

//------------------------------------------------------------------------------------------------------

ConvexHull::~ConvexHull()
{
}


//------------------------------------------------------------------------------------------------------

int ConvexHull::getN()
{
    
    return N;
}

//------------------------------------------------------------------------------------------------------
// http://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float RandomFloat(float min, float max)
{
    // this  function assumes max > min, you may want
    // more robust error checking for a non-debug build
    //assert(max > min);
    float random = ((float) rand()) / (float) RAND_MAX;
    
    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;
    return (random*range) + min;
}
//------------------------------------------------------------------------------------------------------
// randomize n floats in [min,max]

void ConvexHull::randomizePoints(int n, float min, float max)
{
    Point *p;
    
    //	srand((unsigned)time(0));
    
    for(int index=0; index<n; index++)
    {
        p = new Point;
        p->x = RandomFloat(min,max);
        vop->push_back(*p);
        
    }
    
    for(int index=0; index<n; index++)
        (*vop)[index].y = RandomFloat(min,max);
    
    N=n;
    
}


//------------------------------------------------------------------------------------------------------
// randomize n ints in [min,max]

void ConvexHull::randomizePoints(int n, int min, int max)
{
	Point *p; 
	
	srand((unsigned)time(0));
	int random_integer;
	
	for(int index=0; index<n; index++)
	{
		random_integer = (rand()%100)+1;
		p = new Point;
		p->x = (float) random_integer;
		vop->push_back(*p);
		
	}
	
	for(int index=0; index<n; index++)
	{
		random_integer = (rand()%100)+1;
		(*vop)[index].y = ((float) random_integer);
	}
	
	N=n;
	
}



//------------------------------------------------------------------------------------------------------

void ConvexHull::boundingBox()
{
    float XLENGTH, YLENGTH;
    
    MINX = (*vop)[0].x;
    MAXX = MINX;
    MINY = (*vop)[0].y;
    MAXY = MINY;
    
    for (int i=1; i<N; i++)
    {
        if ( (*vop)[i].x < MINX )
            MINX = (*vop)[i].x;
        
        if ( (*vop)[i].x > MAXX )
            MAXX = (*vop)[i].x;
        
        if ( (*vop)[i].y < MINY )
            MINY = (*vop)[i].y;
        
        if ( (*vop)[i].y > MAXY )
            MAXY = (*vop)[i].y;
    }
    
        // giving a glutter to borders
    MINX--;
    MAXX++;
    MINY--;
    MAXY++;
    
    XLENGTH = fabsf(MAXX) - fabsf(MINX);
    YLENGTH = fabsf(MAXY) - fabsf(MINY);
    
    if ( XLENGTH > YLENGTH )
    {
        MINY = MINY - (XLENGTH-YLENGTH) / 2;
        MAXY = MAXY + (XLENGTH-YLENGTH) / 2;
    }
    else //if ( XLENGTH < YLENGTH )
    {
        MINX = MINX - (YLENGTH-XLENGTH) / 2;
        MAXX = MAXX + (YLENGTH-XLENGTH) / 2;
    }
    
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::filterPointsInDiskShape()
{
    float x,y;
    for (int i=0; i<N; i++)
    {
        x = (*vop)[i].x;
        y = (*vop)[i].y;
        
        if ((x-50)*(x-50)+(y-50)*(y-50)>2500) // if outside the disk centered at (50,50)
        {
            vop->erase(vop->begin()+i);
            N--;
            i--;
        }
        
    }
    
}


//------------------------------------------------------------------------------------------------------

void ConvexHull::readPointsFromFile()
{
	Point *p; 
	float x, y, z;
	FILE *f;
 
        // Current directory: /Users/abelgomes/Documents/projectos.em.desenvolvimento/TORCH
    char * dir = getcwd(NULL, 0);
    cout << "Current directory: \n" << dir << endl;

        // Current subdirectory: /Users/abelgomes/Documents/projectos.em.desenvolvimento/TORCH/modelos/
   char * subdir = "/modelos/";
    strcat(dir, subdir);
    cout << "Pointsets' subdirectory: \n" << dir << "\n" << endl;
    
        // Pointsets' files:
        // 01-Bugatti.txt
        // 02-Formica.txt
        // 03-Airplane.txt
        // 04-T800Head.txt
        // 05-AlCapone.txt
        // 06-Vessel.txt
    char file[100];
    printf("Type a filename for an input pointset: \n");
    scanf("%s" , file);
    strcat(dir, file);
    cout << "File: \n" << dir << endl;

    
    
    //if((f=fopen("/Users/abelgomes/Documents/projectos.em.desenvolvimento/TORCH/modelos/points.txt","r"))==NULL)

    
    //if((f=fopen("/Users/abelgomes/Documents/projectos.em.desenvolvimento/TORCH/modelos/128-disk-100645.txt","r"))==NULL)
    //if((f=fopen("/Users/abelgomes/Documents/projectos.em.desenvolvimento/TORCH/modelos/256-disk-200957.txt","r"))==NULL)
    //if((f=fopen("/Users/abelgomes/Documents/projectos.em.desenvolvimento/TORCH/modelos/512-disk-402484.txt","r"))==NULL)
    
    if((f=fopen(dir,"r"))==NULL)
	{
		printf("!!!!Ficheiro nao encontrado!!!!\n\n");
		return;
	}
    
    while(!feof(f))
    {
        fscanf(f,"%f%f\r",&x,&y); // read in coordinates of each point
        
        p = new Point;
        //		p->x = x;
        //		p->y = y;
        p->x = 100*x;
        p->y = 100*y;
        //p->y = 100*z; //só para o caso da Formica
//        p->CONVEXHULLFLAG = false;
//        p->side = -1;
        vop->push_back(*p);
    }     

    
	fclose(f);
	
	N = vop->size();
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::writePointsToFile()
{
    float x, y;
    int n;
    FILE *f;
    
    if((f=fopen("/Users/abelgomes/Documents/projectos.em.desenvolvimento/TORCH/modelos/points.txt","w"))==NULL)
    {
        printf("!!!!Ficheiro inexistente!!!!\n\n");
        return;
    }
    
    n = vop->size();
    
    for (int i=0; i<n; i++)
    {
        x = (*vop)[i].x;
        y = (*vop)[i].y;
        
        fprintf(f,"%f %f\n",x,y); // write coordinates of each point to a file
        
    }   
    fclose(f);
}


//------------------------------------------------------------------------------------------------------

void ConvexHull::writeVerticesToFile()
{
    float x, y;
    int n;
    FILE *f;
    
    if((f=fopen("/Users/abelgomes/Documents/projectos.em.desenvolvimento/TORCH/modelos/points.txt","w"))==NULL)
    {
        printf("!!!!Ficheiro inexistente!!!!\n\n");
        return;
    }
    
    n = cycle->size();
    
    for (int i=0; i<n; i++)
    {
        x = (*cycle)[i].x;
        y = (*cycle)[i].y;
        
        fprintf(f,"%f %f\n",x,y); // write coordinates of each point to a file
        
    }   
    fclose(f);
}

//------------------------------------------------------------------------------------------------------


inline bool operator<(const Point& a, const Point& b)
{
		return a.x < b.x;
}


//------------------------------------------------------------------------------------------------------

void ConvexHull::sortPoints()
{
	std::sort((*vop).begin(),(*vop).end());
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::findIndicesOfPoles()
{
	float y, min, max;
	
	// already done
	WESTINDEX = 0;		
	EASTINDEX = N-1;
	
	// initialize the remaining two extrema
	SOUTHINDEX = 0;
	NORTHINDEX = 0;

	min = (*vop)[0].y;
	max = (*vop)[0].y;
	
	
	for(int i=1; i<N; i++)
	{
		y = (*vop)[i].y;
		
		if ( y <= min )
		{
			min = y;
			SOUTHINDEX = i;
		}

		if ( y >= max )
		{
			max = y;
			NORTHINDEX = i;
		}
		
	}

	
}



//------------------------------------------------------------------------------------------------------

void ConvexHull::SWHULL()
{
	float min; //, index;
    
	if (N==0)
		return;
    
	//index = 0;
	min = (*vop)[0].y;
    (*SW).push_back(0);
    
	for(int i=1; i<=SOUTHINDEX; ++i)
		if ( (*vop)[i].y <= min )
		{
            min = (*vop)[i].y;
            //index = i;
            (*SW).push_back(i);
		}
    
	return;
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::SEHULL()
{
    float min;//, index;
    
	if (N==0)
		return;
	
	//index = N-1;
	min = (*vop)[N-1].y;
	(*SE).push_back(N-1);
	
	for(int i=N-2; i>=SOUTHINDEX; --i)
		if ( (*vop)[i].y <= min )
		{
            min = (*vop)[i].y;
            //index = i;
            (*SE).push_back(i);
            
		}
	return;
}



//------------------------------------------------------------------------------------------------------

void ConvexHull::NWHULL()
{
	float max; // index;
    
	if (N==0)
		return;
	
	//index = 0;
	max = (*vop)[0].y;
	(*NW).push_back(0);
	
	for(int i=1; i<=NORTHINDEX; ++i)
		if ( (*vop)[i].y >= max )
		{
			max = (*vop)[i].y;
			//index = i;
			(*NW).push_back(i);
		}
    
	return;
}


//------------------------------------------------------------------------------------------------------

void ConvexHull::NEHULL()
{
    float max;//, index;
	
	if (N==0) 
		return;
	
	//index = N-1;;
	max = (*vop)[N-1].y;
	(*NE).push_back(N-1);
	
	for(int i=N-2; i>=NORTHINDEX; --i)
		if ( (*vop)[i].y >= max )
		{
			max = (*vop)[i].y;
			//index = i;
			(*NE).push_back(i);
		}
	return;
}


//------------------------------------------------------------------------------------------------------

void ConvexHull::approximateConvexHull()
{
	
	Point *P;
	int i;
	
	int bl = SW->size();
	int br = SE->size();
	int tl = NW->size();
	int tr = NE->size();
    
    //(*cycle).resize(bl+br+tl+tr);
	
	//--- BOTTOM-LEFT

	for (int index=0; index<bl; ++index)
	{
		i = (*SW)[index];
		P = &((*vop)[i]);
		cycle->push_back(*P);
        //(*cycle)[index] = *P;
	}
	
	//--- BOTTOM-RIGHT
	
	for (int index=br-2; index>=0; --index)
	{
		i = (*SE)[index];
		P = &((*vop)[i]);
		cycle->push_back(*P);
        //(*cycle)[bl+index] = *P;

	}
	
	// --- TOP-RIGHT
	
    //int IDX = bl+br;
	for (int index=1; index<tr; ++index)
	{
		i = (*NE)[index];
		P = &((*vop)[i]);
		cycle->push_back(*P);
        //(*cycle)[IDX+index] = *P;
	}

	//--- TOP-LEFT
	//IDX = bl+br+tl;
	for (int index=tl-2; index>=0; --index)
	{
		i = (*NW)[index];
		P = &((*vop)[i]);
		cycle->push_back(*P);
        //(*cycle)[IDX+index] = *P;
	}
	
	
}

//------------------------------------------------------------------------------------------------------
// A utility function to find next to top in a stack
Point nextToTop(stack<Point> &S)
{
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}


//------------------------------------------------------------------------------------------------------
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
    (q.x - p.x) * (r.y - q.y);
    
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}


//------------------------------------------------------------------------------------------------------
// inflate approximate convex hull A out to convex hull H

void ConvexHull::convexHull()
{
    
    N = cycle->size();
    
    
    S.push((*cycle)[0]);
    S.push((*cycle)[1]);
    S.push((*cycle)[2]);
    
    // Process remaining n-3 points
    for (int i = 3; i < N; i++)
    {
        // Keep removing top while the angle formed by points next-to-top,
        // top, and (*vop)[i] makes a non-left turn
        while ( (orientation(nextToTop(S), S.top(), (*cycle)[i]) != 2) && (S.size()>2) )
        {
            //printf("i=%i\n",i);
            
            //Point p = S.top();
            S.pop();
            //S.pop();
            //S.push(p);
        }
        S.push((*cycle)[i]);
    }
    //	printf("#S=%i\n",S.size());
    
}

//------------------------------------------------------------------------------------------------------

//void ConvexHull::inflateApproximateConvexHull()
//{
//	float DX, DY, dx, dy;
//	Point *A, *B, *C;
//	vector<Point> *newcycle = new vector<Point>;		// vector for hull points
//	
//	
//	N = cycle->size();
//	(*newcycle).resize(N);
//	int endindex = 0;
//	
//	int count = 0;
//	
//	if (N >= 3)
//	{
//		
//		// then rectify the sub-polyline from left minimum towards bottom minimum
//		do
//		{
//			N = cycle->size();
//			count = 0;
//			//(*newcycle)[newindex++] = (*A);
//			
//			for (int index=0; index<N; ++index)
//			{
//				if (index-count < 0)
//					count = N-index+count;
//				A = &((*cycle)[(index-count)%N]);  // count serves to forebracking of A in relation to next points
//				B = &((*cycle)[(index+1)%N]);
//				C = &((*cycle)[(index+2)%N]);
//				
//				dx = B->x - A->x;
//				dy = B->y - A->y;
//				DX = C->x - A->x;
//				DY = C->y - A->y;
//				
//				if ( dx * DY - dy * DX <= 0)
//				{
//					count++;
//				}
//				else
//				{
//					count=0;
//					B->CONVEXHULLFLAG = true;
//					//(*newcycle)[newindex++] = (*B);
//				}
//			}
//			
//			endindex = 0;
//			
//			for (int index=0; index<N; ++index)
//				if ( ((*cycle)[index]).CONVEXHULLFLAG )
//				{
//					((*cycle)[index]).CONVEXHULLFLAG = false;
//					(*cycle)[endindex++] = (*cycle)[index];
//					
//				}
//			
//			(*cycle).resize(endindex);
//			
//			printf("N=%i \t endindex=%i\n",N,endindex);
//			
//		} while (N > endindex);
//		
//	} // end of while
//}


//------------------------------------------------------------------------------------------------------

void ConvexHull::drawSouthPole(Color color)
{
	float x = (*vop)[SOUTHINDEX].x;
	float y = (*vop)[SOUTHINDEX].y;
	
	Circle c(x,y,100);
	c.drawBoundary(color);
	
	return;
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawNorthPole(Color color)
{
	float x = (*vop)[NORTHINDEX].x;
	float y = (*vop)[NORTHINDEX].y;
	
	Circle c(x,y,100);
	c.drawBoundary(color);
	
	return;
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawEastPole(Color color)
{
    printf("EASTINDEX=%i\n",EASTINDEX);
	float x = (*vop)[EASTINDEX].x;
	float y = (*vop)[EASTINDEX].y;
	
	Circle c(x,y,100);
	c.drawBoundary(color);
	
	return;
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawWestPole(Color color)
{
	float x = (*vop)[WESTINDEX].x;
	float y = (*vop)[WESTINDEX].y;
	
	Circle c(x,y,100);
	c.drawBoundary(color);
	
	return;
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawPoints(float radius, Color color)
{
	float x, y;
	Point *p;
	
	int n = vop->size();	
	
	for(int index=0; index<n; index++)
	{
		p = &((*vop)[index]);
		x = p->x;
		y = p->y;
		
		drawCircle(x, y, radius, color);
	}
	
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawPins(Color color)
{
	float x, y;
	Point *p;
	
	int n = N; //vop->size();	
	
	for(int index=0; index<n; index++)
	{
		p = &((*vop)[index]);
		x = p->x;
		y = p->y;
		
		glBegin(GL_LINES);
		glColor3f(color&4,color&2,color&1);
		glVertex2f(x,y);
		glVertex2f(x,0);
		glEnd();
	}
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawSWHull(Color color)
{
	int index;
	float x, y;
	Point *p;
	
	int n = SW->size();	
	
	
	glBegin(GL_LINE_STRIP);
	glColor3f(color&4,color&2,color&1);
	
	for(int i=0; i<n; i++)
	{
		index = (*SW)[i];
		
		p = &((*vop)[index]);
		
		x = p->x;
		y = p->y;
		
		glVertex2f(x,y);
	}
	
	glEnd();
	
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawSEHull(Color color)
{
	int index;
	float x, y;
	Point *p;
	
	int n = SE->size();	
	
	
	glBegin(GL_LINE_STRIP);
	glColor3f(color&4,color&2,color&1);
	
	for(int i=0; i<n; i++)
	{
		index = (*SE)[i];
		
		p = &((*vop)[index]);
		
		x = p->x;
		y = p->y;
		
		glVertex2f(x,y);
	}
	
	glEnd();
	
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawNWHull(Color color)
{
	int index;
	float x, y;
	Point *p;
	
	int n = NW->size();	
	
	
	glBegin(GL_LINE_STRIP);
	glColor3f(color&4,color&2,color&1);
	
	for(int i=0; i<n; i++)
	{
		index = (*NW)[i];
		
		p = &((*vop)[index]);
		
		x = p->x;
		y = p->y;
		
		glVertex2f(x,y);
	}
	
	glEnd();
	
}

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawNEHull(Color color)
{
	int index;
	float x, y;
	Point *p;
	
	int n = NE->size();	
	
	glBegin(GL_LINE_STRIP);
	glColor3f(color&4,color&2,color&1);
	
	for(int i=0; i<n; i++)
	{
		index = (*NE)[i];
		
		p = &((*vop)[index]);
		
		x = p->x;
		y = p->y;
		
		glVertex2f(x,y);
	}
	
	glEnd();
}


//------------------------------------------------------------------------------------------------------

//void ConvexHull::drawConvexHull(Color color)
//{
//	Point *P;
//	
//	int n = cycle->size();
//    printf("n=%i\n",n);
//	
//	glBegin(GL_LINE_STRIP);
//	glColor3f(color&4,color&2,color&1);
//	
//
//	for (int index=0; index<n; index++)
//	{
//       // printf("index=%i\n",index);
//		P = &((*cycle)[index]);
////		if (P->CONVEXHULLFLAG)
//			glVertex2f(P->x,P->y);
//	}
//	P = &((*cycle)[0]);
////	if (P->CONVEXHULLFLAG)
//		glVertex2f(P->x,P->y);
//	glEnd();
//}	

//------------------------------------------------------------------------------------------------------

void ConvexHull::drawConvexHull(Color color)
{
    Point FIRST = S.top();
    
    glBegin(GL_LINE_STRIP);
    glColor3f(color&4,color&2,color&1);
    
    while (!S.empty())
    {
        Point p = S.top();
        glVertex2f(p.x,p.y);
        //cout << "(" << p.x << ", " << p.y <<")" << endl;
        S.pop();
    }
    glVertex2f(FIRST.x,FIRST.y);
    glEnd();
}	


//------------------------------------------------------------------------------------------------------
//--- end of file

