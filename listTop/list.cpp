#include "list.h"

//Node& Node::operator= (const Node& node)
//{
//	if (this == &node)
//	{
//		return *this;
//	}
//
//	value = node.value;
//	parent = node.parent;
//	child = node.parent;
//
//	return *this;
//}


std::ostream& operator<< (std::ostream& out, const Node* node)
{
	if (node == nullptr)
		return out;

	out << node->value;
	return out;
}


std::ostream& operator<< (std::ostream& out, const Node& node)
{
	if (&node == nullptr)
		return out;

	out << node.value;
	return out;
}


List::List(int size)
{
	if (size < 0)
	{
		std::cout << "Incorrect size\n";
		return;
	}

	if (size == 0)
	{
		return;
	}

	home = new Node(rand() % ::RANDOM_MAX);
	Node* node = home;

	for (int i = 0; i < size - 1; ++i)
	{
		node->child = new Node(rand() % ::RANDOM_MAX, node);
		node = node->child;
	}
}


List::List(int size, int value)
{
	if (size < 0)
	{
		std::cout << "Incorrect size\n";
		size = 0;
	}
	
	if (size == 0)
	{
		return;
	}


	home = new Node(value);
	Node* node = home;

	for (int i = 0; i < size - 1; ++i) // we have already created first Node(home) -> size - 1
	{
		node->child = new Node(value, node);
		node = node->child;
	}
}


List::List(int* arr, int size)
{
	if (size <= 0)
		return;

	home = new Node(arr[0]);
	Node* node = home;
	for (int i = 1; i < size; ++i)
	{
		node->child = new Node(arr[i], node);
		node = node->child;
	}
}


List::List(const List& list)
{
	if (list.home == nullptr)
		return;

	
	Node* second = list.home;
	home = new Node(second->value);
	Node* first = home;

	while (second->child != nullptr)
	{
		first->child = new Node(second->child->value, first);

		first = first->child;
		second = second->child;
	}
}



int List::size() const
{
	int size = 0;
	Node* node = home;

	while (node != nullptr)
	{
		node = node->child;
		++size;
	}

	return size;
}


bool List::isEmpty() const
{
	return (home == nullptr);
}


Node* List::find(int value)
{
	Node* node = home;
	while (node != nullptr)
	{
		if (node->value == value)
			return node;

		node = node->child;
	}

	return node; // nullptr
}

// insert(int key) -> '�������� � ������ ������'
// insert(int key, int index) -> '�������� � index ������� ������'
void List::insert(int value, int index)
{
	if (index < 0 || index > size())
	{
		std::cout << "Incorrect index\n";
		return;
	}

	if (index == size())
	{
		append(value);
		return;
	}

	Node* node = home;

	if (index == 0)
	{
		/*Node* newNode = new Node(value);
		newNode->child = home;
		home->parent = newNode;
		home = newNode;*/
		prepend(value);
		return;
	}

	

	for (int i = 0; i < index && node->child != nullptr; ++i)
		node = node->child;

	Node* newNode = new Node(value, node->parent, node);
	node->parent->child = newNode;
	node->parent = newNode;
}


void List::insertAfterKey(int value, int key)
{
	if (isEmpty())
	{
		//insert(key);
		std::cout << "List is empty\n";
		return;
	}

	Node* node = home;
	while (node->child != nullptr && node->value != key)
	{
		node = node->child;
	}

	if (node->value == key)
	{
		Node* newNode = new Node(value, node, node->child);
		if (node->child != nullptr)
			node->child->parent = newNode;

		node->child = newNode;
	}
	else
	{
		std::cout << "No such key in List\n";
	}


}


void List::append(int key)
{
	
	Node* node = home;
	if (node == nullptr)
	{
		node = new Node(key);
		home = node;
		return;
	}

	while (node->child != nullptr)
	{
		node = node->child;
	}

	node->child = new Node(key, node);
}


void List::prepend(int value)
{
	// insert before head
	insert(value, 0);
}


