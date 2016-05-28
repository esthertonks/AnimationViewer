#include "RenderableVertexListFactory.h"
#include "RenderableVertexList.h"
#include "../RenderableCreators/NormalsVertexListCreator.h"
#include "../RenderableCreators/BoneVertexListCreator.h"

namespace render 
{
	RenderableVertexListPtr RenderableVertexListFactory::CreateBoneVertexList(
		const glm::vec3 blueColour,
		const int pointSize
		)
	{
		BoneVertexListCreatorPtr boneVertexListCreatorPtr = render::BoneVertexListCreatorPtr(new BoneVertexListCreator(blueColour));
		VertexListCreatorBasePtr vertexListCreator = boost::dynamic_pointer_cast<render::VertexListCreatorBase>(boneVertexListCreatorPtr);
		return render::RenderableVertexListPtr(new render::RenderableVertexList(vertexListCreator, pointSize));

	}

	RenderableVertexListPtr RenderableVertexListFactory::CreateNormalsVertexList(
		mesh::MeshPtr mesh,
		const glm::vec3 colour,
		const float normalLength,
		const int pointSize
	)
	{
		NormalsVertexListCreatorPtr normalsVertexListCreatorPtr = render::NormalsVertexListCreatorPtr(new NormalsVertexListCreator(mesh, colour, normalLength));
		VertexListCreatorBasePtr vertexListCreator = boost::dynamic_pointer_cast<render::VertexListCreatorBase>(normalsVertexListCreatorPtr);
		return RenderableVertexListPtr(new RenderableVertexList(vertexListCreator, pointSize));
	}
}
