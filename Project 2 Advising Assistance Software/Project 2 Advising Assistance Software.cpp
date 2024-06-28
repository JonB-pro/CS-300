// Jonathan Boeglin
// Project 2 Advising Assistance Software.cpp
// Credit for BST structure idea: CS300 5-2 Assignment Binary Search Tree

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

// Course structure for holding csv info
struct Course {
	string crsNum;
	string crsName;
	string crsReqs;

	Course() {

	}

};

// Node structure for holding courses in BST
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course c) : Node() {
		course = c;
	}
};

// BST class
class BinarySearchTree {
private:
	Node* root;
	void addNode(Node* node, Course c);
	void inOrder(Node* node);
public:
	BinarySearchTree();
	void RecursiveDestroy(Node* node);
	virtual ~BinarySearchTree();
	void PrintInOrder();
	void BSTInsert(Course c);
	void CourseInfo(Course course);
	Course SearchCourseForPrint(string crsNum);
};

// Constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

// Allows destructor to remove whole tree
void BinarySearchTree::RecursiveDestroy(Node* node) {

	// if the node isn't empty
	if (node != nullptr) {
		// remove nodes from left
		RecursiveDestroy(node->left);
		node->left = nullptr;
		// remove nodes from right
		RecursiveDestroy(node->right);
		node->right = nullptr;
		// final deletion
		delete node;
	}
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
	RecursiveDestroy(root);
}

 // Inserts new course if root is taken
void BinarySearchTree::addNode(Node* node, Course c) {

	// if new course is smaller
	if (node->course.crsNum > c.crsNum) {

		// if nothing smaller after, add to left
		if (node->left == nullptr) {
			node->left = new Node(c);
		}
		// else keep going left
		else {
			this->addNode(node->left, c);
		}
	}

	// if new course is bigger
	else {

		// if nothing bigger after, add to right
		if (node->right == nullptr) {
			node->right = new Node(c);
		}
		// else keep going right
		else {
			this->addNode(node->right, c);
		}
	}
}

// Inserts new course into tree
void BinarySearchTree::BSTInsert(Course c) {

	// if tree is empty, add to root
	if (root == nullptr) {
		root = new Node(c);
	}

	// otherwise find empty spot
	else {
		this->addNode(root, c);
	}
}

// Checks tree for course to print
Course BinarySearchTree::SearchCourseForPrint(string crsNum) {
	Node* node = root;

	while (node != nullptr) {
		// returns course info on match
		if (crsNum == node->course.crsNum) {
			return node->course;
		}

		// checks left sides of tree
		if (crsNum < node->course.crsNum) {
			node = node->left;
		}
		 // checks right sides of tree
		else {
			node = node->right;
		}
	}
	
	// returns empty course if not found
	Course course;
	return course;
}
// Starts run through tree
void BinarySearchTree::PrintInOrder() {
	this->inOrder(root);
}

// Runs through tree in alphanumeric order
void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.crsNum << ", " << node->course.crsName << endl;
		inOrder(node->right);
	}
}

// Prints course information
void BinarySearchTree::CourseInfo(Course course) {
	cout << course.crsNum << ", " << course.crsName << endl;
	cout << "Prerequisites: " << course.crsReqs << endl;
	return;
}

// Loads csv file from folder containing program and inserts into tree
void fileLoad(string fileName, BinarySearchTree* bst) {
	ifstream file;
	file.open(fileName);
	string line;
	int i = 0;

	// Assigns each line to course object
	while (getline(file, line)) {
		i = 0;
		string cNum = "";
		string cName = "";
		string preReqs = "";

		stringstream inStr(line);
		getline(inStr, cNum, ',');
		getline(inStr, cName, ',');
		getline(inStr, preReqs);

		if ((cNum == "") || (cName == "")) {
			cout << "Please use correct course format";
			break;
		}

		Course course;
		course.crsNum = cNum;
		course.crsName = cName;
		course.crsReqs = preReqs;

		// Inserts into tree
		bst->BSTInsert(course);
		line = "";
	}

	file.close();
}

// Displays menu options
void MenuDisplay() {
	cout << "  1. Load Courses" << endl;
	cout << "  2. Print Alphanumerically Sorted Course List" << endl;
	cout << "  3. Print Individual Course" << endl;
	cout << "  9. Exit" << endl;
	cout << "What would you like to do? ";
}


int main() {
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;
	int choice = 0;
	string filePath, cNum = "";
	string str = "";
	stringstream inStr(str);

	// Display Title and then loop menu until exited
	cout << "Welcome to course planner." << endl;
	cout << endl;
	while (choice != 9) {

		MenuDisplay();
		cin >> choice;

		// menu options switch
		switch (choice) {
		// Load file
		case 1:
			cout << "Enter name of file you wish to load: ";
			//filePath = "Courses.csv"
			cin >> filePath;
			fileLoad(filePath, bst);
			cout << endl;
			cout << "File loaded." << endl;
			break;

		// Print courses alphanumerically
		case 2:
			cout << "Here is a sample schedule: " << endl;
			cout << endl;
			bst->PrintInOrder();
			cout << endl;
			break;

		// Print individual course info
		case 3:
			cout << "What course do you want to know about? ";
			cin >> cNum;
			transform(cNum.begin(), cNum.end(), cNum.begin(), ::toupper);
			cout << endl;
			course = bst->SearchCourseForPrint(cNum);
			if (!course.crsNum.empty()) {
				bst->CourseInfo(course);
			}
			else {
				cout << "Course " << cNum << " does not exist." << endl;
			}
			cout << endl;
			break;
		
		// End program
		case 9:
			break;

		// Numbers not on menu pushed
		default:
			cout << choice << " is not a valid option." << endl;
			cout << endl;
			break;
		}
	}

}


