#include <stdio.h>
#include <iostream>
#include <math.h>
#include <glut/glut.h>

using namespace std;
#include "convexhull.h"
#include <time.h>
#include <stack>

//--- bounding box in R^2
//#define Xmin 0
//#define Xmax 101
//#define Ymin 0
//#define Ymax 101


//--- viewport
#define Xwidth 500
#define Yheight 500


ConvexHull *CH = new ConvexHull;
stack<int> s;

//int N;

//------------------------------------------------------------------------------------------------------

void myCurveDrawing()
{
    //--- Set background color
    glClearColor(0.9,0.9,0.9,0);
    glClear (GL_COLOR_BUFFER_BIT);
    
    //--- Clear projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //--- Set domain in R^2
    //gluOrtho2D(Xmin,Xmax,Ymin,Ymax);
    gluOrtho2D(CH->MINX,CH->MAXX,CH->MINY,CH->MAXY);  // bounding box centered at the origin of R^2
    
    //--- Enable antialiased lines
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
    
    //--- Set line width
    float lineWidth = 1.0f;
    glLineWidth(lineWidth);
    
    printf("N = %d\n",CH->getN());
    
    //--- for shaped point sets if used
    //CH->filterPointsInDiskShape();
    printf("N = %d\n",CH->getN());
    
    //--- Draw point multiset of the convex hull
    //CH->drawPoints(2,RED); //for shapes
    //  CH->drawPoints(2.0,RED); //for al capone e buggati
    //	CH->drawPoints(3.0,RED); //for flight e vagabond
    //	CH->drawPoints(80.0,RED); //for head
    //	CH->drawPoints(1.0,RED); // for formica
    //	CH->drawPoints(1.0,RED); // for arbitrary point sets
    
    //--- Start counting time
    //	time_t start,end;
    //	double dif;
    //	time (&start);
    
    printf("N = %d\n",CH->getN());
    
    clock_t time = clock();
    
    //--- Sort points in x-direction
    CH->sortPoints();
    //printf("N of v=%i\n",CH->getN());
    
    printf ("It took you %lf seconds to sort point set.\n", (double)(clock()-time)/CLOCKS_PER_SEC );
    
    //--- Find indices of extremal vertices (NORTH, SOUTH, WEST, EAST)
    CH->findIndicesOfPoles();
    
    printf ("It took you %lf seconds to find indices of poles.\n", (double)(clock()-time)/CLOCKS_PER_SEC );
    
    //--- Build lateral approximate hulls
    CH->SWHULL();
    CH->SEHULL();
    CH->NWHULL();
    CH->NEHULL();
    
    printf ("It took you %lf seconds to find 4 hulls.\n", (double)(clock()-time)/CLOCKS_PER_SEC );
    
    CH->approximateConvexHull();
    
    printf ("It took you %lf seconds to find approx hull.\n", (double)(clock()-time)/CLOCKS_PER_SEC );
    
    //CH->inflateApproximateConvexHull();
    CH->convexHull();
    
    printf ("It tookA you %lf seconds to find convex hull.\n", (double)(clock()-time)/CLOCKS_PER_SEC );
    
    //	time (&end);
    //	dif = difftime (end,start);
    //	printf ("It took you %.2lf seconds to type your name.\n", dif );
    
    //--- Draw vertical point pins
    //	CH->drawPins(RED);
    
//    CH->drawSWHull(BLUE);
//    CH->drawSEHull(MAGENTA);
//    CH->drawNWHull(RED);
//    CH->drawNEHull(CYAN);
    
    //    glLineWidth(lineWidth+2.0);
    //
    //    CH->drawSWHull(BLUE);
    //    CH->drawSEHull(BLUE);
    //    CH->drawNWHull(BLUE);
    //    CH->drawNEHull(BLUE);
    
//    CH->drawEastPole(BLACK);
//    CH->drawWestPole(BLACK);
//    CH->drawNorthPole(BLACK);
//    CH->drawSouthPole(BLACK);
    
    //	glLineWidth(lineWidth+2.0);
    
    	CH->drawConvexHull(BLACK);
    
    printf("N merda = %d\n",CH->getN());

    //CH->writeVerticesToFile();
    
    
    glutSwapBuffers();
    printf("merda...");
    return;
}





//------------------------------------------------------------------------------------------------------

void myKeyboard(unsigned char key, int x, int y)
{
    
    switch (key)
    {
        case 27:
            exit(0);
        case 's':
            CH->writePointsToFile();
            break;
        default:
            break;
    }
    
    
}

//------------------------------------------------------------------------------------------------------

void printVector(vector<int> *v)
{
    int n = v->size();
    for(int i=0; i<n; i++)
        printf("i=%i : %i\n",i,(*v)[i]);
    
}

//------------------------------------------------------------------------------------------------------
// randomize n ints in [min,max]

void randomizeInts(vector<int> *v, int n, int min, int max)
{
    
    srand((unsigned)time(0));
    int x;
    
    int m = max-min;
    
    for(int index=0; index<n; index++)
    {
        x = min+(rand()%m);
        v->push_back(x);
        
    }
    
    
}

//------------------------------------------------------------------------------------------------------

void mmx(vector<int> *v, int *min, int *max)
{
    int _min, _max;
    int n = v->size();
    
    _min = (*v)[0];
    _max = (*v)[0];
    
    
    for(int i=1; i<n; i++)
    {
        
        if ( (*v)[i] < _min )
            _min = (*v)[i];
        
        if ( (*v)[i] > _max )
            _max = (*v)[i];
        
    }
    
    (*min) = _min;
    (*max) = _max;
}

