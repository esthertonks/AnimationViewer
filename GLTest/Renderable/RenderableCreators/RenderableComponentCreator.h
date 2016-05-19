#pragma once

namespace render {

	enum RenderableComponetType {
		Bones,
		Normals
	};

	class RenderableComponentCreator abstract
	{
	public:
		RenderableComponentCreator();
		virtual ~RenderableComponentCreator() {};

		//virtual RenderableComponetType GetType() = 0;

		//virtual void Create() = 0;
	};

}

