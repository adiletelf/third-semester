#pragma once
#include <iostream>
const int RANDOM_MAX = 100;

class Node
{
public:
	int value;
	Node* parent;
	Node* child;

	Node(int value_ = 0, Node* parent_ = nullptr, Node* child_ = nullptr) :
		value(value_), parent(parent_), child(child_)
	{
	}

	~Node()
	{
	}

	//Node& operator= (const Node& node);

	friend std::ostream& operator<< (std::ostream& out, const Node* node);
	friend std::ostream& operator<< (std::ostream& out, const Node&  node);
};





class List
{
private:
	Node* home{ nullptr };

public:
	List(int size = 0); // random values
	List(int size, int value);
	List(int* arr, int size);
	List(const List& list);
	~List()
	{
		clear();
	}

	int size() const;
	bool isEmpty() const;

	Node* find(int value);
	void insert(int value, int index); // insert at index
	void insertAfterKey(int value, int key); // insert after first found key
	void append(int value);
	void prepend(int value); // insert before home
	void del(int index);
	void delHead();
	void delTail();

	int min();
	int max();
	void clear();

	Node* operator[] (int index);
	List& operator=  (const List& list);
	List  operator+  (const List& list);

	bool  operator== (const List& list);
	bool  operator!= (const List& list);


	friend std::ostream& operator<< (std::ostream& out, List& list);
	friend std::istream& operator>> (std::istream& in, List& list);

};

