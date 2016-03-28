/*eventListShopperFunctions*/
/*eventListFunctions*/
#include "globals.h"//CURRENTLY FOR ROBITS

//processing event: after each item obtained, check if the event is now done. if it is, find the next waiting item and all other items waiting at store
//then move their arrival time forward and re-add them with remaining timeSlice time.

//Written by Qiaoyu Liao
EventListShopper::EventListShopper(){
	eventListHead = NULL;
}

void EventListShopper::setEventListHead(ShopperLink given){
	eventListHead = given;
}


ShopperLink EventListShopper::getEventListHead(){
	return eventListHead;
}

void EventListShopper::addToList(ShopperLink given){
	ShopperLink testPrvPtr;
	ShopperLink testCurPtr;
	testPrvPtr = NULL;
	testCurPtr = eventListHead;
	given->shopperLink = NULL;
	int existsAlready = 0;
	while (testCurPtr != NULL){ // check in case there are copies of this shopper
		if (testCurPtr->shopperID == given->shopperID){
			existsAlready = 1;
		}
		testPrvPtr = testCurPtr;
		testCurPtr = testCurPtr->shopperLink;
	}

	ShopperLink previousPtr;
	ShopperLink currentPtr;
	
	previousPtr = NULL;
	currentPtr = eventListHead;
	if (currentPtr == NULL){ //Place the shopper in the event list, deleting copies
		setEventListHead(given);
	} else {
		while( currentPtr != NULL && 
			((given->arrivalTime > currentPtr->arrivalTime) 
			|| (given->arrivalTime == currentPtr->arrivalTime && (given->shopperID > currentPtr->shopperID)))){
				if (currentPtr->shopperID == given->shopperID){
					if (previousPtr != NULL){
						previousPtr->shopperLink = currentPtr->shopperLink;
						currentPtr = currentPtr->shopperLink;
					} else if (previousPtr == NULL) {
						setEventListHead(currentPtr->shopperLink);
					}
				} else {
					previousPtr = currentPtr;
					if (currentPtr->shopperLink != NULL){
						currentPtr = currentPtr->shopperLink;
					} else {
						currentPtr = NULL;
					}
				}
		}

		if (previousPtr != NULL && existsAlready != 1){
			previousPtr->shopperLink = given;
			given->shopperLink = currentPtr;
		} else if (existsAlready != 1) {
			eventListHead = given;
			given->shopperLink = currentPtr;
		}
		
	}
	return;
}


