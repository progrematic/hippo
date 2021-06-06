#pragma once

#include "hippo/graphics/rendercommands.h"

#include <stack>
#include <queue>
#include <memory>

#include "external/glm/glm.hpp"

#define HIPPO_SUBMIT_RC(type, ...) std::move(std::make_unique<hippo::graphics::rendercommands::type>(__VA_ARGS__))

namespace hippo::managers
{
	class RenderManager
	{
		friend class graphics::rendercommands::PushFramebuffer;
		friend class graphics::rendercommands::PopFramebuffer;

	public:
		RenderManager() {}
		~RenderManager() {}

		void Initialize();
		void Shutdown();

		void Clear();
		void SetViewport(const glm::ivec4 dimensions);
		void SetClearColour(const glm::vec4 cc);
		void SetWireframeMode(bool enabled);

		void Submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rc);

		// Execute submitted RenderCommands _in the order they were received_.
		// We can extend the API if we need to mitigate performance impact
		void Flush();

	private:
		void PushFramebuffer(std::shared_ptr<graphics::Framebuffer> framebuffer);
		void PopFramebuffer();

	private:
		std::queue<std::unique_ptr<graphics::rendercommands::RenderCommand>> mRenderCommands;
		std::stack<std::shared_ptr<graphics::Framebuffer>> mFramebuffers;
	};
}