void List::del(int index)
{
	if (index < 0 || index >= size())
	{
		std::cout << "Incorrect index\n";
		return;
	}

	if (index == 0)
	{
		delHead();
		return;
	}

	if (index == size() - 1)
	{
		delTail();
		return;
	}

	//Node* nod = this->operator[] (index);
	Node* node = (*this)[index];

	node->parent->child = node->child;
	node->child->parent = node->parent;

	delete node;
	node = nullptr;
}


void List::delHead()
{
	if (!home)
		return;

	Node* node = home;
	home = home->child;
	delete node;
	node = nullptr;
	home->parent = nullptr;
}


void List::delTail()
{
	if (!home)
		return;

	Node* node = (*this)[size() - 1];
	node->parent->child = nullptr;
	delete node;
	node = nullptr;
}



int List::min()
{
	if (home == nullptr)
	{
		std::cout << "List is not initialized\n";
		return 0;
	}


	Node* node = home;
	int min = home->value;
	
	int num;
	while (node->child != nullptr)
	{
		node = node->child;
		num = node->value;

		if (num < min)
			min = num;
	}

	return min;
}


int List::max()
{
	if (home == nullptr)
	{
		std::cout << "List is not initialized\n";
		return 0;
	}

	Node* node = home;
	int max = home->value;

	int num;
	while (node->child != nullptr)
	{
		node = node->child;
		num = node->value;

		if (num > max)
			max = num;
	}

	return max;
}


void List::clear()
{
	if (!home)
	{
		return;
	}

	Node* node = home;
	while (node->child != nullptr)
	{
		node = node->child;
		delete node->parent;
		node->parent = nullptr;
	}

	delete node;
	node = nullptr;
	home = nullptr;
}


Node* List::operator[] (int index)
{
	if (!home)
	{
		std::cout << "List is not initialized.\n";
		return nullptr;
	}
	
	if (index < 0 || index >= size())
	{
		std::cout << "Incorrect index\n";
		return nullptr;
	}


	Node* node = home;
	for (int i = 0; i < index; ++i)
	{
		node = node->child;
	}

	return node;
}


List& List::operator= (const List& list)
{
	if (this == &list)
	{
		return *this;
	}

	clear();

	if (!list.home)
	{
		return *this;
	}
	home = new Node(list.home->value);

	Node* first = home;
	Node* second = list.home;

	while (second->child != nullptr)
	{
		first->child = new Node(second->child->value, first);
		first = first->child;
		second = second->child;
	}

	return *this;
}


List List::operator+ (const List& list)
{
	if (!list.home)
	{
		return *this;
	}

	List result(*this);
	List secondPart(list);
	
	Node* node = result.home;
	while (node->child != nullptr)
	{
		node = node->child;
	}
	node->child = secondPart.home;
	secondPart.home->parent = node;

	return result;
}


bool List::operator== (const List& list)
{
	if (this == &list)
		return true;

	Node* first = home;
	Node* second = list.home;

	if (first->value != second->value)
		return false;

	while (first->child && second->child)
	{
		first = first->child;
		second = second->child;

		if (first->value != second->value)
			return false;
	}

	if (first->child != second->child)
		return false;

	return true;
}


bool List::operator!= (const List & list)
{
	return !(*this == list);
}


std::ostream& operator<< (std::ostream& out, List& list)
{
	Node* node = list.home;
	while (node != nullptr)
	{
		out << node->value << ' ';
		node = node->child;
	}

	return out;
}


std::istream& operator>> (std::istream& in, List& list)
{
	int size = -1;
	
	while(size < 0)
	{
		std::cout << "Enter a size of the list: ";
		in >> size;

		if (size < 0)
			std::cout << "Incorrect size. Try again please.\n";
	}

	if (size == 0)
	{
		return in;
	}

	std::cout << "Enter " << size << " values: ";
	
	list.home = new Node;
	Node* node = list.home;
	int num;
	in >> num;
	node->value = num;

	for (int i = 1; i < size; ++i)
	{
		in >> num;
		node->child = new Node(num, node);
		node = node->child;
	}

	return in;
}
