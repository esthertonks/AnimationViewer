/**
	This class contains a mesh suitable for loaded information which is stored per triangle (and not per vertex as will be required to render
	This class stores the root of the mesh node hierarchy. The mesh hierarchy a flat linked list of parent->child nodes.
*/
#pragma once

#include "../Container/LinkedList .h"
#include "MeshNode.h"
#include "../Batch/BatchFwdDecl.h"

namespace mesh
{

class MeshNode;
class BoneNode;

class Mesh : public container::LinkedList<MeshNode>
{
public:

	Mesh::Mesh();
	Mesh::~Mesh();

	MeshNode* GetNodeHierarchy()
	{
		return m_root;
	}

	void AddChildNode(
		MeshNode &node
		)
	{
		Add(node);
	}

	int GetNumVerticesWithMaterialId(
		int materialId
	)
	{
		return m_numVerticesPerMaterial[materialId];
	}

	render::AppearanceTable &GetAppearances()
	{
		return m_appearanceTable;
	}


private:
	friend import::FBXImport; // Friend as the import class needs direct access to these arrays. All other classes accessing a mesh node should use the access function provided.
	render::AppearanceTable m_appearanceTable; // Mapping of material id's to material names
	std::vector<unsigned int> m_numVerticesPerMaterial; // A count of the number of vertex indices per material batch. Necessary for creating batches later
};
}