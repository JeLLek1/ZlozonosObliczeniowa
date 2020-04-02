#pragma once
#include "Container.h"
struct NodeA {
	int value;
	NodeA* left, * right, * parent;
	int bf;
	NodeA(int value, NodeA* parent);
};

class Avl : public Container {
private:
	NodeA* root;
	void ereaseFrom(NodeA* node);
	size_t treeLevel(NodeA* fromNode, size_t level);
	void fillSpaceArray(int* spaceArray, NodeA* node, size_t level);
	void drawTree(int* spaceArray, bool* levelDrown, NodeA* node, int level, bool right);
	void rotateLL(NodeA* node);
	void rotateRR(NodeA* node);
	void rotateRL(NodeA* node);
	void rotateLR(NodeA* node);
	NodeA* popNode(NodeA* node);

	NodeA* findPredecessor(NodeA* node);
public:
	static const int MAX_MENU = 5;
	Avl(std::string name);
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
	~Avl();
};

