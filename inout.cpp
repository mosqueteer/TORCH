/*
 *  inout.cpp
 *  pinconvexhull
 *
 *  Created by Abel Gomes on 3/31/13.
 *  Copyright 2013 Universidade da Beira Interior. All rights reserved.
 *
 */

#include "inout.h"

//------------------------------------------------------------------------------------------------------

Point* Read_File(FILE *f,int *size)
{  
    Point *PointSet=NULL, aux;
	
    while(1)
	{
		fscanf(f,"%f%f",&aux.x,&aux.y); // read in coordinates of each point
		
		if(feof(f))						// check end of file 
			return PointSet;
		
			// allocate memory space for new point
		PointSet=(Point*)realloc(PointSet,(++(*size))*sizeof(Point)); 
		
		PointSet[(*size)-1].x=aux.x;  // x-coordinate of new point
		PointSet[(*size)-1].y=aux.y;  // y-coordinate of new point
		
	}     
}
