#include "hippo/graphics/rendercommands.h"
#include "hippo/graphics/helpers.h"
#include "hippo/engine.h"
#include "hippo/log.h"

#include "hippo/graphics/mesh.h"
#include "hippo/graphics/shader.h"
#include "hippo/graphics/framebuffer.h"

#include "glad/glad.h"

namespace hippo::graphics::rendercommands
{
	void RenderMesh::Execute()
	{
		std::shared_ptr<Mesh> mesh = mMesh.lock();
		std::shared_ptr<Shader> shader = mShader.lock();
		if (mesh && shader)
		{
			mesh->Bind();
			shader->Bind();

			if (mesh->GetElementCount() > 0)
			{
				glDrawElements(GL_TRIANGLES, mesh->GetElementCount(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount()); HIPPO_CHECK_GL_ERROR;
			}

			shader->Unbind();
			mesh->Unbind();
		}
		else
		{
			HIPPO_WARN("Attempting to execute RenderMesh with invalid data");
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