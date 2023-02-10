#include <iostream>
#include <fstream>

using namespace std;

struct Node
{
	int data;
	Node* left;
	Node* right;
};

int getOption(); // console menu; returns option
int getKey(); // user input; returns key; called for insertNode(), removeNode(), search()
void buildFromFile(Node*&); // creates tree from file; clears existing tree
void insertNode(Node*&, int); // insert one node at a time
void removeNode(Node*&, int); // delete one node at a time
int findPredKey(Node*&); // helper function for removeNode(); handles immediate predecessor
void search(Node*, int); // search for key
void clear(Node*&); // clears entire tree
void display(Node*); // called from main or buildFromFile(); prints lines and calls displayTree() to the tree
void displayTree(Node*, int); // called by display to print the nodes of the tree


int main()
{
	Node* root = NULL; // root node
	int option; // menu option

	// starting menu
	cout << "Option 1: Build from file" << endl
		 << "Option 2: Insert a key" << endl << endl;

	cout << "Option: ";
	cin >> option;

	while(option < 1 || option > 2)
	{
		cout << "Invalid option" << endl << "Option: ";
		cin >> option;
	}


	// control block
	do
	{
		if(option == 1)
		{
			clear(root);
			root = NULL;
			buildFromFile(root);
		}

		else if(option == 2)
		{
			insertNode(root, getKey());
			display(root);
		}

		else if(option == 3)
		{
			removeNode(root, getKey());
			display(root);
		}

		else if(option == 4)
		{
			search(root, getKey());
			display(root);
		}

		else //(option == 5)
		{
			clear(root);
			root = NULL;
		}

		option = getOption();
	}
	while(option != 6);

	cout << endl << "Program ending";

	return 0;
}


// console menu; returns option
int getOption()
{
	int option;

	cout << endl;
	cout << "Option 1: Build from file" << endl
		 << "Option 2: Insert a key" << endl
		 << "Option 3: Delete a key" << endl
		 << "Option 4: Search for a key" << endl
		 << "Option 5: Clear the tree" << endl
		 << "Option 6: Exit program" << endl << endl;

	cout << "Option: ";
	cin >> option;

	// check bounds
	while(option < 1 || option > 6)
	{
		cout << "Invalid option" << endl << "Option: ";
		cin >> option;
	}

	return option;
}

// user input; returns key; called for insertNode(), removeNode(), search()
int getKey()
{
	int key;

	cout  << endl << "Key(0-99): ";
	cin >> key;

	// check bounds
	while(key < 0 || key > 99)
	{
		cout << "Invalid key" << endl << endl << "Key: ";
		cin >> key;
	}

	return key;
}


// creates tree from file; clears existing tree
void buildFromFile(Node*& root)
{
	string filename;
	int key;

	cout << endl << "filename: ";
	cin >> filename;

	ifstream inFile(filename);

	// validate file exists
	if(!inFile)
	{
		cout << endl << "ERROR: file name can't be found" << endl;
		return;
	}

	cin.ignore();

	while(!inFile.eof())
	{
		// press enter to continue
		cout << endl << "Press enter to continue";
		cin.ignore();

		// get key from file; send to insert function; display
		inFile >> key;
		insertNode(root, key);
		display(root);
	}

	inFile.close();
}


// insert one node at a time
void insertNode(Node*& node, int key)
{
	// creating the new node
	if(node == NULL)
	{
		node = new Node;
		node->data = key;
		node->left = NULL;
		node->right = NULL;
	}

	// recursive calls to traverse tree
	else if(key < node->data)
		insertNode(node->left, key);

	else if(key > node->data)
		insertNode(node->right, key);

	else
		cout << "ERROR: key could not be placed" << endl;
}


// delete one node at a time
void removeNode(Node*& node, int key)
{
	Node* ptr = node;
	Node* trailingPtr = NULL;

	// loop to find node
	while(key != ptr->data)
	{
		if(key < ptr->data)
		{
			trailingPtr = ptr;
			ptr = ptr->left;
		}

		else if(key > ptr->data)
		{
			trailingPtr = ptr;
			ptr = ptr->right;
		}

		// not found
		if(ptr == NULL)
		{
			cout << "ERROR: key could not be deleted" << endl;
			return;
		}
	}

	// simple deletion
	if(ptr->left == NULL)
	{
		// deleting root node
		if(trailingPtr == NULL)
			node = ptr->right;

		else if(trailingPtr->right == ptr)
			trailingPtr->right = ptr->right;

		else //(trailingPtr->left == node)
			trailingPtr->left = ptr->right;

		delete ptr;
	}

	// "replacing" deleted node with immediate predecessor; just moving the data from immediate predecessor to deleted node
	else
		ptr->data = findPredKey(ptr);

	ptr = NULL;
	trailingPtr = NULL;
}

// helper function for removeNode(); handles immediate predecessor
int findPredKey(Node*& node)
{
	Node* ptr = node;
	Node* trailingPtr = node;
	int data;

	// find immediate predecessor; delete; return data from immediate predecessor

	ptr = ptr->left;

	if(ptr->right == NULL)
		node->left = ptr->left;

	else
	{
		while(ptr->right != NULL)
		{
			trailingPtr = ptr;
			ptr = ptr->right;
		}

		trailingPtr->right = ptr->left;
	}

	data = ptr->data;
	delete ptr;

	ptr = NULL;
	trailingPtr = NULL;

	return data;
}


// search for key
void search(Node* node, int key)
{
	if(node == NULL)
		cout << "ERROR: key could not be found" << endl;

	else if(key == node->data)
		cout << endl << "Found! " <<  node->data << endl;

	else if(key < node->data)
	{
		cout << "Checked " << node->data << endl;
		search(node->left, key);
	}

	else //(key > node->data)
	{
		cout << "Checked " << node->data << endl;
		search(node->right, key);
	}
}


// clears entire tree
void clear(Node*& node)
{
	if(node == NULL)
		return;

	clear(node->left);
	clear(node->right);
	delete node;
}


// called from main / buildFromFile(); prints lines and calls displayTree() to the tree
void display(Node* node)
{
	cout << endl << "-----------------------------------------" << endl;
	displayTree(node, 0);
	cout << endl << "-----------------------------------------" << endl;
}


// called by display to print the nodes of the tree
void displayTree(Node* node, int spacing)
{
	if(node == NULL)
		return;

	spacing += 8;

	displayTree(node->right, spacing);

	cout << endl;

	// loop for spacing
	for(int n = 8; n < spacing; n++)
		cout << " ";

	cout << node->data << endl;

	displayTree(node->left, spacing);
}

