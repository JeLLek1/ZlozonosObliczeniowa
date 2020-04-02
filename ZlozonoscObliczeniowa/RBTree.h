#pragma once
#include "Container.h"
enum class Color {
	RED = 0,
	BLACK = 1
};

struct Node {
	int value;
	Node* left, * right, * parent;
	Color color;
	Node(int value, Node* parent, Node* NIL);
};

class RBTree : public Container{
private:
	Node* root;
	Node* NIL;
	void ereaseFrom(Node* node);
	size_t treeLevel(Node* fromNode, size_t level);
	void fillSpaceArray(int* spaceArray, Node* node, size_t level);
	void drawTree(int* spaceArray, bool* levelDrown, Node* node, int level, bool right);
	void rotateLeft(Node* node);
	void rotateRight(Node* node);
	Node* findSuccesor(Node* node);
public:
	static const int MAX_MENU = 5;
	RBTree(std::string name);
	void push(int value);
	void erase();
	void random(size_t size);
	bool find(int value);
	bool popValue(int value);
	bool loadFile(std::fstream* file);
	virtual void drawMenu(int currentOption);
	virtual void handleInput(int currentOption);
	virtual void display();
	virtual int maxMenu();
	~RBTree();
};

