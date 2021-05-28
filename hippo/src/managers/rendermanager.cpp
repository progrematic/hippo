#include "hippo/managers/rendermanager.h"
#include "hippo/graphics/helpers.h"
#include "hippo/log.h"

#include "glad/glad.h"

namespace hippo::managers
{
	void RenderManager::Initialize()
	{
		HIPPO_INFO("OpenGL Info:\n  Vendor:\t{}\n  Renderer:\t{}\n  Version:\t{}",
			(const char*)glGetString(GL_VENDOR),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VERSION));

		// Initialize OpenGL
		glEnable(GL_DEPTH_TEST); HIPPO_CHECK_GL_ERROR;
		glDepthFunc(GL_LEQUAL); HIPPO_CHECK_GL_ERROR;

		glEnable(GL_BLEND); HIPPO_CHECK_GL_ERROR;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); HIPPO_CHECK_GL_ERROR;

		SetClearColour(
			static_cast<float>(0x64) / static_cast<float>(0xFF),
			static_cast<float>(0x95) / static_cast<float>(0xFF),
			static_cast<float>(0xED) / static_cast<float>(0xFF),
			1
		);	// cornflower blue
	}

	void RenderManager::Shutdown()
	{
		while (mRenderCommands.size() > 0)
		{
			mRenderCommands.pop();
		}
	}

	void RenderManager::SetClearColour(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a); HIPPO_CHECK_GL_ERROR;
	}

	void RenderManager::Submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rc)
	{
		mRenderCommands.push(std::move(rc));
	}

	void RenderManager::Flush()
	{
		while (mRenderCommands.size() > 0)
		{
			auto rc = std::move(mRenderCommands.front());
			mRenderCommands.pop();

			rc->Execute();
		}
	}

	void RenderManager::Clear()
	{
		HIPPO_ASSERT(mRenderCommands.size() == 0, "Unflushed render commands in queue!");
		while (mRenderCommands.size() > 0)
		{
			mRenderCommands.pop();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); HIPPO_CHECK_GL_ERROR;
	}

	void RenderManager::SetWireframeMode(bool enabled)
	{
		if (enabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); HIPPO_CHECK_GL_ERROR;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); HIPPO_CHECK_GL_ERROR;
		}
	}

}