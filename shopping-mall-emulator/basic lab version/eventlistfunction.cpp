void EventListShopper::addToListI(ShopperLink given){
	
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

void EventListShopper::addToListP(ShopperLink given){
	
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
				(given->arrivalTime == currentPtr->arrivalTime && (given->State != processing || given->State != waiting))){
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



void Eventlistshopper::processEventList(Tree rootTree)
{
	switch(eventListHead->State)
	{

	case entering:
		std::cout << "Shopper S" << eventListHead->shopperID << " enters the RoboMall at time " << currentTime << std::endl;

		for (i = 0; i < eventListHead->expectedItems; i++)
		{ //check how many items this shopper is expecting: create an item for each one
			if (eventListHead->shoppingList != NULL)
			{
				
				store *location = new store;
				if (location == NULL)
				{
					std::cout << "No memory available" << endl;
					return;
				}

				location = rootTree.findItem(eventListHead->shoppingList);// find the first item in the list in tree

				if( location != NULL)
				{

					eventListHead->shoppingList = eventListHead->shoppingList->itemLink;// remove the head
					//NOTE: BRIAN DID CHECK IF THE STORE IS ALREADY ADDED, HOWEVER, IF IT IS ALREADY ADDED, THEN THE TIME
					//THE ROBOT STOP BY A STORE WILL BE LONGER

					//CHECK IF THE TARGET ITEM LIST ALREADY HAS THE STORE WE FOUND
					StoreLink previousSPtr = NULL;
					StoreLink currentSPtr = eventListHead->targetStore;

					while(currentPtr != NULL && location != NULL && 
						(currentPtr->xPos != location->xPos || currentPtr->yPos != location->yPos || currentPtr->zPos != location->zPos))
					{
						previousSPtr = currentSPtr;
						currentSPtr = currentSPtr->storeLink;
					}

					if(currentSPtr != NULL && location != NULL)// if we found the store in the store list
					{
						ItemLink previousIPtr = NULL;
						ItemLink currentIPtr = currentSPtr->targetItem;

						while(currentIPtr != NULL && currentIPtr->name != location->targetItem->name)// loop to find the item in the store
						{
							previousIPrr = currentIPtr;
							currentIPtr = currentIPtr->itemLink;
						}
							
						if(currentIPtr != NULL)// if found in the store
						{
							currentIPtr->count += location->targetItem->count;
							//CHANGE ITEM NUMBER OF ITEMATSTORE
							delete location;
						}

						else//not found in the store
						{// added to the item list
							item *newItem = new item;
							newItem->name = location->targetItem->name;
							newItem->count = location->targetItem->count;
							newItem->itemLink = NULL;

							if(previousIPtr == NULL)
							{
								currentSPtr->targetItem = newItem;
							}

							else//previousIPtr != NULL
							{
								previousIPtr->itemLink = newItem;
							}

							//ITEMATSTORE ADDED
						}

					}

					//DOESNT MATTER FOR LOCATION == NULL
					else // if not found the store in store list (currentSPtr == NULL)
					{
						if(previousSPtr == NULL && location != NULL)
						{
							eventListHead->storeLink = location;
						}

						else if (lcoation != NULL)
						{
						previousSPtr->storeLink = location;
						}

						//ITEMATSTORE ADDED
					}


				}//END IF LOCATION != NULL


			}
		}


		if (eventListHead->xPos != eventListHead->targetStore->xPos || eventListHead->yPos != eventListHead->targetStore->yPos || eventListHead->zPos != eventListHead->targetStore->zPos)
		{ //if it is not at its target store
			R[0] = eventListHead->xPos;
			R[1] = eventListHead->yPos;
			R[2] = eventListHead->zPos;
			S[0] = eventListHead->targetStore->xPos;
			S[1] = eventListHead->targetStore->yPos;
			S[2] = eventListHead->targetStore->zPos;
			eventListHead->arrivalTime += travel(R, S, SIZE); //travel to the target store
		}

		
		eventListHead->xPos = S[0]; //set its position to its target store
		eventListHead->yPos = S[1];
		eventListHead->zPos = S[2];

		eventListHead->State = arriving;
		ShopperLink tmpPtr = eventListHead;//take the next thing off the list
		eventListHead = eventListHead->shopperLink; //move the event queue to the next element
		//tmpPtr->shopperLink = NULL;       JUST FOR SAFETY     ALREADY CHECKED         IN THE FUNCTION
		addToListI(tmpPtr);

		break;


	case arriving:

		 if (eventListHead->xPos == 8 && eventListHead->yPos == 16 && eventListHead->zPos == 0)
		 { //IF THE SHOPPER IS DONE COMPLETELY
			std::cout << "Shopper S" << eventListHead->shopperID << " left the RoboMall at time " << currentTime << std::endl; //say that they left and remove them from the event list
			shopperLink tmpPtr = eventListHead;
			eventListHead = eventListHead->shopperLink;
			delete tmpPtr;
		 }

		 else//IF IT IS NOT ARRIVING AT THE EXIST, WHICH MEANS IT IS NOT DONE
		 {//GO THROUGHT THE SHOPPER LIST TO CHECK IF ANY SHOPPER IS AT THE CURRENT STORE WAITING OR PROCESSING
			ShopperLink previousPtr;
			ShopperLink currentPtr;
			ShopperLink tmpSPtr;
			previousPtr = NULL;
			currentPtr = eventListHead->shopperLink;
			tmpSPtr = NULL;
			int counter = 0;
			int maxArrivalTime = 0;

			while(currentPtr != NULL)
			{
				if(currentPtr->xPos == eventListHead->xPos && currentPtr->ypos == eventListHead->yPos &&
					currentPtr->zPos == eventListHead->zPos && (currentPtr->State == processing || currentPtr->State == waiting))
				{
					tmpSPtr = currentPtr;
				}

				previousPtr = currentPtr;
				currentPtr = currentPtr->shopperLink;
			}

			if (tmpSPtr == NULL)//IT ARRIVES WHEN THE QUEUE IS EMPTY
			{
				std::cout << "shopper S" << eventListHead->shopperID << " arrives at store " << eventListHead->targetStoreNum 
					<< "(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << 
					" Floor at time " << currentTime << std::endl;
				processEvent(rootTree);//PROCESS THROUGH IT AND ADDED BACK TO LIST
				// CAN BE HANDLED IN PROCESSEVENT 
				/*ShopperLink tmpPtr = eventListHead;
				eventListHead = eventListHead->shopperLink;
				addToListP(tmpPtr);*/
			}

			else if(tmpSPtr->State == processing || tmpSPTr->State == waiting) //IT ARRIVE WHEN SOMETHING IS IN THE TREE
			{
				std::cout << "shopper S" << eventListHead->shopperID << " arrives at store " << eventListHead->targetStoreNum 
					<< "(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << 
					" Floor at time " << currentTime << std::endl;
				ShopperLink tmpPtr = eventListHead;
				eventListHead = eventListHead->shopperLink;// move the eventlisthead to the next shopper
				tmpPtr->arrivalTime = tmpSPtr->arrivalTime + 1;// increment the time as one unit later than the previous shopper in the same waiting queue
				tmpPtr->State = waiting;// state change to waiting
				addToListP(tmpPtr);

			}
		/*	else if(tmpSPtr->State == waiting)
			{
				std::cout << "shopper S" << eventListHead->shopperID << " arrives at store " << eventListHead->targetStoreNum 
					<< "(" << eventListHead->xPos << "," << eventListHead->yPos << ") on the " << eventListHead->zPos + 1 << 
					" Floor at time " << currentTime << std::endl;
				ShopperLink tmpPtr = eventListHead;
				eventListHead = eventListHead->shopperlink;// move the eventlisthead to the next shopper
				tmpPtr->arrivalTime = tmpSPtr->arrivalTime + 1;// increment the time as one unit later than the previous shopper in the same waiting queue
				tmpPtr->State = waiting;// state change to waiting
				addToList(tmpPtr);

			}*/
			else // I HAVE NO IDEA WHAT'S WRONG WITH IT!!
			{
				std::cout << "something is wrong with it and I have no idea why!!!!" << std::endl;
			}
		 }
		 break;


	case waiting:

		ShopperLink previousPtr;
		ShopperLink currentPtr;
		ShopperLink tmpSPtr;
		previousPtr = NULL;
		currentPtr = eventListHead->shopperLink;
		tmpSPtr = NULL;
		int counter = 0;
		while(currentPtr != NULL){ //check if there is another shopper being handled by the clerk
			if (currentPtr->xPos == eventListHead->xPos && 
				currentPtr->yPos == eventListHead->yPos && 
				currentPtr->zPos == eventListHead->zPos && 
				currentPtr->State == processing ){
				tmpSPtr = currentPtr; // if there is, find the last one in the event list
			}
			previousPtr = currentPtr;
			currentPtr = currentPtr->shopperLink;
		}

		if (tmpSPtr == NULL)
		{
			processEvent(rootTree);//PROCESS THROUGH IT AND ADDED BACK TO LIST
			// CAN BE HANDLED IN PROCESSEVENT 	
			/*ShopperLink tmpPtr = eventListHead;
			eventListHead = eventListHead->shopperLink;
			addToListP(tmpPtr);*/
		}

		else// tmpSPtr != NULL
		{
			//HARD TO HANDLE
			int temp,increment;
			tmpPtr = eventListHead;
			eventListHead = eventListHead->shopperLink;
			temp = tmp->arrivalTime;
			tmpPtr->arrivalTime = tmpSPtr->arrivalTime + 1;
			increment = tmpPtr->arrivalTime - temp;
			addToListP(tmpPtr);

			ShopperLink previousPtr1;
			ShopperLink currentPtr1;
			previousPtr1 = NULL;
			currentPtr1 = eventListHead;

			while(currentPtr1 != NULL)
			{ //check if there is another shopper being handled by the clerk
				if (currentPtr1->xPos == tmpPtr->xPos && 
					currentPtr1->yPos == tmpPtr->yPos && 
					currentPtr1->zPos == tmpPtr->zPos && 
					currentPtr1->State == arriving )
				{
						shopperLink tempPtr = currentPtr1;
						previousPtr1->shopperLink = currentPtr1->shopperLink;//SKIP THE CURRENT POINTER
						currentPtr1 = currentPtr1->shopperLink;
						addToListP(tempPtr);
				}
				else if (currentPtr1->xPos == tmpPtr->xPos && 
					currentPtr1->yPos == tmpPtr->yPos && 
					currentPtr1->zPos == tmpPtr->zPos && 
					currentPtr1->robotID == tmpPtr->robotID )
				{
					break;
				}
				else
				{
				previousPtr1 = currentPtr1;
				currentPtr1 = currentPtr1->shopperLink;
				}
			}

		}

		break;

	case processing:// after all the process 

		if (eventListHead->targetStore == NULL)
		{ //if it is done with all its stores
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
			



		if (eventListHead->xPos != eventListHead->targetStore->xPos || eventListHead->yPos != eventListHead->targetStore->yPos || eventListHead->zPos != eventListHead->targetStore->zPos)
		{ //if it is not at its target store
			R[0] = eventListHead->xPos;
			R[1] = eventListHead->yPos;
			R[2] = eventListHead->zPos;
			S[0] = eventListHead->targetStore->xPos;
			S[1] = eventListHead->targetStore->yPos;
			S[2] = eventListHead->targetStore->zPos;
			eventListHead->arrivalTime += travel(R, S, SIZE); //travel to the target store
		}

		
		eventListHead->xPos = S[0]; //set its position to its target store
		eventListHead->yPos = S[1];
		eventListHead->zPos = S[2];

		eventListHead->State = arriving;
		ShopperLink tmpPtr = eventListHead;//take the next thing off the list
		eventListHead = eventListHead->shopperLink; //move the event queue to the next element
		//tmpPtr->shopperLink = NULL;       JUST FOR SAFETY     ALREADY CHECKED         IN THE FUNCTION
		addToListI(tmpPtr);
		break;

		default: //if the shopper doesn't have one of these four states: end the program with an error.
		std::cout << "Not a valid state for shopper" << std::endl;
		exit(EXIT_FAILURE);
	}
}


void EventListShopper::processEvent(Tree rootTree)
{
	int timeS = timeSlice;//TIME SLICE LEFT

	if (eventListHead->zPos ==1)// RR
	{
		while( timeS != 0 && eventListHead->targetStore->targetItem != NULL)
		{
			int left = rootTree.checkStorage(eventListHead->targetStore->targetItem->name,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);

			if(left >= eventListHead->targetStore->targetItem->count)
			{
				if(eventListHead->targetStore->targetItem->count > timeS)// TIME SLICE IS SMALLER THAN COUNT
				{
					rootTree.remove(eventListHead->targetStore->targetItem->name,timeS,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);
					eventListHead->targetStore->targetItem->count -= timeS;
					eventListHead->arrivalTime += timeS;
					std::cout << "Robot" << eventListHead->robotID << " get " << timeS << " item" << eventListHead->targetStore->targetItem->name << " using up time slice and added back to the round robin queue" << std::endl;
					timeS = 0;

					if(eventListHead->targetStore->targetItem->itemLink == NULL){
						eventListHead->targetStore = eventListHead->targetStore->storeLink;
					}

					eventListHead->State = processing;
					shopperLink tmpPtr = eventListHead;
					eventListHead = EventListHead->shopperLink;
					addToListP(tmpPtr);
				}

				else if(eventListHead->targetStore->targetItem->count = timeS)// TIME SLICE EQUAL TO COUNT
				{
					rootTree.remove(eventListHead->targetStore->targetItem->name,timeS,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);
					ItemLink tempPtr = eventListHead->targetStore->targetItem;
					eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
					delete tempPtr;
					eventListHead->arrivalTime += timeS;
					std::cout << "Robot" << eventListHead->robotID << " get " << timeS << " item" << eventListHead->targetStore->targetItem->name << " using up time slice and added back to the round robin queue" << std::endl;
					timeS = 0;

					if(eventListHead->targetStore->targetItem->itemLink == NULL){
						eventListHead->targetStore = eventListHead->targetStore->storeLink;
					}

					eventListHead->State = processing;
					shopperLink tmpPtr = eventListHead;
					eventListHead = EventListHead->shopperLink;
					addToListP(tmpPtr);
				}

				else//TIME SLICE BIGGER THAN COUNT
				{
					rootTree.remove(eventListHead->targetStore->targetItem->name,eventListHead->targetStore->targetItem->count,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);
					std::cout << "Robot" << eventListHead->robotID << " get " << eventListHead->targetStore->targetItem->count << " item" << eventListHead->targetStore->targetItem->name << "not using up slice" << std::endl;
					timeS -= eventListHead->targetStore->targetItem->count;		
					eventListHead->arrivalTime += eventListHead->targetStore->targetItem->count;
					ItemLink tempPtr = eventListHead->targetStore->targetItem;
					eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
					delete tempPtr;
				}
			}

			else if(left < eventListHead->targetStore->targetItem->count && left != 0)
			{
				if(left > timeS)// TIME SLICE IS SMALLER THAN LEFT
				{
					rootTree.remove(eventListHead->targetStore->targetItem->name,timeS,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);
					eventListHead->targetStore->targetItem->count -= timeS;
					eventListHead->arrivalTime += timeS;
					std::cout << "Robot" << eventListHead->robotID << " get " << timeS << " item" << eventListHead->targetStore->targetItem->name << " using up time slice and added back to the round robin queue" << std::endl;
					timeS = 0;
					eventListHead->State = arriving;
					shopperLink tmpPtr = eventListHead;
					eventListHead = EventListHead->shopperLink;
					addToListP(tmpPtr);
				}

				// START FROM HERE, DIFFERNETIATE THE CIRCUMENTANCES IN RR QUEUE AND 
				// COMPARE THE NUMBER LEFT IN TREE, NUMBER NEEDED


				else if(left = timeS)// TIME SLICE EQUAL TO LEFT
				{
					rootTree.remove(eventListHead->targetStore->targetItem->name,timeS,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);
					ItemLink tempPtr = eventListHead->targetStore->targetItem;// move to next item becasue we don't have enough item in the store
					eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
					delete tempPtr;
					eventListHead->arrivalTime += timeS;
					std::cout << "Robot" << eventListHead->robotID << " only get " << timeS << " item" << eventListHead->targetStore->targetItem->name << " using up time slice and added back to the round robin queue" << std::endl;
					timeS = 0;
					eventListHead->State = arriving;
					shopperLink tmpPtr = eventListHead;
					eventListHead = EventListHead->shopperLink;//next shopper 
					addToListP(tmpPtr);
				}

				else//TIME SLICE BIGGER THAN LEFT
				{
					rootTree.remove(eventListHead->targetStore->targetItem->name,left,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);
					std::cout << "Robot" << eventListHead->robotID << " get " << eventListHead->targetStore->targetItem->count << " item" << eventListHead->targetStore->targetItem->name << "not using up slice" << std::endl;
					timeS -= left; // not enought item in the store, move to next item in the store's list
					eventListHead->arrivalTime += left;
					ItemLink tempPtr = eventListHead->targetStore->targetItem;//traverse next
					eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
					delete tempPtr;
				}
			}

			else// WHEN LEFT = 0
			{
				std::cout << "nothing left"<< std::endl;
				ItemLink tempPtr = eventListHead->targetStore->targetItem;// traverse next 
				eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
				delete tempPtr;

			}

		}
	}

	else// zPos == 0   FCFS
	{
     
		while( eventListHead->targetStore->targetItem != NULL)
		{
			int left = rootTree.checkStorage(eventListHead->targetStore->targetItem->name,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);

			if(left >= eventListHead->targetStore->targetItem->count)
			{
				rootTree.remove(eventListHead->targetStore->targetItem->name,eventListHead->targetStore->targetItem->count,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);
				std::cout << "Robot" << eventListHead->robotID << " get " << eventListHead->targetStore->targetItem->count << " item" << eventListHead->targetStore->targetItem->name << std::endl;
				eventListHead->arrivalTime += eventListHead->targetStore->targetItem->count;
				ItemLink tempPtr = eventListHead->targetStore->targetItem;//traverse next
				eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
				delete tempPtr;
			}

			else if( left < eventListHead->targetStore->targetItem->count && left > 0 )
			{
				rootTree.remove(eventListHead->targetStore->targetItem->name,left,eventListHead->targetStore-xPos,eventListHead->targetStore-yPos,eventListHead->targetStore-zPos);
				std::cout << "Robot" << eventListHead->robotID << " get " << eventListHead->targetStore->targetItem->count << " item" << eventListHead->targetStore->targetItem->name << "not getting enough items" << std::endl;
				eventListHead->arrivalTime += left;
				ItemLink tempPtr = eventListHead->targetStore->targetItem;//traverse next
				eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
				delete tempPtr;
			}

			else //		LEFT == 0
			{
				std::cout << "Robot" << eventListHead->robotID << " get nothing from the store" << std::endl;
				ItemLink tempPtr = eventListHead->targetStore->targetItem;//traverse next
				eventListHead->targetStore->targetItem = eventListHead->targetStore->targetItem->itemLink;
				delete tempPtr;
			}
		}

		// 
		eventListHead->arrivalTime -= 1;
		eventListHead->State = processing;
		eventListHead->targetStore = eventListHead->targetStore->storeLink;// target store next
		tmpPtr = eventListHead;
		eventListHead = eventListHead->shopperLink;
		addToListP(tmpPr);

	}


}

