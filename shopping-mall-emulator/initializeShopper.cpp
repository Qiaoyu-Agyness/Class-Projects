/*initializeShopper is a function that builds a shopper with input information*/
#include "globals.h"

void initializeShopper(ShopperLink given){
	given->xPos = 8;//set its starting location to the entrance
	given->yPos = 16;
	given->zPos = 0;
	given->targetStoreNum = 1;//set its first target store to number one
	given->shoppingList = NULL; //make sure its shopping list is empty
	given->targetStore = NULL; // make sure it has no target stores
	given->State = entering; //set its state to be entering the mall
	given->itemsObtained = 0; //set its items obtained to be 0
	std::cin >> given->arrivalTime >> given->expectedItems; //read in its arrival time and how many items it expects

	int i = 0; // for loop variable
	ItemLink previousPtr;
	ItemLink currentPtr;
	ItemLink tmpPtr;
	for (i = 0; i < given->expectedItems; i++){ //as long as it expects another item
		previousPtr = NULL;
		currentPtr = given->shoppingList; // run through its shopping list until you hit the end
		while(currentPtr != NULL){
			previousPtr = currentPtr;
			currentPtr = currentPtr->itemLink;
		}
		item *newItem = new item;
		if (newItem == NULL){
			std::cout << "No memory available" << std::endl;
			return;
		} else {
			tmpPtr = newItem;
		}
		std::cin >> tmpPtr->name >> tmpPtr->count;//read in the name of the item and the number it is looking for.
		if(previousPtr == NULL){ // if the list is null
			given->shoppingList = tmpPtr; //make this item the head of the shopping list
		} else {
			previousPtr->itemLink = tmpPtr; //else add it to the end
		}
	}
}
