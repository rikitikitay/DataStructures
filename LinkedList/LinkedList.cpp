#include "LinkedList.h"
#include <cassert>
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
	this->next = new Node(value, this->next);
}

void LinkedList::Node::removeNext()
{
	Node* removeNode = this->next;
	Node* newNext = removeNode->next;
	delete removeNode;
	this->next = newNext;
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

void LinkedList::remove(const size_t pos)
{
}

void LinkedList::removeNextNode(Node* node)
{
}

long long int LinkedList::findIndex(const ValueType& value) const
{
	return 0;
}

LinkedList::Node* LinkedList::findNode(const ValueType& value) const
{
	return nullptr;
}

void LinkedList::reverse()
{
}

LinkedList LinkedList::reverse() const
{
	return LinkedList();
}

LinkedList LinkedList::getReverseList() const
{
	return LinkedList();
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
