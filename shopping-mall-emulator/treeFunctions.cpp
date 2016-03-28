/*These are the functions which exist within the tree class.*/

#include "globals.h"
Tree::Tree(){
	basenode = NULL;
}


void Tree::insertTreenode(std::string givename, int count, int xposition, int yposition, int zposition){
	treenode *nullptr = NULL;
	// if the basenode points to null
	if (basenode == nullptr){		
		//create a new listnode
		listnode *newlist = new listnode;
		newlist->num = count;
		newlist->xpos = xposition;
		newlist->ypos = yposition;
		newlist->zpos = zposition;
		newlist->nextListnode = NULL;
		// create a new treenode
		basenode = new treenode;
		basenode->name = givename;
		basenode->list = newlist;
		basenode->leftTreenode = NULL;
		basenode->rightTreenode = NULL;
	}//end if

	else{

	char g1 = givename.at(0);
	char g2 = givename.at(1);
	char n1 = (basenode->name).at(0);
	char n2 = (basenode->name).at(1);


	// if item name is smaller than the item name of current node
	 if ( g1 < n1 || ( g1 == n1 && g2 < n2)){
			//insert the item into the left tree
			insertTreenode(givename,count,xposition,yposition,zposition,&(basenode->leftTreenode));
		}// end else if

	 // if item name is same to the item name of current node
	else if (g1 == n1 && g2 == n2){
	        //insert the item into current nodetree
			insertTreenode(givename,count,xposition,yposition,zposition,&basenode);
		}// end else if

	else {// if item name is larger than the item name of current node
			//insert the item into the right tree
			insertTreenode(givename,count,xposition,yposition,zposition,&(basenode->rightTreenode));
		}//end else
	}//end else

	return;
}//end function insertTreenode



void Tree::insertTreenode(std::string givename,int count, int xposition, int yposition, int zposition, TreenodePtr *leaf){
    
    treenode *nullptr = NULL;
    // if current leafptr points to null
    if ( (*leaf) == nullptr){
        //create a new treenode
        treenode *newnode = new treenode;
        //create a new listnode
        listnode *newlist = new listnode;
        //set the item number, xpos, ypos, zpos and next node in new listnode
        newlist->num = count;
        newlist->xpos = xposition;
        newlist->ypos = yposition;
        newlist->zpos = zposition;
        newlist->nextListnode = NULL;
        // set the new treenode
        newnode->name = givename;
        newnode->list = newlist;
        newnode->leftTreenode = NULL;
        newnode->rightTreenode = NULL;
        //leaf pointer points to the new treenode
        *leaf = newnode;
    }//end if

    
    
    // if the current leaf pointer points to sth that is not null
    else
    {
    char g1 = givename.at(0);
    char g2 = givename.at(1);
    char n1 = ((*leaf)->name).at(0);
    char n2 = ((*leaf)->name).at(1);

     // if the incoming item name is smaller than the current item name
     if( g1 < n1 || (g1 == n1 && g2 < n2) ){
        //insert the incoming item into the lefttree 
        insertTreenode(givename,count,xposition,yposition,zposition,&((*leaf)->leftTreenode));
    }//end if


     //if the incoming item name and current item name are the same
     else if( g1 == n1 && g2 == n2)
     {
         
                ListnodePtr previousPtr;
        ListnodePtr currentPtr;
        previousPtr = NULL;
        currentPtr = (*leaf)->list;
         
        // loop to find out if the store already has same product in storage
        while(currentPtr != NULL)
        {
            if (currentPtr->xpos != xposition || currentPtr->ypos != yposition || currentPtr->zpos != zposition)
            {
            previousPtr = currentPtr;
            currentPtr = currentPtr->nextListnode;
            }//end if
            else
            {
                break;
            }
        }//end while

         
        // if there's same product at the same store
        if( currentPtr != NULL)
        {
            if (previousPtr != NULL)
            {
                listnode *tmpPtr = new listnode;
                tmpPtr = currentPtr;
                previousPtr->nextListnode = currentPtr->nextListnode;
                // add count to the number of the item
                insertTreenode(givename,count + tmpPtr->num,xposition,yposition,zposition,leaf);
                delete tmpPtr;

            }
            
            else {
                  currentPtr->num += count;
            }
            
            
        }//end if

        // if did not found the same store position that has the item
        else
        {

        ListnodePtr previousPtr1;
        ListnodePtr currentPtr1;
        // new listnode 
        listnode *newlist = new listnode;
        newlist->num = count;
        newlist->xpos = xposition;
        newlist->ypos = yposition;
        newlist->zpos = zposition;
        newlist->nextListnode = NULL;

        previousPtr1 = NULL;
        currentPtr1 = (*leaf)->list;

        // loop to find the correct location in the list
            while( currentPtr1 != NULL && currentPtr1->num > count)
              
             /* || (currentPtr1->num == count && zposition > currentPtr1->zpos)
              || (currentPtr1->num == count && zposition == currentPtr1->zpos && (xposition * 10) + yposition > (currentPtr1->xpos * 10) + currentPtr1->ypos)) */
        {
            previousPtr1 = currentPtr1;
            currentPtr1 = currentPtr1->nextListnode;
        }//end while
            

        // insert at the beginning of list
        if(previousPtr1 == NULL){
            newlist->nextListnode = (*leaf)->list;
            (*leaf)->list = newlist;
        }//end if

        else{
            //insert node between previousptr1 and currentptr1
            previousPtr1->nextListnode = newlist;
            newlist->nextListnode = currentPtr1;
        }//end else

        }//end else 
    }//end else if

    else{// if the item name is larger than the current leafptr's item name
        insertTreenode(givename,count,xposition,yposition,zposition,&((*leaf)->rightTreenode));
    }//end else

}//end else
    return;
}//end function insertTreenode






