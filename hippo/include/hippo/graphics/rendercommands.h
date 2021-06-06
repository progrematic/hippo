#pragma once

#include <memory>

namespace hippo::graphics
{
	class Mesh;
	class Shader;
	class Framebuffer;

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
			virtual void Execute() override;

		private:
			std::weak_ptr<Mesh> mMesh;
			std::weak_ptr<Shader> mShader;
		};

		class PushFramebuffer : public RenderCommand
		{
		public:
			PushFramebuffer(std::weak_ptr<Framebuffer> framebuffer) : mFramebuffer(framebuffer) {}
			virtual void Execute() override;

		private:
			std::weak_ptr<Framebuffer> mFramebuffer;
		};

		class PopFramebuffer : public RenderCommand
		{
		public:
			PopFramebuffer() {}
			virtual void Execute() override;
		};
	}
}