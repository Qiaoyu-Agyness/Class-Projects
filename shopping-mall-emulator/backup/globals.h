/*global variables*/
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
using std::cin;
using std::cout;
using std::endl;
//THIS PIECE OF CODE WAS WRITTEN BY BOB KINICKI: HE HAS FULL CREDIT
#if !defined(NULL)//check if NULL is defined, if it isn't: define it
  #define NULL 0
#endif

#ifndef GLOBALS_H
#define GLOBALS_H
#define ROW    0
#define COL    1
#define FLOOR  2
#define SIZE   3
#define MIN    0
#define MID    8
#define MAX   16
#define INVALID 9
#define MAX_ROBOTS  9
#define MAX_STORES 20
#define MAX_PATH  200 
#define STIME 0
#define ETIME 1
//END KINICKI CODE
//This macro is written by Brian Rubenstein
#define MINMAC(x, y) ((x) < (y) ? (x) : (y))
//WRITTEN BY BOB KINICKI: HE HAS FULL CREDIT
enum Choices    {Invalid, One, Two, Three, Next, TwoWay, Elevator, Store};
enum Coordinate {Row, Col, Floor};
enum Direction  {North, NorthEast, East, SouthEast,
		 South, SouthWest, West, NorthWest};
enum Floors     {First, Second};
enum statetype {entering, arriving, waiting, processing}; //enumeration for shopper state: by Brian Rubenstein

#endif

//This structure is written by Brian Rubenstein
struct node{ // define the structure for node
	int xPos;
	int yPos;
	int serviceTime;
	int arrivalTime;
	int storeNum;
	int totalTime;
	int hasArrived;
	int numTimeSlices;
	struct node *link; // self referencial link
};

typedef node *Link;

//Written by Brian Rubenstein
//structure for item
struct item{
	std::string name;
	int count;
	item *itemLink;
};

typedef item *ItemLink;

//Written by Qiaoyu Liao
//structure for store
struct store{
	int xPos;//store position
	int yPos;
	int zPos;
	int targetItemNum;//how many items the shopper is looking for at the store/how many the robot is dropping off
	store *storeLink;
	ItemLink targetItem;//list of items being obtained/dropped off at the store
};
typedef store *StoreLink;


//Written by Brian Rubenstein
//structure for shopper
struct shopper{
	int shopperID;
	int xPos;//position
	int yPos;
	int zPos;
	int arrivalTime;//arrival time for the event list
	int targetStoreNum;//what number store is this?
	int expectedItems;//how many items is the shopper expecting as input
	int itemsObtained;//how many items has the shopper obtained at the store
	statetype State;//what is the shopper doing
	ItemLink shoppingList;//Takes in input
	StoreLink targetStore;//what store are they travelling to
	ItemLink itemsAtStore;//what items are at the store
	shopper *shopperLink;
};
typedef shopper *ShopperLink;

//Written by Qiaoyu Liao
//structure for robot
struct robot{
	int robotID;
	int xPos;//position
	int yPos;
	int zPos;
	int arrivalTime;//arrival time for the event list
	int targetStoreNum;//what number store is this?
	StoreLink targetStore;//what store are they travelling to
	robot *robotLink;
};
typedef robot *RobotLink;


//Written by Qiaoyu Liao
//structure for items in the tree
struct listnode{
	int num;// number of items
	int xpos;// x-position
	int ypos;//y-position
	int zpos;//z-position
	struct listnode *nextListnode;// pointer to the next listnode
};
typedef listnode *ListnodePtr;


//Written by Qiaoyu Liao
//node in the binary tree of item name
struct treenode{
	treenode *leftTreenode;//left pointer
	std::string name;// name code of the item
	listnode *list;// list of the position and item numbers of the item
	treenode *rightTreenode;//right pointer
};
typedef treenode *TreenodePtr;

//Written by Qiaoyu Liao
// tree class definition
class Tree{
public:	
	Tree();//constructor
	void insertTreenode(std::string givename,int count, int xposition, int yposition, int zposition);// insert an item into the tree
	void printTree();//print out the tree
	StoreLink findItem(ItemLink givenItem);
private:
	treenode *basenode;
	void insertTreenode(std::string givename,int count, int xposition, int yposition, int zposition, TreenodePtr *leaf);
	void printTree(treenode *leaf);
	StoreLink findItem(ItemLink givenItem, TreenodePtr givenTreenode);
};
//initiate a new null tree structure


//Written by Brian Rubenstein
//class for the robot event list
class EventList{
public:
	EventList();
	void setEventListHead(RobotLink given);
	void addToList(RobotLink given);

	void processEventList(Tree* rootTree);
	RobotLink getEventListHead();
private:
	robot *eventListHead;
};

//Written by Brian Rubenstein
//class for the shopper event list
class EventListShopper{
public:
	EventListShopper();
	void setEventListHead(ShopperLink given);
	void addToList(ShopperLink given);
	void processEvent(Tree rootTree);
	void processEventList(Tree rootTree);
	ShopperLink getEventListHead();
private:
	shopper *eventListHead;
};


//Written by Brian Rubenstein
#ifndef MAIN
extern Link eventQueueHead; //initializes the pointer for the start of the list
extern Link storeQueueHead;//initializes the CPU queue
extern Link storeQueueTail;
extern int totalTime;
extern int STORES; //will be initialized from command line arguments
extern int timeSlice; //will be initialized from command line arguments
extern int isStoreBusy;
int travel (int from[], int to[], int size);
//WRITTEN BY BOB KINICKI
extern int R[3], S[3]; //R[3] holds shopper location and S[3] holds next store location
extern int type  [MAX+1][MAX+1][2];
extern int step  [MAX+1][MAX+1][2];
#endif
//Written by Brian Rubenstein
Link initEvent();
void add_event (Link newPtr);
void processEvent();
void addToQueue(Link *head, Link *tail, Link given);
