//#pragma once
//
//#include "../Container/LinkedTree.h"
//#include <string>
//#include <fbxsdk.h>
//
//namespace import
//{
//	class FBXImport;
//}
//
//
//namespace mesh
//{
//
//enum NodeType
//{
//	MeshType,
//	BoneType
//};
//
//class Node : public container::LinkedTreeItem<Node>
//{
//public:
//
//	Node(){};
//	~Node(){};
//
//virtual NodeType GetType() = 0;
//
//void SetName(
//	const std::string& name
//	)
//{
//	m_name = name;
//};
//
//const std::string&	GetName()
//{		
//	return m_name;
//}
//
//void SetGlobalTransform(
//	FbxAMatrix &globalTransform
//	)
//{
//	m_globalTransform = globalTransform;			
//};
//
//const FbxAMatrix &GetGlobalTransform() const
//{
//	return m_globalTransform;
//};
//
//FbxAMatrix &GetGlobalTransform()
//{
//	return m_globalTransform;
//};
//
//private:
//	friend class import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.
//
//	std::string m_name;
//
//	// FBX SDK for higher precision
//	FbxAMatrix m_globalTransform;		// Transform from this node to the global model transform. This is updated each tick during animation
//};
//
//}