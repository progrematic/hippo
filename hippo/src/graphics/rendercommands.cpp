#include "hippo/graphics/rendercommands.h"
#include "hippo/graphics/helpers.h"
#include "hippo/engine.h"
#include "hippo/log.h"

#include "hippo/graphics/vertex.h"
#include "hippo/graphics/shader.h"
#include "hippo/graphics/texture.h"
#include "hippo/graphics/framebuffer.h"

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

}