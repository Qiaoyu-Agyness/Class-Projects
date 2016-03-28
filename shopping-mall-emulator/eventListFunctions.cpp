/*eventListFunctions*/
#include "globals.h"


EventList::EventList(){ //constructor for robot eventList
	eventListHead = NULL;
}

void EventList::setEventListHead(RobotLink given){ //setter for robot
	eventListHead = given;
}

RobotLink EventList::getEventListHead(){ //getter for robot
	return eventListHead;
}

void EventList::addToList(RobotLink given){
	given->robotLink = NULL; // make sure the given robot (new robot) has no link
	RobotLink previousPtr;
	RobotLink currentPtr;
	
	previousPtr = NULL;
	currentPtr = eventListHead;
	if (currentPtr == NULL){ // if there is nothing in the event list, add it to the head
		setEventListHead(given);
	} else { //otherwise
		while( currentPtr != NULL && (given->arrivalTime > currentPtr->arrivalTime || (given->arrivalTime == currentPtr->arrivalTime && given->robotID > currentPtr->robotID))){ 
			//keep going through the event list as long as the given robot has a later arrival time, or they are the same and it has a greater ID;
			previousPtr = currentPtr;
			if (currentPtr->robotLink != NULL){ //if it has not hit the end yet
				currentPtr = currentPtr->robotLink; //move current pointer
			} else {
				currentPtr = NULL;//otherwise, currentPtr should be null
			}
		}
		if (previousPtr != NULL){ // if the robot given did not have the earliest arrival time
			previousPtr->robotLink = given; //place it in the list
			given->robotLink = currentPtr;
		} else { //otherwise
			eventListHead = given; //set the robot at the front of the list
			given->robotLink = currentPtr;
		}
	}
}



void EventList::processEventList(Tree* rootTree){//Processes through the event list once
	if (eventListHead->targetStore == NULL){ //if it is done, send it to the entrance
		store *entrance = new store;
		if (entrance == NULL){
			std::cout << "No memory available" << endl;
			return;
		} else {
			entrance->xPos = 8;
			entrance->yPos = 16;
			entrance->zPos = 0;
		}
		eventListHead->targetStore = entrance;
	}

	if (eventListHead->xPos != eventListHead->targetStore->xPos || eventListHead->yPos != eventListHead->targetStore->yPos || eventListHead->zPos != eventListHead->targetStore->zPos){//if it is not at its target store
		if(eventListHead->xPos == 8 && eventListHead->yPos == 16 && eventListHead->zPos == 0){ //if it is at the entrance, and has a target store, say that it entered
			std::cout << "Robot R" << eventListHead->robotID + 1 << " enters the RoboMall at time " << eventListHead->arrivalTime << std::endl;
		}
		
		R[0] = eventListHead->xPos;
		R[1] = eventListHead->yPos;
		R[2] = eventListHead->zPos;
		S[0] = eventListHead->targetStore->xPos;
		S[1] = eventListHead->targetStore->yPos;
		S[2] = eventListHead->targetStore->zPos;
		eventListHead->arrivalTime += travel(R, S, SIZE); // travel it to its target store
		robot *tmpPtr = new robot;

		eventListHead->xPos = S[0]; //Move the robot's x y and z coordinates to its new location
		eventListHead->yPos = S[1];
		eventListHead->zPos = S[2];

		if (tmpPtr == NULL){
			std::cout << "No memory available" << endl;
			return;
		} else { //add it back to the list, with traveltime added to its arrival time
			tmpPtr = eventListHead;
			eventListHead = eventListHead->robotLink;
			addToList(tmpPtr);
		}
	} else { //if it is at its store
		if(eventListHead->xPos != 8 || eventListHead->yPos != 16 || eventListHead->zPos != 0){ //check that its 'store' is not the exit, if it isnt, print out that it arrived
			std::cout << "Robot R" << eventListHead->robotID + 1 << " arrives at store " << eventListHead->targetStoreNum << "(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << " Floor at time " << eventListHead->arrivalTime << std::endl;
			eventListHead->arrivalTime += eventListHead->targetStore->targetItemNum; //increment arrival time by the amount of items it has to drop off
			std::cout << "Robot R" << eventListHead->robotID + 1 << " delivered the following items to store " << eventListHead->targetStoreNum << "(" << eventListHead->xPos << "," << eventListHead->yPos << "):" << std::endl; //print out that it is delivering items
			
			int l = 0; //counter variable
			for ( l = 0; l < eventListHead->targetStore->targetItemNum; l++){//keep going as long as items remain
				(*rootTree).insertTreenode(eventListHead->targetStore->targetItem->name, eventListHead->targetStore->targetItem->count, eventListHead->targetStore->xPos, eventListHead->targetStore->yPos, eventListHead->targetStore->zPos); //add the next item to the tree of items
				std::cout << "	Item name: "<<eventListHead->targetStore->targetItem->name << " Quantity of Item: " << eventListHead->targetStore->targetItem->count << std::endl; //print out the item delivery
				item *tempItemPtr = new item;
				if
					(tempItemPtr == NULL){
						std::cout << "No memory left." << std::endl;
						return;
				}
				else if (eventListHead->targetStore->targetItem->itemLink != NULL){ //if it still has items left
					tempItemPtr = eventListHead->targetStore->targetItem;
					eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
					delete tempItemPtr; //delete the finished item, and move to the next
				}
			}
			std::cout << "Robot R" << eventListHead->robotID + 1 << " leaves store "<< eventListHead->targetStoreNum << "(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << " Floor at time " << eventListHead->arrivalTime << std::endl;//print out that it left the store
			eventListHead->targetStoreNum += 1; //set its target store num variable to one more (for print outs)
			store *tmpStorePtr = new store;
			if (tmpStorePtr == NULL) {
				std::cout << "No memory available" << std::endl;
				return;
			} else {
				tmpStorePtr = eventListHead->targetStore;
			}

			eventListHead->targetStore = eventListHead->targetStore->storeLink; //tell it to go to the next store
			delete tmpStorePtr; //delete the finished store
			robot *tmpPtr = new robot;
			if (tmpPtr == NULL){
				std::cout << "No memory available" << std::endl;
				return;
			} else {//re-add the robot to the event list
				tmpPtr = eventListHead;
				eventListHead = eventListHead->robotLink;
				addToList(tmpPtr);
			}
		} else {//if its target store was the entrance, say that it left
			std::cout << "Robot R" << eventListHead->robotID + 1 << " left the RoboMall at time " << eventListHead->arrivalTime << std::endl; 
			eventListHead = eventListHead->robotLink;
		}
	}
}
