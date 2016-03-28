//  RoboMall Program

/* This is Program 2 for CS2303 in A14.
   Author: Bob Kinicki

This file contains the main function and
help functions: move, travel and printtravel

                                             */

#include "globals.h"

// type, step and time are globals
int type [MAX+1][MAX+1][2];
int step [MAX+1][MAX+1][2];

// prototypes for other functions found in other files

int init_mall ();
void printloc (int []);
bool IsEqual (int [], int []);
void d1(int [], int); 
void d2(int [], int[], int);
void d3(int [], int[], int);  
void next(int [], int [], int);
void twoway(int [], int [], int);
void elevator(int [], int[], int);
//WRITTEN BY BOB KINICKI AND EDITED FOR C++ BY BRIAN RUBENSTEIN
// move function controls robot's steps  
void move (int cur[], int des[], int size)
{

/* Choices {Invalid, One, Two, Three, Next, 
   TwoWay, Elevator,Store}              

   The main idea is to categorize the type of
   choice the robot has and use switch to process
   that choice.  type array keeps choice type for
   all mall locations.

*/
  switch (type[cur[ROW]][cur[COL]][cur[FLOOR]])
  {
    case Store:    
    case One:      d1(cur, SIZE);
                   break;
    case Two:      d2(cur,des,SIZE);
                   break;
    case Three:    d3(cur,des,SIZE);
                   break;
    case Next:     next(cur,des,SIZE);
                   break;
    case TwoWay:   twoway(cur,des,SIZE);
                   break;
    case Elevator: elevator(cur,des,SIZE);
                   break;
    case INVALID:
    default:
        std::cout << "M: Lost in Mall at ";
		printloc(cur);
		std::cout << "Type = " << type[cur[ROW]][cur[COL]][cur[FLOOR]];
		exit(EXIT_FAILURE);
  } //end switch

  // Simulation time incremented here after one step!
  /*  This is a debug print in case you want to see individual
      robot steps.  

    printf("Time: %d Robot at ", time);
    printloc(cur);                        
  */
  return;
}

/* travel function takes robot  from 'from' location to 'to' location */
int travel (int from[], int to[], int size)
{
  int cntr = 0;        // cntr used to check for infinite loop
  
   //printloc(from);

   // Loop until robot arrives at store or A1
   while(!IsEqual(from,to) &&
         (cntr < MAX_PATH))
   {
     // move controls robot's steps
     move(from,to, SIZE);
     cntr++;
   }
   if (cntr == MAX_PATH) 
   {
	   std::cout << std::endl << "Infinite Loop";
     exit(EXIT_FAILURE);
   }
   return cntr;
 }

void printtravel(int ttime [][2], int robots)
{
  int i;
  std::cout << "Shopper   Start Time   Finish Time" << std::endl;
  std::cout << "      1" << "   " << ttime[0][STIME] << "   " << ttime[0][ETIME] << std::endl;
  return;
}

