// Set of  small useful functions

#include "externals.h"
//THIS CODE IS WRITTEN BY BOB KINICKI AND EDITED FOR C++ BY BRIAN RUBENSTEIN
// debug function 
void printloc(int cell[])
{
	std::cout << "row = " << cell[0] << " col = " << cell[1] << " floor = " << cell[2] << std::endl;
  return;
}

/* IsEqual tests to see if robot has
   arrived at destination.          */
bool IsEqual(int now[], int des[])
{
  if ((now[0] == des[0]) &&
      (now[1] == des[1]) &&
      (now[2] == des[2]))
    return true;
  else
    return false;
}
