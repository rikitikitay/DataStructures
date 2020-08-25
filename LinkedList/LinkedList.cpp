#include "LinkedList.h"
#include <cassert>
#include <stdexcept>
LinkedList::Node::Node(const ValueType& value, Node* next)
{
	this->value = value;
	this->next = next;
}

LinkedList::Node::~Node()
{
	// ничего не удаляем, т.к. агрегация
}

void LinkedList::Node::insertNext(const ValueType& value)
{
	if (!this) 
	{
		throw invalid_argument("this node is nullptr !!! can't insert next node");
	}
	this->next = new Node(value, this->next);
}

void LinkedList::Node::removeNext()
{
	if (!this) 
	{
		throw invalid_argument("this node is nullptr !!! can't remove next node");
	}
	Node* removeNode = this->next;
	if (removeNode) 
	{
		Node* newNext = removeNode->next;
		delete removeNode;
		this->next = newNext;
	} else 
	{
		throw invalid_argument("this node is last !!! cant't remove next node"); 
	}; 
}

LinkedList::LinkedList()
	: _head(nullptr), _size(0)
{
}

LinkedList::LinkedList(const LinkedList& copyList)
{
	this->_size = copyList._size;
	if (this->_size == 0) {
		this->_head = nullptr;
		return;
	}

	this->_head = new Node(copyList._head->value); 

	Node* currentNode = this->_head; 

	Node* currentCopyNode = copyList._head; 

	while (currentCopyNode->next) { 
		currentNode->next = new Node(currentCopyNode->next->value);
		currentCopyNode = currentCopyNode->next;
		currentNode = currentNode->next;
	};

}

LinkedList& LinkedList::operator=(const LinkedList& copyList)
{
	if (this == &copyList) {
		return *this;
	}

	forceNodeDelete(this->_head);

	this->_size = copyList._size;
	if (this->_size == 0) {
		this->_head = nullptr;
		return *this;
	}

	this->_head = new Node(copyList._head->value); 

	Node* currentNode = this->_head; 
	Node* currentCopyNode = copyList._head; 
	
	while (currentCopyNode->next) { 
		currentNode->next = new Node(currentCopyNode->next->value);
		currentCopyNode = currentCopyNode->next;
		currentNode = currentNode->next;
	};
	return *this;
}

LinkedList::LinkedList(LinkedList&& moveList) noexcept
{
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;

}

LinkedList& LinkedList::operator=(LinkedList&& moveList) noexcept
{
	if (this == &moveList) {
		return *this;
	}
	forceNodeDelete(_head);
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;
	return *this;
}

LinkedList::~LinkedList()
{
	forceNodeDelete(_head);
}

const ValueType& LinkedList::operator[](const size_t pos) const
{
	return getNode(pos)->value;
}

ValueType& LinkedList::operator[](const size_t pos) 
{
	return getNode(pos)->value;
}

LinkedList::Node* LinkedList::getNode(const size_t pos) const
{	
	// assert(pos >= 0);
	assert(pos < this->_size);
	
	Node* bufNode = this->_head;
	for (int i = 0; i < pos; ++i) {
		bufNode = bufNode->next;
	}

	return bufNode;
}

void LinkedList::insert(const size_t pos, const ValueType& value)
{	
	// assert(pos >= 0); // pos всегда больше 0 из того что size_t, 
	// не знаю, что с этим делать
	assert(pos <= this->_size);

	if (pos == 0) {
		pushFront(value);
	}
	else {
		Node* bufNode = this->_head;
		for (size_t i = 0; i < pos-1; ++i) {
			bufNode = bufNode->next;
		}
		bufNode->insertNext(value);
		++_size;
	}
}

void LinkedList::insertAfterNode(Node* node, const ValueType& value)
{
	node->insertNext(value);
	++_size;
}

void LinkedList::pushBack(const ValueType& value)
{
	if (_size == 0) {
		pushFront(value);
		return;
	}
	insert(_size, value);
}

void LinkedList::pushFront(const ValueType& value)
{
	_head = new Node(value, _head);
	++_size;
}

void LinkedList::removeFront() 
{
	Node* newHead = this->_head->next;
	delete this->_head;
	this->_head = newHead;
	--_size;
}

void LinkedList::remove(const size_t pos)
{	
	assert(this->_size > 0);
	assert(pos < this->_size);
	 if (pos == 0) 
	{
		this->removeFront();
		return;
	} else 
	{
		Node* bufNode = this->_head;
		for (size_t i = 0; i < pos - 1; ++i) 
		{
			bufNode = bufNode->next;	
		}
		bufNode->removeNext();
		--_size;
	}
}

void LinkedList::removeBack() 
{
	this->remove(_size - 1); // вероятно, можно лучше, но пока так
}


void LinkedList::removeNextNode(Node* node)
{
	node->removeNext(); //а если нулевой указатель?
	--_size;
}
long long int LinkedList::findIndex(const ValueType& value) const
{
	long long int index = -1;
	long long int count = 0;
	Node* bufNode = this->_head;	
	while (bufNode) 
	{	
		if (bufNode->value == value) 
		{
			index = count;
			break;
		}
		bufNode = bufNode->next;
		count += 1;	
	}
	return index;
}

LinkedList::Node* LinkedList::findNode(const ValueType& value) const
{
	Node* bufNode = this->_head;	
	while (bufNode) 
	{	
		if (bufNode->value == value) 
		{
			break;
		}
		bufNode = bufNode->next;
	}
	return bufNode;
}

void LinkedList::reverse()
{
	Node* bufNode = this->_head;
	if (bufNode) 	
		while (bufNode->next)
		{
			pushFront(bufNode->next->value);
			this->removeNextNode(bufNode);
		}	
}

LinkedList LinkedList::reverse() const // потестить ещё
{
	return this->getReverseList();
}

LinkedList LinkedList::getReverseList() const
{
	LinkedList bufList;
	Node* bufNode = this->_head;
	if (bufNode) 
	{	
		bufList.pushFront(bufNode->value);
		while (bufNode->next) 
		{
			bufList.pushFront(bufNode->next->value);
			bufNode = bufNode->next;
		}
	}
	return bufList;
}

size_t LinkedList::size() const
{
	return _size;
}

void LinkedList::forceNodeDelete(Node* node)
{
	if (node == nullptr) {
		return;
	}

	Node* nextDeleteNode = node->next;
	delete node;
	forceNodeDelete(nextDeleteNode);
	
}