//This code is written by Brian Rubenstein
void EventListShopper::processEventList(Tree rootTree){//ERROR WITH THIS FUNCTION IS THAT IT IS RECEIVING POINTERS FOR INFORMATION WITHIN THE EVENTLISTHEAD
	//ShopperLink tmpPtr = eventListHead;//take the next thing off the list
	//eventListHead = eventListHead->shopperLink; //move the event queue to the next element
	int currentTime = eventListHead->arrivalTime; //initialize the current time
	int i = 0;
	switch(eventListHead->State){ //Check where the shopper is:
	case entering: //Shopper is entering the RoboMall
		std::cout << "Shopper S" << eventListHead->shopperID << " enters the RoboMall at time " << currentTime << std::endl; //output
		
		for (i = 0; i < eventListHead->expectedItems; i++){ //check how many items this shopper is expecting: create an item for each one
			if (eventListHead->shoppingList != NULL){
				store *location = new store;
				if (location == NULL){
					std::cout << "No memory available" << endl;
					return;
				}


				location = rootTree.findItem(eventListHead->shoppingList); //Find item in tree of items
				eventListHead->shoppingList = eventListHead->shoppingList->itemLink; //set the remove the item found from list of items
				
				int storeInList = 0;
				StoreLink previousPtr;
				StoreLink currentPtr;
				ItemLink previousItemPtr;
				ItemLink currentItemPtr;
				previousPtr = NULL;
				currentPtr = eventListHead->targetStore;
				while (currentPtr != NULL && location != NULL){ //check if the store found is already being gone to by the shopper
					if ((currentPtr->xPos == location->xPos && currentPtr->yPos == location->yPos && currentPtr->zPos == location->zPos) || storeInList == 1){
						if (storeInList == 0){ //if the target store is in the list, add the shopper's next item on it's list of items at that store
							storeInList = 1; //prevents duplicate adding
							previousItemPtr = NULL;
							currentItemPtr = currentPtr->targetItem;
							while(currentItemPtr != NULL){
								previousItemPtr = currentItemPtr;
								currentItemPtr = currentItemPtr->itemLink;
							}
							if (previousItemPtr == NULL){
								currentPtr->targetItem = location->targetItem;
							}else{
								previousItemPtr->itemLink = location->targetItem;
							}
						}
						previousPtr = currentPtr;
						currentPtr = currentPtr->storeLink;
						
					} else {
						previousPtr = currentPtr;
						currentPtr = currentPtr->storeLink;
					}
				}

				if (storeInList == 0 && location != NULL){ //if it isn't in the list, add the store found
					if (previousPtr == NULL){
						eventListHead->targetStore = location;
					} else if (location != NULL) {
						previousPtr->storeLink = location;
					}
				}
			}
		}
		eventListHead->State = arriving; //Set the shopper's state to arriving: and add it back to the list
		ShopperLink tmpPtr;
		tmpPtr = eventListHead;//take the next thing off the list
		eventListHead = eventListHead->shopperLink; //move the event queue to the next element
		addToList(tmpPtr);
		break;
	case arriving: //Shopper arrives at their target store
		
		if (eventListHead->targetStore == NULL){ //if it is done with all its stores
			store *entrance = new store;
			if (entrance == NULL){
				std::cout << "No memory available" << endl;
				return;
			} else { //send it to the entrance
				entrance->xPos = 8;
				entrance->yPos = 16;
				entrance->zPos = 0;
				entrance->targetItem = NULL;
				entrance->targetItemNum = 0;
			}
			eventListHead->targetStore = entrance;
		}

		if (eventListHead->xPos != eventListHead->targetStore->xPos || eventListHead->yPos != eventListHead->targetStore->yPos || eventListHead->zPos != eventListHead->targetStore->zPos){ //if it is not at its target store
			R[0] = eventListHead->xPos;
			R[1] = eventListHead->yPos;
			R[2] = eventListHead->zPos;
			S[0] = eventListHead->targetStore->xPos;
			S[1] = eventListHead->targetStore->yPos;
			S[2] = eventListHead->targetStore->zPos;
			
			if (eventListHead->xPos != 8 || eventListHead->yPos != 16 || eventListHead->zPos != 0){
				std::cout << "Shopper S" << eventListHead->shopperID << " leaves store " << eventListHead->targetStoreNum <<"(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << " Floor at time " << eventListHead->arrivalTime << std::endl;
				eventListHead->targetStoreNum += 1;//set the shopper to looking for it's next store
			}
			eventListHead->arrivalTime += travel(R, S, SIZE); //travel to the target store
			

			eventListHead->xPos = S[0]; //set its position to its target store
			eventListHead->yPos = S[1];
			eventListHead->zPos = S[2];
			ShopperLink tmpPtr = eventListHead; //take the next thing off the list
			eventListHead = eventListHead->shopperLink; //move the event queue to the next element
			addToList(tmpPtr); //add it back to the list
		} 
		else if (eventListHead->xPos == 8 && eventListHead->yPos == 16 && eventListHead->zPos == 0){ //IF THE SHOPPER IS DONE COMPLETELY
			std::cout << "Shopper S" << eventListHead->shopperID << " left the RoboMall at time " << currentTime << std::endl; //say that they left and remove them from the event list
			eventListHead = eventListHead->shopperLink;
		} else {//IF THE SHOPPER IS AT THEIR STORE
			
			ShopperLink previousPtr;
			ShopperLink currentPtr;
			ShopperLink tmpSPtr;
			previousPtr = NULL;
			tmpSPtr = NULL;
			if (eventListHead->shopperLink != NULL){
				while (eventListHead->shopperLink->shopperID == eventListHead->shopperID && eventListHead->shopperLink != NULL){
					eventListHead->shopperLink = eventListHead->shopperLink->shopperLink;
				}
				currentPtr = eventListHead->shopperLink;
			} else {
				currentPtr = NULL;
			}
			int counter = 0;
			while(currentPtr != NULL){ //check if there is another shopper being handled by the clerk or waiting at the store
				if (currentPtr->xPos == eventListHead->xPos && 
					currentPtr->yPos == eventListHead->yPos && 
					currentPtr->zPos == eventListHead->zPos && 
					(currentPtr->State == processing || currentPtr->State == waiting)){
					tmpSPtr = currentPtr; // if there is, find the last one in the event list
				}
				previousPtr = currentPtr;
				currentPtr = currentPtr->shopperLink;
			}
			if (tmpSPtr == NULL){ // if there is no one waiting or being handled at the store
				std::cout << "shopper S" << eventListHead->shopperID << " arrives at store " << eventListHead->targetStoreNum << "(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << " Floor at time " << currentTime << std::endl;
				eventListHead->State = processing; //set the shopper to being handled
				processEvent(rootTree);
			} else if (tmpSPtr->State == waiting){// if there are other things waiting at the store
				std::cout << "shopper S" << eventListHead->shopperID << " arrives at store " << eventListHead->targetStoreNum << "(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << " Floor at time " << currentTime << std::endl;
				if (eventListHead->zPos == 1){ //if second floor, set its arrival time to be timeSlice units AFTER the last waiting shopper
					eventListHead->arrivalTime = tmpSPtr->arrivalTime + timeSlice;
				} else {//otherwise, if there are other things waiting on the first floor
					ItemLink previousItemPtr;
					ItemLink currentItemPtr;
					previousItemPtr = NULL;
					currentItemPtr = tmpSPtr->targetStore->targetItem;
					while(currentItemPtr != NULL){ //find how many items the last waiting shopper has
						counter += currentItemPtr->count;
						previousItemPtr = currentItemPtr;
						currentItemPtr = currentItemPtr->itemLink;
					}
					eventListHead->arrivalTime = tmpSPtr->arrivalTime + counter; //set the shopper's arrival time to the waiting shopper's arrival time plus the amount of items they have
				}
				eventListHead->State = waiting; //set the shopper's state to waiting
				ShopperLink tmpPtr = eventListHead;//take the next thing off the list
				eventListHead = eventListHead->shopperLink; //move the event queue to the next element
				addToList(tmpPtr);
			} else {
				std::cout << "shopper S" << eventListHead->shopperID << " arrives at store " << eventListHead->targetStoreNum << "(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << " Floor at time " << currentTime << std::endl;
				ShopperLink tmpPtr = eventListHead;//take the next thing off the list
				eventListHead = eventListHead->shopperLink; //move the event queue to the next element
				tmpPtr->shopperLink = tmpSPtr->shopperLink;
				tmpSPtr->shopperLink = tmpPtr;
				tmpPtr->arrivalTime = tmpSPtr->arrivalTime;
				tmpPtr->State = waiting;
				addToList(tmpPtr);
			}
		}

		break;
	case waiting: //shopper is waiting for another shopper to finish at the store and rearrived
		processEvent(rootTree); //process the shopper
		break;
	case processing: //shopper just finished one time unit at the store
		ShopperLink previousPtr;
		ShopperLink currentPtr;
		ShopperLink tmpSPtr;
		previousPtr = NULL;
		currentPtr = eventListHead->shopperLink;
		while(currentPtr != NULL){ //check if there is anything waiting at the store
			if (currentPtr->xPos == tmpPtr->xPos && 
				currentPtr->yPos == tmpPtr->yPos && 
				currentPtr->zPos == tmpPtr->zPos && 
				(currentPtr->State == processing || currentPtr->State == waiting)){
				tmpSPtr = currentPtr;
			}
			previousPtr = currentPtr;
			currentPtr = currentPtr->shopperLink;
		}
		if(tmpSPtr == NULL){//if there isn't, just keep processing
			processEvent(rootTree);
		} else {//if there is
			int counter = 0;
			if (eventListHead->zPos == 1){//set the shopper timeSlice units after the last one, if they are on the second floor
				eventListHead->arrivalTime = tmpSPtr->arrivalTime + timeSlice;
			} else {//otherwise set the shopper after all the waiting shopper's items have processed (this should never happen, as it is a FCFS queue)
				ItemLink previousItemPtr;
				ItemLink currentItemPtr;
				previousItemPtr = NULL;
				currentItemPtr = tmpSPtr->targetStore->targetItem;
				while(currentItemPtr != NULL){
					counter += currentItemPtr->count;
					previousItemPtr = currentItemPtr;
					currentItemPtr = currentItemPtr->itemLink;
				}
				eventListHead->arrivalTime = tmpSPtr->arrivalTime + counter;
			}
			eventListHead->State = waiting; //set the shopper's state to waiting
			eventListHead = eventListHead->shopperLink; //move the event queue to the next element
			tmpPtr->shopperLink = tmpSPtr->shopperLink;
			addToList(tmpPtr);//add back to the list
		}
		break;
	default: //if the shopper doesn't have one of these four states: end the program with an error.
		std::cout << "Not a valid state for shopper" << std::endl;
		exit(EXIT_FAILURE);
	}
}