void Tree::printTree(){
	printTree(basenode);
	return;
}//end function printTree



void Tree::printTree(treenode *leaf){
    treenode* nullptrt = NULL;
    // if the current treenode does not points to null
    if (leaf != nullptrt){
        //print the left treenode
        printTree(leaf->leftTreenode);    
        //print out the current tree node
        ListnodePtr listPtr;
        listPtr = leaf->list;
        listnode* nullptrl = NULL;
        cout << "name "<< (*leaf).name << endl;
        //loop to print out the store position and number of items
        while ( listPtr != nullptrl){
            cout <<" itemnum " << listPtr->num << " in position(" << listPtr->xpos << "," << listPtr->ypos << "," << listPtr->zpos<< ")"<<endl;
        listPtr = listPtr->nextListnode;
    }
    cout << endl;
    //print out the right treenode
    printTree(leaf->rightTreenode);
    }
    return;
}//end function printTree



//This looks through a tree and finds the given item within it.
StoreLink Tree::findItem(ItemLink givenItem){
	store *location = new store; //create the store which will be returned
	if (location == NULL){
		std::cout << "No memory available" << endl;
		return location;
	}
	if (basenode != NULL){ //check to make sure there is an item in the tree
		if (basenode->name == givenItem->name){//if the root node is the item being looked for
			if(givenItem->count < basenode->list->num){//if the store has enough items for the shopper
				location->xPos = basenode->list->xpos;//set the returning stores position to the found store
				location->yPos = basenode->list->ypos;
				location->zPos = basenode->list->zpos;
				basenode->list->num -= givenItem->count;//reserve those items for the shopper
				item *newGivenItem = new item;
				if (newGivenItem == NULL){ //create a new item which will be the one the shopper is looking for
					std::cout << "No memory available" << endl;
					return location;
				}
				newGivenItem->name = givenItem->name; //set the new item's name to be the same as the given one
				newGivenItem->count = givenItem->count; // set the new item's count to be the same as the given one
				location->targetItem = newGivenItem; // set the store's item to be the new item
				location->targetItem->itemLink = NULL; // make sure the store's item doesn't have another item attached
				
				return location; // return the new store
			} else { // otherwise: if the store does not have enough items
				ListnodePtr previousPtr;
				ListnodePtr currentPtr;
				ListnodePtr tmpPtr;
				previousPtr = NULL;
				tmpPtr = NULL;
				currentPtr = basenode->list;
				while(currentPtr != NULL){ //look through the list and find one that does
					if (currentPtr->num > givenItem->count){
					tmpPtr = currentPtr;
					}
					previousPtr = currentPtr;
					currentPtr = currentPtr->nextListnode;
				}
				if (tmpPtr != NULL) { //check to make sure there was an store which had enough items for the shopper
					location->xPos = tmpPtr->xpos; //set the shopper's position to the last one that had enough
					location->yPos = tmpPtr->ypos;
					location->zPos = tmpPtr->zpos;
					tmpPtr->num -= givenItem->count; //reserve the items for the shopper
					item *newGivenItem = new item; //create a new item which will be the one the shopper is looking for
					if (newGivenItem == NULL){
						std::cout << "No memory available" << endl;
						return location;
					}
					newGivenItem->name = givenItem->name; //set the new item's name to be the same as the given one
					newGivenItem->count = givenItem->count; // set the new item's count to be the same as the given one
					location->targetItem = newGivenItem; // set the store's item to be the new item
					location->targetItem->itemLink = NULL; // make sure the store's item doesn't have another item attached
					return location; //return the new store
				} else {
					return NULL; // if no stores have enough items, return NULL.
				}
			}
		} else if (basenode->name > givenItem->name && basenode->leftTreenode != NULL){ //otherwise, if this isn't the item being looked for, and the name is less in value
			location = findItem(givenItem, basenode->leftTreenode); //look in the left branch
			return location; //return whatever is found
		} else if (basenode->rightTreenode != NULL) { //otherwise, if this isn't the item being looked for, and the name is greater in value
			location = findItem(givenItem, basenode->rightTreenode); //look in the right branch
			return location; //return whatever is found
		} else { //SHOULD NEVER HAPPEN, if the name is not the same, nor smaller or greater
			StoreLink exitPtr;
			exitPtr = NULL;
			return exitPtr;
			}
	} else { //if the tree is empty, return NULL;
		StoreLink exitPtr;
		exitPtr = NULL;
		return exitPtr;
	}
}