//------------------------------------------------------------------------------------------------------
// randomize n ints in [min,max]

void mysort(vector<int> *v, vector<int> *a,int min, int max)
{
    register int x;
    register int _min = min;
    int n = v->size();
    
    int m = max-_min;
    a->resize(m+1);
    
    
    for(int i=0; i<n; i++)
    {
        x = (*v)[i];
        (*a)[x-_min]=x;
        
    }
    
    
}

//------------------------------------------------------------------------------------------------------
// randomize n ints in [min,max]

void mysorting(vector<int> *v, int min, int max)
{
    register int x,xaux;
    int n = v->size();
    
    for (int i=0; i<n; i++)
    {
        if (i==(*v)[i])
            continue;
        
        x = (*v)[i];
        //printf("i = %i : x = %i : (*v)[%i] = %i\n",i,x,x,(*v)[x]);
        (*v)[i] = -1;
        
        if ((*v)[x]==-1) (*v)[x] = x; // fim de linha
        
        while ( x!=(*v)[x] )
        {
            xaux = x;
            x = (*v)[x];
            (*v)[xaux] = xaux;
            
        }
        //printf("i = %i\n",i);
        //printf("------\n");
        //printVector(v);
    }
}


//------------------------------------------------------------------------------------------------------
// randomize n ints in [min,max]

void mysortingwithreps(vector<int> *v, int min, int max)
{
    register int x,xaux;
    int n = v->size();
    
    for (int i=0; i<n; i++)
    {
        if (i==(*v)[i])
            continue;
        
        x = (*v)[i];
        //printf("i = %i : x = %i : (*v)[%i] = %i\n",i,x,x,(*v)[x]);
        (*v)[i] = -1;
        
        if ((*v)[x]==-1) (*v)[x] = x; // fim de linha
        
        while ( x!=(*v)[x] )
        {
            xaux = x;
            x = (*v)[x];
            (*v)[xaux] = xaux;
            
        }
        //printf("i = %i\n",i);
        //printf("------\n");
        //printVector(v);
    }
}

//------------------------------------------------------------------------------------------------------
// randomize n ints in [min,max]

void compress(vector<int> *a)
{
    int n = a->size();
    
    int blankindex = -1;
    for (int i=0; i<n; i++)
        if ((*a)[i] != 0)
            continue;
        else
        {
            blankindex = i;
            break;
        }
    if (blankindex==-1)
        return;
    
    for (int i=blankindex+1; i<n; i++)
        if ((*a)[i] != 0)
        {
            (*a)[blankindex] = (*a)[i];
            (*a)[i] = 0;
            blankindex++;
        }
        else
            continue;
    
    a->resize(blankindex);
    
}


//------------------------------------------------------------------------------------------------------
// randomize n ints in [min,max]

void compress(vector<int> *a, stack<int> *s)
{
    int n = a->size();
    
    for (int i=0; i<n; i++)
        if ((*a)[i] != 0)
            s->push((*a)[i]);
    
}


//------------------------------------------------------------------------------------------------------

int main (int argc,  char * argv[])
{
    //    vector<int> v, a;
    //    stack<int> s;
    //    int min=0, max=99;
    //    randomizeInts(&v,1000000,0,999999);
    //    //printVector(&v);
    //
    //    clock_t time = clock();
    ////    mmx(&v,&min,&max);
    ////    printf ("It took you %lf seconds to minmax.\n", (double)(clock()-time)/CLOCKS_PER_SEC );
    //    //printf("min=%i  \t  max=%i\n",min,max);
    //    //mysort(&v, &a, min, max);
    //    mysorting(&v, min, max);
    //    //printVector(&v);
    //
    //    printf ("It took you %lf seconds to sorting.\n", (double)(clock()-time)/CLOCKS_PER_SEC );
    //    //printf("N of a=%i\n",a.size());
    //   //compress(&a);
    //    //a.erase( unique( a.begin(), a.end() ), a.end() );
    //    //a.erase(std::remove(a.begin(), a.end(), 0), a.end());
    //    v.erase(std::remove(v.begin(), v.end(), -1), v.end());
    //    //compress(&a,&s);
    //    printf ("It took you %lf seconds to remove 0's.\n", (double)(clock()-time)/CLOCKS_PER_SEC );
    //    //printf("N of a=%i\n",a.size());
    //    printf("N of v=%i\n",v.size());
    //    //printf("N of s=%i\n",s.size());
    //
    //    //printVector(&a);
    //
    //    //getchar();
    
    //	CH->randomizePoints(50,0,100);
    //	CH->randomizePoints(128000,0,100);
    //	CH->randomizePoints(256000,0,100);
    //	CH->randomizePoints(512000,0,100);
    //	CH->randomizePoints(1000000,0,1000000);
    //	CH->randomizePoints(2000000,0,100);
    //	CH->randomizePoints(4000000,0,100);
    //	CH->randomizePoints(8000000,0,100);
    //  CH->randomizePoints(16000000,0,100);
    //	CH->randomizePoints(32000000,0,100);
    //	CH->randomizePoints(64000000,0,100);
    //	CH->randomizePoints(128000000,0,100);
    //	CH->randomizePoints(256000000,0,100);
    	CH->randomizePoints(1000000,0,100);
    
    //CH->readPointsFromFile();
    CH->boundingBox();
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(Xwidth,Yheight);
    glutCreateWindow("TORCH: Total Order-Based Convex Hull");
    glutDisplayFunc(myCurveDrawing);
    glutKeyboardFunc(myKeyboard);
    glutMainLoop();
    return 0;
} 

