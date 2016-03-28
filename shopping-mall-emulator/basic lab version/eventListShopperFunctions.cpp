/*eventListShopperFunctions*/
/*eventListFunctions*/
#include "globals.h"//CURRENTLY FOR ROBITS

//processing event: after each item obtained, check if the event is now done. if it is, find the next waiting item and all other items waiting at store
//then move their arrival time forward and re-add them with remaining timeSlice time.

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
	
	given->shopperLink = NULL;
	ShopperLink previousPtr;
	ShopperLink currentPtr;
	
	previousPtr = NULL;
	currentPtr = eventListHead;
	if (currentPtr == NULL){
		setEventListHead(given);
	} else {
		//std::cout << currentPtr->xPos << std::endl;
		while( currentPtr != NULL&& 
				((given->arrivalTime > currentPtr->arrivalTime) 
				|| 
				(given->arrivalTime == currentPtr->arrivalTime && (given->shopperID > currentPtr->shopperID)))){
			previousPtr = currentPtr;
			if (currentPtr->shopperLink != NULL){ //LOCATION OF THE ERROR
				currentPtr = currentPtr->shopperLink; //ROBOT POINTS AT JUNK
			} else {
				currentPtr = NULL;
			}
		}

		if (previousPtr != NULL){
			previousPtr->shopperLink = given;
			given->shopperLink = currentPtr;
		} else {
			eventListHead = given;
			given->shopperLink = currentPtr;
		}
		
	}
	return;
}



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
						if (storeInList == 0){ //if it is in the list, add the shopper's list of items at that store
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

				//IF LOCATION IS A NULL POINTER, IT DOESN'T MATTER IN THIS CASE
				if (storeInList == 0){ //if it isn't in the list, add the store found
					if (previousPtr == NULL){
						eventListHead->targetStore = location;
					} else {
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
			eventListHead->arrivalTime += travel(R, S, SIZE); //travel to the target store
			

			eventListHead->xPos = S[0]; //set its position to its target store
			eventListHead->yPos = S[1];
			eventListHead->zPos = S[2];
			ShopperLink tmpPtr = eventListHead; //take the next thing off the list
			eventListHead = eventListHead->shopperLink; //move the event queue to the next element
			addToList(tmpPtr); //add it back to the list
		} else if (eventListHead->xPos == 8 && eventListHead->yPos == 16 && eventListHead->zPos == 0){ //IF THE SHOPPER IS DONE COMPLETELY
			std::cout << "Shopper S" << eventListHead->shopperID << " left the RoboMall at time " << currentTime << std::endl; //say that they left and remove them from the event list
			eventListHead = eventListHead->shopperLink;
		} else {//IF THE SHOPPER IS AT THEIR STORE
			ShopperLink previousPtr;
			ShopperLink currentPtr;
			ShopperLink tmpSPtr;
			previousPtr = NULL;
			currentPtr = eventListHead->shopperLink;
			tmpSPtr = NULL;
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
				if(eventListHead->zPos == 1){
					eventListHead->arrivalTime += timeSlice; //if it is on the second floor, increase its arrival time by the timeSlice (for round robin)
				} else { //if it is on the first floor
					ItemLink previousItemPtr;
					ItemLink currentItemPtr;
					previousItemPtr = NULL;
					currentItemPtr = eventListHead->targetStore->targetItem;
					while(currentItemPtr != NULL){//find amount of items shopper has
						counter += currentItemPtr->count;
						previousItemPtr = currentItemPtr;
						currentItemPtr = currentItemPtr->itemLink;
					}

					eventListHead->arrivalTime += counter;//increase its arrival time by the amount of items it has (FCFS)
				}
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
	case waiting: //shopper is waiting for another shopper to finish at the store
		processEvent(rootTree); //process
		break;
	case processing: //shopper just finished one time unit at the store
		ShopperLink previousPtr;
		ShopperLink currentPtr;
		ShopperLink tmpSPtr;
		previousPtr = NULL;
		currentPtr = eventListHead->shopperLink;
		while(currentPtr != NULL){
			if (currentPtr->xPos == tmpPtr->xPos && 
				currentPtr->yPos == tmpPtr->yPos && 
				currentPtr->zPos == tmpPtr->zPos && 
				(currentPtr->State == processing || currentPtr->State == waiting)){
				tmpSPtr = currentPtr;
			}
			previousPtr = currentPtr;
			currentPtr = currentPtr->shopperLink;
		}
		if(tmpSPtr == NULL){
			processEvent(rootTree);
		} else {
			int counter = 0;
			if (eventListHead->zPos == 1){
				//eventListHead->arrivalTime = tmpSPtr->arrivalTime + timeSlice;
			} else {
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
			eventListHead->State = waiting;
			eventListHead = eventListHead->shopperLink; //move the event queue to the next element
			tmpPtr->shopperLink = tmpSPtr->shopperLink;
			addToList(tmpPtr);
		}
		break;
	default:
		exit(EXIT_FAILURE);
	}
}

void EventListShopper::processEvent(Tree rootTree){
	
	int processing = 0;
	int counter = 0;
	int finished  = 0;
	if (eventListHead->zPos == 1){// On the second floor, RR
		ItemLink previousEItemPtr;
		ItemLink currentEItemPtr;
		previousEItemPtr = NULL;
		currentEItemPtr = eventListHead->targetStore->targetItem;
		while(currentEItemPtr != NULL){ // counter getting wrong number: currentEItemPtr's item link should not be NULL
			counter += currentEItemPtr->count;
			previousEItemPtr = currentEItemPtr;
			currentEItemPtr = currentEItemPtr->itemLink;
		}
		processing = MINMAC(timeSlice, counter + 1);
	} else {
		ItemLink previousEItemPtr;
		ItemLink currentEItemPtr;
		previousEItemPtr = NULL;
		currentEItemPtr = eventListHead->targetStore->targetItem;
		while(currentEItemPtr != NULL){ // counter getting wrong number: currentEItemPtr's item link should not be NULL
			counter += currentEItemPtr->count;
			previousEItemPtr = currentEItemPtr;
			currentEItemPtr = currentEItemPtr->itemLink;
		}
		if (eventListHead->xPos == 8 && eventListHead->yPos == 16){
			processing = counter;
		} else {
			processing = counter + 1;
		}
	}
	ShopperLink newHead;
	newHead = eventListHead;
	int initialProcessing = processing;
	int hasBought = 0;
	for (processing = processing; processing > 0; processing--){
		//std::cout << "Process number: " << processing << std::endl;
		if (eventListHead->targetStore->targetItem == NULL){
			eventListHead->State = arriving;
			finished = 1;
			ShopperLink temporaryPointer;
			ShopperLink previousPtr;
			ShopperLink currentPtr;
			previousPtr = NULL;
			currentPtr = eventListHead->shopperLink;
			while(currentPtr != NULL){
				if (currentPtr->targetStore != NULL){
					if (currentPtr->targetStore->xPos == eventListHead->targetStore->xPos && currentPtr->targetStore->yPos == eventListHead->targetStore->yPos && currentPtr->targetStore->zPos == eventListHead->targetStore->zPos && currentPtr->State == waiting){
					
						currentPtr->arrivalTime -= processing;
						if (previousPtr == NULL){
							eventListHead = currentPtr->shopperLink;
							addToList(currentPtr);
							previousPtr = currentPtr;
							currentPtr = currentPtr->shopperLink;
						} else {
							previousPtr->shopperLink = currentPtr->shopperLink;
							temporaryPointer = currentPtr;
							currentPtr = previousPtr->shopperLink;
							addToList(temporaryPointer);
							
						}
					} else {
						previousPtr = currentPtr;
						currentPtr = currentPtr->shopperLink;
					}
				} else {
					previousPtr = currentPtr;
					currentPtr = currentPtr->shopperLink;
				}
				
			}
			std::cout << "Shopper S" << newHead->shopperID << " leaves store (" << newHead->xPos << "," << newHead->yPos << ") on the " << newHead->zPos + 1 << " Floor at time " << newHead->arrivalTime + (initialProcessing - processing) << std::endl;
			newHead->targetStore = newHead->targetStore->storeLink;
			eventListHead = eventListHead->shopperLink;
			addToList(newHead);
		} else {
			newHead->targetStore->targetItem->count -= 1;
			newHead->itemsObtained += 1;
			if (eventListHead->targetStore->targetItem->count == 0){
				if (hasBought == 0){
					std::cout << "Shopper S" << eventListHead->shopperID << " bought the following items at store (" << eventListHead->xPos << "," << eventListHead->yPos << "):" <<std::endl;
					hasBought = 1;
				}
				std::cout << "	Item name: " << eventListHead->targetStore->targetItem->name << "	Quantity obtained: " << eventListHead->itemsObtained << std::endl;
				eventListHead->itemsObtained = 0;
				eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
				
			}
		}
	}
	if (finished == 0){
		ShopperLink previousSPtr;
		ShopperLink currentSPtr;
		ShopperLink tmpSPtr;
		previousSPtr = NULL;
		currentSPtr = eventListHead->shopperLink;
		while(currentSPtr != NULL){
			
			if (currentSPtr->xPos == eventListHead->xPos && 
				currentSPtr->yPos == eventListHead->yPos && 
				currentSPtr->zPos == eventListHead->zPos && 
				(currentSPtr->State == processing || currentSPtr->State == waiting || (currentSPtr->State == arriving && currentSPtr->arrivalTime <= eventListHead->arrivalTime + processing))){
				tmpSPtr = currentSPtr;
			}
			previousSPtr = currentSPtr;
			currentSPtr = currentSPtr->shopperLink;
			
		}
		if(tmpSPtr == NULL){
			//eventListHead->arrivalTime += timeSlice;
			ShopperLink tmpPtr = eventListHead;//take the next thing off the list
			eventListHead = eventListHead->shopperLink; //move the event queue to the next element
			tmpPtr->arrivalTime += initialProcessing;
			addToList(tmpPtr);
		} else {
			if(eventListHead->zPos == 1){
				//eventListHead->arrivalTime = tmpSPtr->arrivalTime + timeSlice;
			} else {
				ItemLink previousItemPtr;
				ItemLink currentItemPtr;
				previousItemPtr = NULL;
				currentItemPtr = tmpSPtr->targetStore->targetItem;
				while(currentItemPtr != NULL){
					counter += currentItemPtr->count;
					previousItemPtr = currentItemPtr;
					currentItemPtr = currentItemPtr->itemLink;
				}
				
			}
			eventListHead->State = waiting;
			ShopperLink tmpPtr = eventListHead;//take the next thing off the list
			eventListHead = eventListHead->shopperLink; //move the event queue to the next element
			addToList(tmpPtr);
		}
	} else {
		newHead->arrivalTime = newHead->arrivalTime + initialProcessing;
	}
}

