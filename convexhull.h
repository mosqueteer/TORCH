/*
 *  convexhull.h
 *  TORCH Project (Total Order-Based Convex Hull)
 *
 *  Created by Abel Gomes on 1/12/13.
 *  Copyright 2013 Universidade da Beira Interior. All rights reserved.
 *
 */

#ifndef ConvexHull_
#define ConvexHull_

#include<iterator>
#include<vector>
#include<map>
#include<iostream>

#include "datatypes.h"

using namespace std;
class Point;


class ConvexHull
{
	vector<Point> *vop;		// initial vector of points spread in boxed domain
	int N;					// size of vop

	int SOUTHINDEX;
	int NORTHINDEX;
	int WESTINDEX;
	int EASTINDEX;
		
	vector<int> *NE;		// vector for 1st quadrant points
	vector<int> *NW;		// vector for 2nd quadrant points
	vector<int> *SW;		// vector for 3rd quadrant points
	vector<int> *SE;		// vector for 4th quadrant points
	
	vector<Point> *cycle;	// vector for hull points 
	
	
public: 
    float MINX, MAXX, MINY, MAXY;			// bounding box

    ConvexHull();
	virtual ~ConvexHull();
	
    void randomizePoints(int, float, float);
    void randomizePoints(int, int, int);
	void readPointsFromFile();
	void writePointsToFile();
    void writeVerticesToFile();
	
    int getN();
    
    void boundingBox();
    void filterPointsInDiskShape();


	// --- sorting methods for points
	
	void sortPoints();
	
	// --- finding indices of poles
	
	void findIndicesOfPoles();
	
	// --- finding lateral approximate hulls

	void SWHULL();
	void SEHULL();
	void NWHULL();
	void NEHULL();
		
	// --- inflating methods for points

	void approximateConvexHull();		
	void inflateApproximateConvexHull();
    void convexHull();
	
	// --- drawing nethods

	void drawPoints(float, Color);
	void drawPins(Color);
	
	void drawSouthPole(Color);
	void drawNorthPole(Color);
	void drawEastPole(Color);
	void drawWestPole(Color);	
	
	void drawSWHull(Color);
	void drawSEHull(Color);
	void drawNWHull(Color);
	void drawNEHull(Color);

	void drawConvexHull(Color);
};

#endif
