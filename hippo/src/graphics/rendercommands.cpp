#include "hippo/graphics/rendercommands.h"
#include "hippo/graphics/helpers.h"
#include "hippo/engine.h"
#include "hippo/log.h"

#include "hippo/graphics/vertex.h"
#include "hippo/graphics/shader.h"
#include "hippo/graphics/texture.h"
#include "hippo/graphics/framebuffer.h"
#include "hippo/graphics/camera.h"
#include "hippo/graphics/material.h"

#include "glad/glad.h"

namespace hippo::graphics::rendercommands
{
	void RenderVertexArray::Execute()
	{
		std::shared_ptr<VertexArray> va = mVertexArray.lock();
		std::shared_ptr<Shader> shader = mShader.lock();
		if (va && shader)
		{
			HIPPO_ASSERT(va->IsValid(), "Attempting to execute invalid RenderVertexArray - did you forget to call VertexArray::Upload()?");
			if (va->IsValid())
			{
				va->Bind();
				shader->Bind();

				// TODO: Convert camera matrices to leverage UBOs
				const auto& rm = Engine::Instance().GetRenderManager();
				const auto& cam = rm.GetActiveCamera();
				if (cam)
				{
					shader->SetUniformMat4("proj", cam->GetProjectionMatrix());
					shader->SetUniformMat4("view", cam->GetViewMatrix());
				}

				shader->SetUniformMat4("model", mModelMatrix);

				if (va->GetElementCount() > 0)
				{
					glDrawElements(GL_TRIANGLES, va->GetElementCount(), GL_UNSIGNED_INT, 0);
				}
				else
				{
					glDrawArrays(GL_TRIANGLE_STRIP, 0, va->GetVertexCount()); HIPPO_CHECK_GL_ERROR;
				}

				shader->Unbind();
				va->Unbind();
			}
		}
		else
		{
			HIPPO_WARN("Attempting to execute RenderVertexArray with invalid data");
		}
	}

	void RenderVertexArrayTextured::Execute()
	{
		std::shared_ptr<VertexArray> va = mVertexArray.lock();
		std::shared_ptr<Texture> texture = mTexture.lock();
		std::shared_ptr<Shader> shader = mShader.lock();
		if (va && texture && shader)
		{
			HIPPO_ASSERT(va->IsValid(), "Attempting to execute invalid RenderVertexArrayTextured - did you forget to call VertexArray::Upload()?");
			if (va->IsValid())
			{
				va->Bind();
				texture->Bind();
				shader->Bind();

				// TODO: Convert camera matrices to leverage UBOs
				const auto& rm = Engine::Instance().GetRenderManager();
				const auto& cam = rm.GetActiveCamera();
				if (cam)
				{
					shader->SetUniformMat4("proj", cam->GetProjectionMatrix());
					shader->SetUniformMat4("view", cam->GetViewMatrix());
				}

				shader->SetUniformMat4("model", mModelMatrix);

				if (va->GetElementCount() > 0)
				{
					glDrawElements(GL_TRIANGLES, va->GetElementCount(), GL_UNSIGNED_INT, 0);
				}
				else
				{
					glDrawArrays(GL_TRIANGLE_STRIP, 0, va->GetVertexCount()); HIPPO_CHECK_GL_ERROR;
				}

				shader->Unbind();
				texture->Unbind();
				va->Unbind();
			}
		}
		else
		{
			HIPPO_WARN("Attempting to execute RenderVertexArrayTextured with invalid data");
		}
	}

	void RenderVertexArrayMaterial::Execute()
	{
		std::shared_ptr<VertexArray> va = mVertexArray.lock();
		std::shared_ptr<Material> mat = mMaterial.lock();
		if (va && mat)
		{
			HIPPO_ASSERT(va->IsValid(), "Attempting to execute invalid RenderVertexArrayMaterial - did you forget to call VertexArray::Upload()?");
			if (va->IsValid())
			{
				va->Bind();

				Shader* shader = mat->GetShader();
				Texture* texture = mat->GetTexture();
				HIPPO_ASSERT(shader, "Attempting to execute invalid RenderVertexArrayMaterial - shader is nullptr");
				if (shader)
				{
					mat->UpdateShaderUniforms();
					shader->Bind();
					if (texture)
					{
						texture->Bind();
					}

					// TODO: Convert camera matrices to leverage UBOs
					const auto& rm = Engine::Instance().GetRenderManager();
					const auto& cam = rm.GetActiveCamera();
					if (cam)
					{
						shader->SetUniformMat4("proj", cam->GetProjectionMatrix());
						shader->SetUniformMat4("view", cam->GetViewMatrix());
					}

					shader->SetUniformMat4("model", mModelMatrix);

					if (va->GetElementCount() > 0)
					{
						glDrawElements(GL_TRIANGLES, va->GetElementCount(), GL_UNSIGNED_INT, 0);
					}
					else
					{
						glDrawArrays(GL_TRIANGLE_STRIP, 0, va->GetVertexCount()); HIPPO_CHECK_GL_ERROR;
					}

					if (texture)
					{
						texture->Unbind();
					}
					shader->Unbind();
				}

				va->Unbind();
			}
		}
		else
		{
			HIPPO_WARN("Attempting to execute RenderVertexArrayMaterial with invalid data");
		}
	}

	void PushFramebuffer::Execute()
	{
		std::shared_ptr<Framebuffer> fb = mFramebuffer.lock();
		if (fb)
		{
			Engine::Instance().GetRenderManager().PushFramebuffer(fb);
		}
		else
		{
			HIPPO_WARN("Attempting to execute PushFramebuffer with invalid data");
		}
	}

	void PopFramebuffer::Execute()
	{
		Engine::Instance().GetRenderManager().PopFramebuffer();
	}

	void PushCamera::Execute()
	{
		std::shared_ptr<Camera> cam = mCamera.lock();
		if (cam)
		{
			Engine::Instance().GetRenderManager().PushCamera(cam);
		}
		else
		{
			HIPPO_WARN("Attempting to execute PushCamera with invalid data");
		}
	}

	void PopCamera::Execute()
	{
		Engine::Instance().GetRenderManager().PopCamera();
	}

}