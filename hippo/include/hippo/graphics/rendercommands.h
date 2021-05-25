#pragma once

#include <memory>

namespace hippo::graphics
{
	class Mesh;
	class Shader;

	namespace rendercommands
	{
		class RenderCommand
		{
		public:
			virtual void Execute() = 0;
			virtual ~RenderCommand() {}
		};

		class RenderMesh : public RenderCommand
		{
		public:
			RenderMesh(std::weak_ptr<Mesh> mesh, std::weak_ptr<Shader> shader)
				: mMesh(mesh)
				, mShader(shader)
			{}
			virtual void Execute();

		private:
			std::weak_ptr<Mesh> mMesh;
			std::weak_ptr<Shader> mShader;
		};
	}
}