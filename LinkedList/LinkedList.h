#pragma once
#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <cstddef>
#include <cassert>
#include <stdexcept>
template <typename ValueType>
class LinkedList
{
	// класс узла списка
	// по своей сути, может содержать любые данные,
	// можно реализовать и ассоциативный массив, просто добавив 
	// поле с ключем в узел и, с учетом этого, поменять методы LinkedList 
	// (доступ по ключу, поиск по ключу и т.д.)
	struct Node {
		Node(const ValueType& value, Node* next = nullptr);
		~Node();

		void insertNext(const ValueType& value);
		void removeNext();

		ValueType value;
		Node* next;
	};

public:
	////
	LinkedList();
	LinkedList(const LinkedList& copyList);
	LinkedList& operator=(const LinkedList& copyList);

	LinkedList(LinkedList&& moveList) noexcept;
	LinkedList& operator=(LinkedList&& moveList) noexcept;

	~LinkedList();
	////

	// доступ к значению элемента по индексу
	const ValueType& operator[](const size_t pos) const;
	ValueType& operator[](const size_t pos);
	// доступ к узлу по индексу
	LinkedList::Node* getNode(const size_t pos) const;
	
	// вставка элемента по индексу, сначала ищем, куда вставлять (О(n)), потом вставляем (O(1))
	void insert(const size_t pos, const ValueType& value);
	// вставка элемента после узла, (O(1))
	void insertAfterNode(Node* node, const ValueType& value);
	// вставка в конец (О(n))
	void pushBack(const ValueType& value);
	// вставка в начало (О(1))
	void pushFront(const ValueType& value);

	// удаление
	void remove(const size_t pos);
	void removeNextNode(Node* node);
	void removeFront();
	void removeBack();
	
	// поиск, О(n)
	long long int findIndex(const ValueType& value) const;
	Node* findNode(const ValueType& value) const;

	// разворот списка
	void reverse();						// изменение текущего списка
	LinkedList reverse() const;			// полчение нового списка (для константных объектов)
	LinkedList getReverseList() const;	// чтобы неконстантный объект тоже мог возвращать новый развернутый список

	size_t size() const;
private:
	Node*	_head;
	size_t	_size;

	void forceNodeDelete(Node* node);
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ realization
template <typename ValueType> 
LinkedList<ValueType>::Node::Node(const ValueType& value, Node* next)
{
	this->value = value;
	this->next = next;
}

template <typename ValueType> 
LinkedList<ValueType>::Node::~Node()
{
	// ничего не удаляем, т.к. агрегация
}

template <typename ValueType> 
void LinkedList<ValueType>::Node::insertNext(const ValueType& value)
{
	if (!this) 
	{
		throw std::invalid_argument("this node is nullptr !!! can't insert next node");
	}
	this->next = new Node(value, this->next);
}

template <typename ValueType> 
void LinkedList<ValueType>::Node::removeNext()
{
	if (!this) 
	{
		throw std::invalid_argument("this node is nullptr !!! can't remove next node");
	}
	Node* removeNode = this->next;
	if (removeNode) 
	{
		Node* newNext = removeNode->next;
		delete removeNode;
		this->next = newNext;
	} else 
	{
		throw std::invalid_argument("this node is last !!! cant't remove next node"); 
	}; 
}

template <typename ValueType> 
LinkedList<ValueType>::LinkedList()
	: _head(nullptr), _size(0)
{
}

template <typename ValueType> 
LinkedList<ValueType>::LinkedList(const LinkedList& copyList)
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

template <typename ValueType> 
LinkedList<ValueType>& LinkedList<ValueType>::operator=(const LinkedList& copyList)
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

template <typename ValueType> 
LinkedList<ValueType>::LinkedList(LinkedList&& moveList) noexcept
{
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;

}

template <typename ValueType> 
LinkedList<ValueType>& LinkedList<ValueType>::operator=(LinkedList&& moveList) noexcept
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

template <typename ValueType> 
LinkedList<ValueType>::~LinkedList()
{
	forceNodeDelete(_head);
}

template <typename ValueType> 
const ValueType& LinkedList<ValueType>::operator[](const size_t pos) const
{
	return getNode(pos)->value;
}

template <typename ValueType> 
ValueType& LinkedList<ValueType>::operator[](const size_t pos) 
{
	return getNode(pos)->value;
}

template <typename ValueType> 
typename LinkedList<ValueType>::Node* LinkedList<ValueType>::getNode(const size_t pos) const
{	
	// assert(pos >= 0);
	assert(pos < this->_size);
	
	Node* bufNode = this->_head;
	for (int i = 0; i < pos; ++i) {
		bufNode = bufNode->next;
	}

	return bufNode;
}

template <typename ValueType> 
void LinkedList<ValueType>::insert(const size_t pos, const ValueType& value)
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

template <typename ValueType> 
void LinkedList<ValueType>::insertAfterNode(Node* node, const ValueType& value)
{
	node->insertNext(value);
	++_size;
}

template <typename ValueType> 
void LinkedList<ValueType>::pushBack(const ValueType& value)
{
	if (_size == 0) {
		pushFront(value);
		return;
	}
	insert(_size, value);
}

template <typename ValueType> 
void LinkedList<ValueType>::pushFront(const ValueType& value)
{
	_head = new Node(value, _head);
	++_size;
}

template <typename ValueType> 
void LinkedList<ValueType>::removeFront() 
{
	Node* newHead = this->_head->next;
	delete this->_head;
	this->_head = newHead;
	--_size;
}

template <typename ValueType> 
void LinkedList<ValueType>::remove(const size_t pos)
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

template <typename ValueType> 
void LinkedList<ValueType>::removeBack() 
{
	this->remove(_size - 1); // вероятно, можно лучше, но пока так
}

template <typename ValueType> 
void LinkedList<ValueType>::removeNextNode(Node* node)
{
	node->removeNext(); //а если нулевой указатель?
	--_size;
}

template <typename ValueType> 
long long int LinkedList<ValueType>::findIndex(const ValueType& value) const
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

template <typename ValueType> 
typename LinkedList<ValueType>::Node* LinkedList<ValueType>::findNode(const ValueType& value) const
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

template <typename ValueType> 
void LinkedList<ValueType>::reverse()
{
	Node* bufNode = this->_head;
	if (bufNode) 	
		while (bufNode->next)
		{
			pushFront(bufNode->next->value);
			this->removeNextNode(bufNode);
		}	
}

template <typename ValueType> 
LinkedList<ValueType> LinkedList<ValueType>::reverse() const // потестить ещё
{
	return this->getReverseList();
}

template <typename ValueType> 
LinkedList<ValueType> LinkedList<ValueType>::getReverseList() const
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

template <typename ValueType> 
size_t LinkedList<ValueType>::size() const
{
	return _size;
}

template <typename ValueType> 
void LinkedList<ValueType>::forceNodeDelete(Node* node)
{
	if (node == nullptr) {
		return;
	}

	Node* nextDeleteNode = node->next;
	delete node;
	forceNodeDelete(nextDeleteNode);
	
}
#endif
