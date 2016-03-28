/*initializeRobot is a function that builds a robot with input information*/
#include "globals.h"

//initializeRobot written by: Qiaoyu Liao
void initializeRobot(RobotLink given, int numberExpectStores){//
	given->xPos = 8; //set its inital position to the entrance
	given->yPos = 16;
	given->zPos = 0;
	given->targetStoreNum = 1; //set its first target store to number one
	given->targetStore = NULL; //make sure it doesn't have a target store yet
	int i = 0;//for loop variables
	int k = 0;
	StoreLink previousPtr;
	StoreLink currentPtr;
	StoreLink tmpPtr;
	given->targetStore = NULL;
	
	for (i = 0; i < numberExpectStores; i++){ //For every store value it expects
		previousPtr = NULL;
		currentPtr = given->targetStore;
		while (currentPtr != NULL){ //run through its store list until it hits null
			previousPtr = currentPtr;
			currentPtr = currentPtr->storeLink;
		}
		store *newStore = new store;

		if (newStore == NULL){
			std::cout << "No memory available" << std::endl;
			return;
		} else {
			tmpPtr = newStore;
		}
		std::cin >> tmpPtr->xPos >> tmpPtr->yPos >> tmpPtr->zPos >> tmpPtr->targetItemNum; //read in information for the store
		tmpPtr->targetItem = NULL;
		tmpPtr->storeLink = NULL;
		k = 0;
		for (k = 0; k < tmpPtr->targetItemNum; k++){ //ITERATE K TIMES WHERE K IS THE AMOUNT OF EXPECTED ITEMS
			ItemLink previousItemPtr;
			ItemLink currentItemPtr;
			previousItemPtr = NULL;//ITERATE THROUGH ALL STORE ITEMS
			currentItemPtr = tmpPtr->targetItem;
			ItemLink tmpItemPtr;
			tmpItemPtr = NULL;
			while(currentItemPtr != NULL){ //Create item list for each store
				previousItemPtr = currentItemPtr;
				currentItemPtr = currentItemPtr->itemLink;
			}
			item *newItem = new item;//Create item list for each store
			if (newItem == NULL){
				std::cout << "No memory available" << std::endl;
				return;
			} else {
				tmpItemPtr = newItem;//BUILD A NEW ITEM AND READ INFO
			}
			std::cin >> tmpItemPtr->name >> tmpItemPtr->count;
			tmpItemPtr->itemLink = NULL;
			if (tmpPtr->targetItem != NULL) {
				previousItemPtr->itemLink = tmpItemPtr;
			} else {
				tmpPtr->targetItem = tmpItemPtr;
			}
		} //END FOR LOOP



		if(previousPtr == NULL){//add it to the list
			given->targetStore = tmpPtr;
		} else {
			previousPtr->storeLink = tmpPtr;
		}
		
	}
	return;
}
