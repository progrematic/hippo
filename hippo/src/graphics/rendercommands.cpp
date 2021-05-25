#include "graphics/rendercommands.h"
#include "graphics/helpers.h"
#include "log.h"

#include "graphics/mesh.h"
#include "graphics/shader.h"

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
}