#pragma once

#include <cstdint>

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
		inline void GetSize(uint32_t& w, uint32_t& h) { w = mWidth; h = mHeight; }
		inline void SetClearColour(float r, float g, float b, float a) { mCCR = r; mCCG = g; mCCB = b; mCCA = a; }
		inline void GetClearColour(float& r, float& g, float& b, float& a) { r = mCCR; g = mCCG; b = mCCB; a = mCCA; }

	private:
		uint32_t mFbo;
		uint32_t mTextureId;
		uint32_t mRenderbufferId;

		uint32_t mWidth, mHeight;
		float mCCR, mCCG, mCCB, mCCA;
	};
}