#pragma once

#include <cstdint>

#include "external/glm/glm.hpp"

namespace hippo::graphics
{
	class Framebuffer
	{
	public:
		Framebuffer(uint32_t width, uint32_t height);
		~Framebuffer();

		inline uint32_t GetFbo() const { return mFbo; }
		inline uint32_t GetTextureId() const { return mTextureId; }
		inline uint32_t GetRenderbufferId() const { return mRenderbufferId; }
		inline const glm::ivec2& GetSize() { return mSize; }
		inline void SetClearColour(const glm::vec4& cc) { mClearColour = cc; }
		inline const glm::vec4& GetClearColour() { return mClearColour; }

	private:
		uint32_t mFbo;
		uint32_t mTextureId;
		uint32_t mRenderbufferId;

		glm::ivec2 mSize;
		glm::vec4 mClearColour;
	};
}