#include <string>
#include <cstring>
#include <iostream>


//node in the list of store position and item numbers
struct listnode{
	int num;
	int xpos;
	int ypos;
	int zpos;
	struct listnode *nextListnode;
};

typedef struct listnode Listnode;
typedef Listnode *ListnodePtr;


//node in the binary tree of item name
struct treenode{
	struct treenode *leftTreenode;//left pointer
	std::string name;// name code of the item
	Listnode *list;// list of the position and item numbers of the item
	struct treenode *rightTreenode;//right pointer
};

typedef struct treenode Treenode;
typedef Treenode *TreenodePtr;







class Tree{
public:
	Tree();
	void insestTreenode(std::string givename, int count, int xposition, int yposition, int zposition);
	void printTree();

private:
	Treenode basenode;

	void insertTreenode(std::string givename, int count, int xposition, int yposition, int zposition, Listnode *leaf);
	void printTree(Listnode *leaf);
};

Tree::Tree(){
	Treenode newnode;
	newnode.name = "NULL";
	newnode.list = NULL;
	newnode.leftTreenode = NULL;
	newnode.rightTreenode = NULL;
	basenode = newnode;
}







int main{
	int i = 0;
	std::cout << "Start" << std::endl;
	std::string itemname;
	int x-pos, y-pos, z-pos, count, items = 0;
	int sentinelNotFound = 0;
	Tree rootTree();
	while (sentinelNotFound == 0){
		std::cout << "Running" << std::endl;
		std::cin >> x-pos >> y-pos >> z-pos;
		
		if(x-pos == 16 && y-pos == 16 && z-pos == 16){// if sentinel found
			sentinelNotFound = 1;
		}//end of if

		else{// if not reach the senitel 
			std::cin >> items;
			for ( i = 0; i < items; i++){
				std::cin >> itemname >> count;
				rootTree.insertTreenode(itemname,count,x-pos,y-pos,z-pos);
			}//end of for loop
		}//end of else
	}// end of while

	rootTree.printTree();//print out tree

	return 0;
}//end of function






	
