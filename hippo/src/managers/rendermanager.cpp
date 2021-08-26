#include "hippo/managers/rendermanager.h"
#include "hippo/graphics/helpers.h"
#include "hippo/graphics/framebuffer.h"
#include "hippo/log.h"
#include "hippo/engine.h"

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

		SetClearColour({
			static_cast<float>(0x64) / static_cast<float>(0xFF),
			static_cast<float>(0x95) / static_cast<float>(0xFF),
			static_cast<float>(0xED) / static_cast<float>(0xFF),
			1}
		);	// cornflower blue
	}

	void RenderManager::Shutdown()
	{
		while (mRenderCommands.size() > 0)
		{
			mRenderCommands.pop();
		}
	}

	void RenderManager::SetViewport(const glm::ivec4 dimensions)
	{
		glViewport(dimensions.x, dimensions.y, dimensions.z, dimensions.w); HIPPO_CHECK_GL_ERROR;
	}

	void RenderManager::SetClearColour(const glm::vec4 cc)
	{
		glClearColor(cc.r, cc.g, cc.b, cc.a); HIPPO_CHECK_GL_ERROR;
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

	void RenderManager::PushFramebuffer(std::shared_ptr<graphics::Framebuffer> framebuffer)
	{
		HIPPO_ASSERT(framebuffer, "Framebuffer is null");
		mFramebuffers.push(framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetFbo()); HIPPO_CHECK_GL_ERROR;
		SetViewport({ 0, 0, framebuffer->GetSize().x, framebuffer->GetSize().y });

		auto cc = framebuffer->GetClearColour();
		glClearColor(cc.r, cc.g, cc.b, cc.a); HIPPO_CHECK_GL_ERROR;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); HIPPO_CHECK_GL_ERROR;
	}

	void RenderManager::PopFramebuffer()
	{
		HIPPO_ASSERT(mFramebuffers.size() > 0, "RenderManager::PopFramebuffer - empty stack");
		if (mFramebuffers.size() > 0)
		{
			mFramebuffers.pop();
			if (mFramebuffers.size() > 0)
			{
				auto nextfb = mFramebuffers.top();
				glBindFramebuffer(GL_FRAMEBUFFER, nextfb->GetFbo()); HIPPO_CHECK_GL_ERROR;
				SetViewport({ 0, 0, nextfb->GetSize().x, nextfb->GetSize().y });
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0); HIPPO_CHECK_GL_ERROR;
				auto& window = Engine::Instance().GetWindow();
				SetViewport({ 0, 0, window.GetSize().x, window.GetSize().y });
			}
		}
	}

	void RenderManager::PushCamera(std::shared_ptr<graphics::Camera> camera)
	{
		HIPPO_ASSERT(camera, "Camera is null");
		mCameras.push(camera);
	}

	void RenderManager::PopCamera()
	{
		HIPPO_ASSERT(mCameras.size() > 0, "RenderManager::PopCamera - empty stack");
		if (mCameras.size() > 0)
		{
			mCameras.pop();
		}
	}

	const hippo::graphics::Camera* RenderManager::GetActiveCamera() const
	{
		if (mCameras.size() > 0)
		{
			return mCameras.top().get();
		}
		return nullptr;
	}

}