//This looks through a branch of a tree and finds the given item within it.
StoreLink Tree::findItem(ItemLink givenItem, TreenodePtr givenTreenode){
	store *location = new store;//create the store which will be returned
	if (location == NULL){
		std::cout << "No memory available" << endl;
		return location;
	}
	if (givenTreenode != NULL){//check to make sure there is an item in the tree
		if (givenTreenode->name == givenItem->name){//if the head of the branch is the item being looked for
			if(givenItem->count < givenTreenode->list->num){//if the store has enough items for the shopper
				location->xPos = givenTreenode->list->xpos;//set the returning stores position to the found store
				location->yPos = givenTreenode->list->ypos;
				location->zPos = givenTreenode->list->zpos;
				givenTreenode->list->num -= givenItem->count;//reserve those items for the shopper
				item *newGivenItem = new item;//create a new item which will be the one the shopper is looking for
				if (newGivenItem == NULL){
					std::cout << "No memory available" << endl;
					return location;
				}
				newGivenItem->name = givenItem->name; //set the new item's name to be the same as the given one
				newGivenItem->count = givenItem->count; // set the new item's count to be the same as the given one
				location->targetItem = newGivenItem; // set the store's item to be the new item
				location->targetItem->itemLink = NULL; // make sure the store's item doesn't have another item attached
				return location;
			} else { // otherwise: if the store does not have enough items 
				ListnodePtr previousPtr;
				ListnodePtr currentPtr;
				ListnodePtr tmpPtr;
				tmpPtr = NULL;
				previousPtr = NULL;
				currentPtr = basenode->list;
				while(currentPtr != NULL){//look through the list and find one that does
				if (currentPtr->num > givenItem->count){
					tmpPtr = currentPtr;
					}
					previousPtr = currentPtr;
					currentPtr = currentPtr->nextListnode;
				}
				if (tmpPtr != NULL) { //check to make sure there was an store which had enough items for the shopper
					location->xPos = tmpPtr->xpos; //set the shopper's position to the last one that had enough
					location->yPos = tmpPtr->ypos;
					location->zPos = tmpPtr->zpos;
					tmpPtr->num -= givenItem->count; //reserve the items for the shopper
					item *newGivenItem = new item; //create a new item which will be the one the shopper is looking for
					if (newGivenItem == NULL){
						std::cout << "No memory available" << endl;
						return location;
					}
					newGivenItem->name = givenItem->name; //set the new item's name to be the same as the given one
					newGivenItem->count = givenItem->count; // set the new item's count to be the same as the given one
					location->targetItem = newGivenItem; // set the store's item to be the new item
					location->targetItem->itemLink = NULL; // make sure the store's item doesn't have another item attached
					return location; //return the new store
				} else {
					return NULL; // if no stores have enough items, return NULL.
				}
			}
		} else if (givenTreenode->name > givenItem->name && givenTreenode->leftTreenode != NULL){ //otherwise, if this isn't the item being looked for, and the name is less in value
			location = findItem(givenItem, givenTreenode->leftTreenode); //look in the left branch
			return location; //return whatever is found
		} else if (givenTreenode->rightTreenode != NULL) {//otherwise, if this isn't the item being looked for, and the name is greater in value
			location = findItem(givenItem, givenTreenode->rightTreenode);//look in the right branch
			return location; //return whatever is found
		} else { //SHOULD NEVER HAPPEN, if the name is not the same, nor smaller or greater
			StoreLink exitPtr;
			exitPtr = NULL;
			return exitPtr;
		}
	} else {//if the branch is empty, return NULL;
		StoreLink exitPtr;
		exitPtr = NULL;
		return exitPtr;
	}
}