//This code is written by Brian Rubenstein
void EventListShopper::processEvent(Tree rootTree){
	int processing = 0; //variable for processing one time unit at a time
	int counter = 0; //counter to find all the items
	int finished  = 0;//variable to check if the shopper is done at the store

	if (eventListHead->zPos == 1){//if on the second floor, find how many items the shopper has
		ItemLink previousEItemPtr;
		ItemLink currentEItemPtr;
		previousEItemPtr = NULL;
		currentEItemPtr = eventListHead->targetStore->targetItem;
		while(currentEItemPtr != NULL){
			counter += currentEItemPtr->count;
			previousEItemPtr = currentEItemPtr;
			currentEItemPtr = currentEItemPtr->itemLink;
		}
		processing = MINMAC(timeSlice, counter + 1);//then make processing the minimum of either timeSlice, or counter + 1 (one unit for finishing the shopper)
	
	} else {//if on the first floor, find how many items the shopper has.
		ItemLink previousEItemPtr;
		ItemLink currentEItemPtr;
		previousEItemPtr = NULL;
		currentEItemPtr = eventListHead->targetStore->targetItem;
		while(currentEItemPtr != NULL){ 
			counter += currentEItemPtr->count;
			previousEItemPtr = currentEItemPtr;
			currentEItemPtr = currentEItemPtr->itemLink;
		}
		processing = counter + 1;//set processing equal to counter + 1 (one unit for finishing the shopper)
	}

	ShopperLink newHead; //create a temporary Shopper to hold the event list head (this is safety checking)
	newHead = eventListHead;
	int hasBought = 0; //variable to check if an item has been fully purchased in this processing cycle
	for (processing = processing; processing > 0; processing--){ //continue processing as long as there are items to go, decrementing processing each time
		if (eventListHead->targetStore->targetItem == NULL){ //check if the shopper has no more items to get from this store
			eventListHead->State = arriving;//set its state to arriving
			finished = 1;//set the finished variable to one
			ShopperLink temporaryPointer;
			ShopperLink previousPtr;
			ShopperLink currentPtr;
			previousPtr = eventListHead;
			currentPtr = eventListHead->shopperLink;
			while(currentPtr != NULL){
				if (currentPtr->targetStore != NULL){//set all other shoppers waiting for the store forward processing time in the queue, and then re-add them to the event list
					if (currentPtr->targetStore->xPos == eventListHead->targetStore->xPos && currentPtr->targetStore->yPos == eventListHead->targetStore->yPos && currentPtr->targetStore->zPos == eventListHead->targetStore->zPos && currentPtr->State == waiting){
						currentPtr->arrivalTime -= processing;
						if (previousPtr == NULL){//if it is the first thing in the eventList (SHOULD NEVER HAPPEN, ON EVENTLISTHEAD)
							eventListHead = currentPtr->shopperLink;//set the eventList head to be it's link
							addToList(currentPtr);//add it back to the list
							previousPtr = currentPtr;
							currentPtr = currentPtr->shopperLink;
						} else {
							previousPtr->shopperLink = currentPtr->shopperLink;//otherwise, make the previous shopper point to the next one, and re-add the shopper
							temporaryPointer = currentPtr;
							currentPtr = previousPtr->shopperLink;
							addToList(temporaryPointer);
							
						}
					} else {
						previousPtr = currentPtr;
						currentPtr = currentPtr->shopperLink;
					}
				} else { //end if
					previousPtr = currentPtr;
					currentPtr = currentPtr->shopperLink;
				}
				
			}//end while
			//std::cout << "Shopper S" << newHead->shopperID << " leaves store " << newHead->targetStoreNum <<"(" << newHead->xPos << "," << newHead->yPos << ") on the " << newHead->zPos + 1 << " Floor at time " << newHead->arrivalTime << std::endl;
			
			newHead->targetStore = newHead->targetStore->storeLink; //tell the shopper to look for its next store
			eventListHead = eventListHead->shopperLink;
			addToList(newHead); //add it back to the list
		} else {
			newHead->targetStore->targetItem->count -= 1; //if it is not done, decrease its count of items required by one
			newHead->itemsObtained += 1; // increase the amount of items it has obtained by one
			newHead->arrivalTime +=1; // increase its arrival time by one
			if (eventListHead->targetStore->targetItem->count == 0){ //if it is out of this item to buy
				if (hasBought == 0){ //if it hasn't bought anything this cycle
					std::cout << "Shopper S" << eventListHead->shopperID << " bought the following items at store (" << eventListHead->xPos << "," << eventListHead->yPos << "):" <<std::endl;
					hasBought = 1;//set its hasBought variable to one
				}
				std::cout << "	Item name: " << eventListHead->targetStore->targetItem->name << "	Quantity obtained: " << eventListHead->itemsObtained << std::endl;
				eventListHead->itemsObtained = 0; //set its items obtained to 0
				eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink; //look at the next item
			}
		}
	}
	if (finished == 0){ //if it is not done yet
		ShopperLink previousSPtr;
		ShopperLink currentSPtr;
		ShopperLink tmpSPtr;
		previousSPtr = NULL;
		currentSPtr = eventListHead->shopperLink;
		while(currentSPtr != NULL){//check if there is anything waiting at store
			if (currentSPtr->xPos == eventListHead->xPos && 
				currentSPtr->yPos == eventListHead->yPos && 
				currentSPtr->zPos == eventListHead->zPos && 
				(currentSPtr->State == processing || currentSPtr->State == waiting || (currentSPtr->State == arriving && currentSPtr->arrivalTime <= eventListHead->arrivalTime + processing))){
				tmpSPtr = currentSPtr;
			}
			previousSPtr = currentSPtr;
			currentSPtr = currentSPtr->shopperLink;
			
		}
		if(tmpSPtr == NULL){ //if there is not
			ShopperLink tmpPtr = eventListHead;//take the next thing off the list
			eventListHead = eventListHead->shopperLink; //move the event queue to the next element
			addToList(tmpPtr); //add it back to the eventList
		} else { //if there is
			if(eventListHead->zPos == 1 && (tmpSPtr->arrivalTime + timeSlice >= eventListHead->arrivalTime)){ //add it after the last shopper, timeSlice units if it is on second floor
				eventListHead->arrivalTime = tmpSPtr->arrivalTime + timeSlice;
			} else if (eventListHead->zPos == 0) {//if it is on first floor, find the amount of items the last waiting thing has, and add the shopper that much time after it
				ItemLink previousItemPtr;
				ItemLink currentItemPtr;
				previousItemPtr = NULL;
				if (tmpSPtr->targetStore != NULL){
					currentItemPtr = tmpSPtr->targetStore->targetItem;
				} else {
					currentItemPtr = NULL;
				}
				counter = 0;
				while(currentItemPtr != NULL){
					counter += currentItemPtr->count;
					previousItemPtr = currentItemPtr;
					currentItemPtr = currentItemPtr->itemLink;
				} if (tmpSPtr->arrivalTime + counter > eventListHead->arrivalTime){
					eventListHead->arrivalTime = tmpSPtr->arrivalTime + counter;
				}
			}
			eventListHead->State = waiting; //set its state to waiting
			ShopperLink tmpPtr = eventListHead;//take the next thing off the list
			eventListHead = eventListHead->shopperLink; //move the event queue to the next element
			addToList(tmpPtr); //re-add the shopper
		}
	}
}

