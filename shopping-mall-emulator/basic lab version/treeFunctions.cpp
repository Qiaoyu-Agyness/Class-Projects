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

StoreLink Tree::findItem(ItemLink givenItem){
	store *location = new store;
	if (location == NULL){
		std::cout << "No memory available" << endl;
		return location;
	}
	if (basenode != NULL){
		if (basenode->name == givenItem->name){
			if(givenItem->count < basenode->list->num){
				location->xPos = basenode->list->xpos;
				location->yPos = basenode->list->ypos;
				location->zPos = basenode->list->zpos;
				basenode->list->num -= givenItem->count;
				item *newGivenItem = new item;
				if (newGivenItem == NULL){
					std::cout << "No memory available" << endl;
					return location;
				}
				newGivenItem->name = givenItem->name;
				newGivenItem->count = givenItem->count;
				location->targetItem = newGivenItem;
				location->targetItem->itemLink = NULL;
				
				return location;
			} else {
				ListnodePtr previousPtr;
				ListnodePtr currentPtr;
				ListnodePtr tmpPtr;
				previousPtr = NULL;
				currentPtr = basenode->list;
				tmpPtr = NULL;
				while(currentPtr != NULL){
					if (currentPtr->num > givenItem->count){
						tmpPtr = currentPtr;
					}
					previousPtr = currentPtr;
					currentPtr = currentPtr->nextListnode;
				}

				if(tmpPtr = NULL){
					StoreLink exitPtr;
					exitPtr = NULL;
					return exitPtr;
				}

				location->xPos = tmpPtr->xpos;
				location->yPos = tmpPtr->ypos;
				location->zPos = tmpPtr->zpos;
				tmpPtr->num -= givenItem->count;
				item *newGivenItem = new item;
				if (newGivenItem == NULL){
					std::cout << "No memory available" << endl;
					return location;
				}
				newGivenItem->name = givenItem->name;
				newGivenItem->count = givenItem->count;
				location->targetItem = newGivenItem;
				location->targetItem->itemLink = NULL;
				return location;
			}
		} else if (basenode->name > givenItem->name && basenode->leftTreenode != NULL){
			location = findItem(givenItem, basenode->leftTreenode);
			return location;
		} else if (basenode->rightTreenode != NULL) {
			location = findItem(givenItem, basenode->rightTreenode);
			return location;
		} else {
			StoreLink exitPtr;
			exitPtr = NULL;
			return exitPtr;
			}
	} else {
		StoreLink exitPtr;
		exitPtr = NULL;
		return exitPtr;
	}
}

StoreLink Tree::findItem(ItemLink givenItem, TreenodePtr givenTreenode){
	store *location = new store;
	if (location == NULL){
		std::cout << "No memory available" << endl;
		return location;
	}
	if (givenTreenode != NULL){
		if (givenTreenode->name == givenItem->name){
			if(givenItem->count < givenTreenode->list->num){
				location->xPos = givenTreenode->list->xpos;
				location->yPos = givenTreenode->list->ypos;
				location->zPos = givenTreenode->list->zpos;
				givenTreenode->list->num -= givenItem->count;
				item *newGivenItem = new item;
				if (newGivenItem == NULL){
					std::cout << "No memory available" << endl;
					return location;
				}
				newGivenItem->name = givenItem->name;
				newGivenItem->count = givenItem->count;
				location->targetItem = newGivenItem;
				location->targetItem->itemLink = NULL;
				return location;
			} else {
				ListnodePtr previousPtr;
				ListnodePtr currentPtr;
				ListnodePtr tmpPtr;
				previousPtr = NULL;
				currentPtr = basenode->list;
				tmpPtr = NULL;
				while(currentPtr != NULL){
				if (currentPtr->num > givenItem->count){
					tmpPtr = currentPtr;
					}
					previousPtr = currentPtr;
					currentPtr = currentPtr->nextListnode;
				}
				if(tmpPtr = NULL){
					StoreLink exitPtr;
					exitPtr = NULL;
					return exitPtr;
				}
				location->xPos = tmpPtr->xpos;
				location->yPos = tmpPtr->ypos;
				location->zPos = tmpPtr->zpos;
				tmpPtr->num -= givenItem->count;
				location->targetItem = givenItem;
				location->targetItem->itemLink = NULL;
				return location;
			}
		} else if (givenTreenode->name > givenItem->name && givenTreenode->leftTreenode != NULL){
			location = findItem(givenItem, givenTreenode->leftTreenode);
			return location;
		} else if (givenTreenode->rightTreenode != NULL) {
			location = findItem(givenItem, givenTreenode->rightTreenode);
			return location;
		} else {
			StoreLink exitPtr;
			exitPtr = NULL;
			return exitPtr;
		}
	} else {
		StoreLink exitPtr;
		exitPtr = NULL;
		return exitPtr;
	}
}

