///**
//	This class is in a format suited to rendering. It stores information per vertex and splits vertices where information differs on triangle corners.
//	It will contain render batches based on material type.
//	This class stores the root of the mesh node hierarchy. The mesh hierarchy a flat linked list of parent->child nodes.
//*/
//#pragma once
//
//#include "../Container/LinkedList .h"
//#include "Batch.h"
//
//namespace render
//{
//
//class Batch;
//
//class BatchList : public container::LinkedList<Batch>
//{
//public:
//
//	BatchList::BatchList(){};
//	BatchList::~BatchList();
//
//	Batch* GetNodeHierarchy()
//	{
//		return m_root;
//	}
//
//	void Add(
//		Batch &node
//		)
//	{
//		Add(node);
//	}
//};
//}