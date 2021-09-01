#pragma once

#include <memory>

#include "external/glm/glm.hpp"

namespace hippo::graphics
{
	class VertexArray;
	class Shader;
	class Texture;
	class Framebuffer;
	class Camera;
	class Material;

	namespace rendercommands
	{
		class RenderCommand
		{
		public:
			virtual void Execute() = 0;
			virtual ~RenderCommand() {}
		};

		class RenderVertexArray : public RenderCommand
		{
		public:
			RenderVertexArray(std::weak_ptr<VertexArray> vertexArray, std::weak_ptr<Shader> shader, const glm::mat4 modelMatrix = glm::mat4(1.f))
				: mVertexArray(vertexArray)
				, mShader(shader)
				, mModelMatrix(modelMatrix)
			{}
			virtual void Execute() override;

		private:
			std::weak_ptr<VertexArray> mVertexArray;
			std::weak_ptr<Shader> mShader;
			glm::mat4 mModelMatrix;
		};

		class RenderVertexArrayTextured : public RenderCommand
		{
		public:
			RenderVertexArrayTextured(std::weak_ptr<VertexArray> vertexArray, std::weak_ptr<Texture> texture, std::weak_ptr<Shader> shader, const glm::mat4 modelMatrix = glm::mat4(1.f))
				: mVertexArray(vertexArray)
				, mTexture(texture)
				, mShader(shader)
				, mModelMatrix(modelMatrix)
			{}
			virtual void Execute() override;

		private:
			std::weak_ptr<VertexArray> mVertexArray;
			std::weak_ptr<Texture> mTexture;
			std::weak_ptr<Shader> mShader;
			glm::mat4 mModelMatrix;
		};

		class RenderVertexArrayMaterial: public RenderCommand
		{
		public:
			RenderVertexArrayMaterial(std::weak_ptr<VertexArray> vertexArray, std::weak_ptr<Material> material, const glm::mat4 modelMatrix = glm::mat4(1.f))
				: mVertexArray(vertexArray)
				, mMaterial(material)
				, mModelMatrix(modelMatrix)
			{}
			virtual void Execute() override;

		private:
			std::weak_ptr<VertexArray> mVertexArray;
			std::weak_ptr<Material> mMaterial;
			glm::mat4 mModelMatrix;
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

		class PushCamera : public RenderCommand
		{
		public:
			PushCamera(std::weak_ptr<Camera> camera) : mCamera(camera) {}
			virtual void Execute() override;

		private:
			std::weak_ptr<Camera> mCamera;
		};

		class PopCamera : public RenderCommand
		{
		public:
			PopCamera() {}
			virtual void Execute() override;
		};
	}
}