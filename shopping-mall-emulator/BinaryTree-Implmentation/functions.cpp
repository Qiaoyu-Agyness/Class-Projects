
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>


using namespace std;
using std::cin;
using std::cout;
using std::endl;


//node in the list of store positions and item numbers
struct listnode{
	int num;// number of items
	int xpos;// x-position
	int ypos;///y-position
	int zpos;//z-position
	listnode *nextListnode;// pointer to the next listnode
};

typedef listnode *ListnodePtr;


//node in the binary tree of item name
struct treenode{
	treenode *leftTreenode;//left pointer
	string name;// name code of the item
	listnode *list;// list of the position and item numbers of the item
	treenode *rightTreenode;//right pointer
};
typedef treenode *TreenodePtr;






// tree class definition
class Tree{
public:	
	Tree();//constructor
	void insertTreenode(string givename,int count, int xposition, int yposition, int zposition);// insert an item into the tree
	void printTree();//print out the tree

private:
	treenode *basenode;
	void insertTreenode(string givename,int count, int xposition, int yposition, int zposition, TreenodePtr *leaf);
	void printTree(treenode *leaf);
};

//initiate a new null tree structure
Tree::Tree(){

	basenode = NULL;

}

//insert the item into the tree public version
void Tree::insertTreenode(string givename, int count, int xposition, int yposition, int zposition){
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

                                                                                                                

//insert item into the tree private version
void Tree::insertTreenode(string givename,int count, int xposition, int yposition, int zposition, TreenodePtr *leaf){
    
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




//printtree public version
void Tree::printTree(){
	printTree(basenode);
	return;
}//end function printTree



//printtree private version
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









//main function
int main(){
	int i = 0;
	string itemname;
	int xpos, ypos, zpos, count, items = 0;
	int sentinelNotFound = 0;
	// initilize tree class
	Tree rootTree;
	rootTree = Tree();
	while (sentinelNotFound == 0){
		cin >> xpos >> ypos >> zpos;
		
		if(xpos == 16 && ypos == 16 && zpos == 16){// if sentinel found
			sentinelNotFound = 1;
		}//end of if

		else{// if not reach the senitel 
			cin >> items;//read in the numbers of stores
			for ( i = 0; i < items; i++){
				cin >> itemname >> count;//read in item name and number
				rootTree.insertTreenode(itemname,count,xpos,ypos,zpos);
			}//end of for loop

			
		}//end of else
	}// end of while

	rootTree.printTree();//print out tree

	return 0;
}//end of mainfunction






	
