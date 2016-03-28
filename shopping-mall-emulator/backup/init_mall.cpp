//  RoboMall Program
/* init_mall fills in specific characteristics of all
   the locations on the First and Second Floors of RoboMall */
//THIS CODE IS WRITTEN BY BOB KINICKI AND EDITED FOR C++ BY BRIAN RUBENSTEIN
#include<stdio.h>
#include "externals.h"

/* This print function serves to print out the settings
   of the type array by init_mall. This is useful to make
   sure this property has been set correctly.              */

void printType(){
int i,j,k ;
char cell;

 for (k = First; k <= Second; k++)
 {
   printf("      ");
   for (j = MIN; j <= MAX; j++)
     printf("%2d",j);
   
   for (i = MIN; i <= MAX; i++)
   {
     printf("\n %2d   ",i);

     for (j = MIN; j <= MAX; j++)
     {
       if (type[i][j][k] == INVALID)
	 cell = ' ';
       else
	 cell = 48 + type[i][j][k];
       printf(" %c", cell);
     }
   }
   printf("\n                %d Floor Types\n\n", k);
 }
 return;
}

/* This print function serves to print out the settings
   of the step array by init_mall. This is useful to make
   sure this property has been set correctly.              */


void printStep(){
int i,j,k ;
char cell;

 for (k = First; k <= Second; k++)
 {
   printf("      ");
   for (j = MIN; j <= MAX; j++)
     printf("%2d",j);
   
   for (i = MIN; i <= MAX; i++)
   {
     printf("\n %2d   ",i);

     for (j = MIN; j <= MAX; j++)
     {
       if (step[i][j][k] == INVALID)
	 cell = ' ';
       else
	 cell = 48 + step[i][j][k];
       printf(" %c", cell);
     }
   }
   printf("\n                %d Floor Steps\n\n", k);
 }
 return;
}

/* init_mall initializes type and step
   based on characteristics of the two floors of RoboMall  */

void init_mall()
{
int i,j,k ;
int dir[8];

// First initialize sparse arrays type and step

 for (k = First; k <= Second; k++)
   for (i = MIN; i <= MAX+1; i++)
   for (j = MIN; j <= MAX+1; j++)

   //  INVALID means the robot should never go here!
   {
       type[i][j][k] = INVALID;
       step[i][j][k] = INVALID;
   }

 for (k = First; k <= Second; k++)
 {
   // The directions for the two floors differ
   if (k == First) {//if first floor
     for (i = 0; i <= 7; i++){
       dir[i] = i; // 0 1 2 3 4 5 6 7 INITIALIZE FIRST FLOOR DIRECTIONS OF TRAVEL
	 }
   }
   else { //if second floor
     for (i = 0; i <= 7; i++)
       dir[i] = (i+4)%8; //4 5 6 7 0 1 2 3 INITIALIZE SECOND FLOOR DIRECIONS OF TRAVEL
   }

   for (j = 6; j < 11; j++)
   {
     type[MIN][j][k] = type[MAX][j][k] = One; //INITIALIZE TOP SIDE, LEFT SIDE, RIGHT SIDE, AND BOTTOM SIDE
     type[j][MIN][k] = type[j][MAX][k] = One;

     step[MIN][j][k] = dir[6];
     step[MAX][j][k] = dir[2];
     step[j][MIN][k] = dir[4];
     step[j][MAX][k] = dir[0];
   }

   for (j = 1; j <= 13; j = j+3)
   {
     for (i = 0; i <= 2; i++)
     {
       type[MID][j+i][k] = One; //INITIALIZE CROSSBAR ONE WAYS
       type[j+i][MID][k] = One;

       step[MID][j+i][k] = dir[2];
       step[j+i][MID][k] = dir[4];
     }
   }

   for (i = 4; i <= 12; i = i+8)
   {
     type[i][4][k] = type[i][12][k] = Store; // INITIALIZE STORES, TWO WAY AND THREE WAY LOCATIONS
     type[i][6][k] = type[i][10][k] = Store;
     type[6][i][k] = type[10][i][k] = Store;
     type[i][7][k] = type[i] [9][k] = TwoWay;
     type[7][i][k] = type[9][ i][k] = TwoWay;
     type[i][MID][k] = type[MID][i][k] = Three;

     step[i][MID][k] = dir[4];
     step[MID][i][k] = dir[2];
   }

   step[MID][MID][k] = dir[2]; //INITIALIZE ELEVATOR
   for (j = 5; j >= 1; j--)
   {
     type[6-j][j][k]  = type[16-j][10+j][k] = One; //INITIALIZE DIAGONALS TOP LEFT AND BOT RIGHT

     step[6-j][j][k]  = dir[5];
     step[16-j][10+j][k] = dir[1];
   }

   for (i = 1; i <= 5; i++)
   {
     type[i][10+i][k] = type[10+i][i][k] = One; //INITIALIZE DIAGONALS BOT LEFT AND TOP RIGHT

     step[i][10+i][k] = dir[7];
     step[10+i][i][k] = dir[3];   
   }

   for (i = 3; i <= 13; i = i+10)
     type[i][i][k] = Next; //INITIALIZE STORE CROSSROADS ON DIAGONALS
   type[3][13][k]  = type[13][3][k] = Next;
   type[MID][MID][k] = Elevator; //INITIALIZE ELEVATOR
 } //end first large for loop
 
// printf("\nInit 2");
 type[MIN][MID][First]  = type[MID][MIN][First]  = Two; //INITIALIZE ENTRANCES TO CROSSBARS
 type[MID][MAX][Second] = type[MAX][MID][Second] = Two;

 for (i = 0; i <= 7; i++)
   dir[i] = i;

 for (k = First; k <= Second; k++)
 {
   step[ 4][ 4][k] = dir[7]; //INITIALIZE STORES OFF OF DIAGONALS
   step[ 4][12][k] = dir[1];
   step[12][ 4][k] = dir[5];
   step[12][12][k] = dir[3];

   for (j = 4; j <= 12; j = j+8)
   {
     step[ 6][ j][k] = step[ 7][ j][k] = dir[4]; //INITIALIZE STORES AND ENTRYWAYS OFF CROSSBARS
     step[ 9][ j][k] = step[10][ j][k] = dir[0];
     step[ j][ 6][k] = step[ j][ 7][k] = dir[2];
     step[ j][ 9][k] = step[ j][10][k] = dir[6];
   }
 }
 step[MIN][6] [First] = step[10][MAX][Second] = dir[5];//INITIALIZE DIAGONAL ENTRANCES AND EXITS
 step[10][MIN][First] = step[MIN][10][Second] = dir[3];
 step[MAX][10][First] = step[6][MIN][Second]  = dir[1];
 step[6][MAX][First]  = step[MAX][6][Second]  = dir[7];

 /* These two print functions show that type and step
    are correct.  They can be commented out once program */

 //printType();
 //printStep();

 return;
}  //end init_mall
