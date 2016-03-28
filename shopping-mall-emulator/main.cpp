/*This function includes all the prototypes for the functions as well as the way that it calls
them. It runs given amount of robots and initializes them before processing them. It then 
runs through given amount of shoppers who are initialized and then processed.*/
#define MAIN 1
#include "globals.h"
Link eventQueueHead = NULL; //initializes the pointer for the start of the list
Link storeQueueHead = NULL;
Link storeQueueTail = NULL;
int totalTime = 0; //counter to keep track of the total time in the simulation
int STORES = 0; //will be initialized from command line arguments
int timeSlice = 0; //will be initialized from command line arguments
int isStoreBusy = 0; //counter to check if the store is busy, not used for this lab but used later
int R[3], S[3]; //R[3] holds shopper location and S[3] holds next store location


int init_mall (); // function prototypes
void printloc (int []);
bool IsEqual (int [], int []);
void d1(int [], int); 
void d2(int [], int[], int);
void d3(int [], int[], int);  
void next(int [], int [], int);
void twoway(int [], int [], int);
void elevator(int [], int[], int);
void move (int cur[], int des[], int size);
void initializeShopper(ShopperLink given);
void printtravel(int ttime [][2], int robots);
void initializeRobot(RobotLink given, int numberExpectStores);


int main (int argc, char *argv[]){
	Tree rootTree; // Initialize tree of items
	rootTree = Tree();

	int i = 0; //for loop variables
	int f = 0;
	
	int robotNum = 0; //number of robots program is expecting
	int shopperNum = 0;//number of shoppers expected

	EventList List;// initialize the robot event list
	List = EventList();
	
	EventListShopper SList; //initialize the shopper event list
	SList = EventListShopper();

	R[FLOOR] = First; //initialize the entrance
	R[ROW] = MID;
	R[COL] = MAX;

	init_mall();//INITIALIZES THE MALL
	
	if (argc != 2){ //checks to make sure the function is being run with the correct number of arguments
		std::cout << "wrong number of arguments" << std::endl;
		return 1;
	}
  
	//STORES = atoi(argv[1]); // initializes stores variable as the command line input: how many stores shopper is going to visit
	timeSlice = atoi(argv[1]); // initializes how long a time slice lasts
	if (timeSlice <= 0){ // this checks that there is a valid input for the sources
		std::cout << "Invalid arguments" << std::endl;
		return 1;
	}

	// BEGIN STAGE 1!
	std::cin >> robotNum; //read in the robot number
	
	int expectStoreNumbers[robotNum]; 
	for (i = 0; i < robotNum; i++){ 
		std::cin >> expectStoreNumbers[i]; //read in how many stores it is expecting per robot
	}
	
	robot robotArray[robotNum]; //create an array to hold the initial versions of the robots

	for (i = 0; i < robotNum; i++){
		initializeRobot(&(robotArray[i]), expectStoreNumbers[i]); //create the robots
		robotArray[i].robotID = i;
		robotArray[i].arrivalTime = i * 25;
	}
	

	for (f = 0; f < robotNum; f++){
		List.addToList(&(robotArray[f])); //create the eventList
	}

	while (List.getEventListHead() != NULL){
		List.processEventList(&rootTree); //process the eventList until it is empty
	}


	// BEGIN STAGE 2
	int n = 0;
	std::cin >> shopperNum; //read in the number of expected shoppers

	shopper shopperArray[shopperNum]; //create an array of initialized shoppers
	for (n = 0; n < shopperNum; n++){
		initializeShopper(&(shopperArray[n]));
		shopperArray[n].shopperID = n + 1;
	}

	rootTree.printTree();

	for (f = 0; f < shopperNum; f++){ 
		SList.addToList((&(shopperArray[f]))); //create eventList for shoppers
	}

	while (SList.getEventListHead() != NULL){
		SList.processEventList(rootTree); //process through the event list
	}
}
