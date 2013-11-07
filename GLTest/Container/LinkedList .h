#pragma once

#include <cstdlib>

namespace container
{
	template<class TYPE> class LinkedList
	{
	public:

	LinkedList::LinkedList()
	{
		m_root = NULL;
		m_end = NULL;
	};

	virtual LinkedList::~LinkedList()
	{
		for(TYPE *type = m_root; type != NULL; )
		{
			TYPE *temp = type;
			type = type->m_next;
			delete temp;
		}
	};


	virtual TYPE* GetRoot()
	{
		return m_root;
	}

	virtual void Add(
		TYPE &node
		)
	{
		if(m_root) // Hierarchy list is not empty - add to the end and move the end to the new node
		{
			m_end->m_next = &node;
			node.m_next = NULL;
			node.m_previous = m_end;
			m_end = m_end->m_next;
		}
		else // List is empty - add the new node to the root node
		{
			m_root = m_end = &node;
			node.m_previous = NULL;
			node.m_next = NULL;
		}
	}

	TYPE *m_root;
	TYPE *m_end;


	};

	template<class ITEM> class LinkedListItem
	{
	friend class LinkedList<ITEM>;
	public:

	LinkedListItem::LinkedListItem()
	{
		m_next = NULL;
		m_previous = NULL;	
	};

	virtual LinkedListItem::~LinkedListItem()
	{
		if(m_next)
		{
			delete m_next;
			m_next = NULL;
		}
		if(m_previous)
		{
			delete m_previous;
			m_previous = NULL;
		}
	};

	ITEM *m_next;
	ITEM *m_previous;
	};

}