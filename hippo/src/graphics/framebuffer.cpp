#include "hippo/graphics/framebuffer.h"
#include "hippo/log.h"

#include "hippo/graphics/helpers.h"

#include "glad/glad.h"

namespace hippo::graphics
{
	Framebuffer::Framebuffer(uint32_t width, uint32_t height)
		: mFbo(0)
		, mTextureId(0)
		, mRenderbufferId(0)
		, mWidth(width)
		, mHeight(height)
		, mCCR(1)
		, mCCG(1)
		, mCCB(1)
		, mCCA(1)
	{
		glGenFramebuffers(1, &mFbo); HIPPO_CHECK_GL_ERROR;
		glBindFramebuffer(GL_FRAMEBUFFER, mFbo); HIPPO_CHECK_GL_ERROR;

		// Create colour texture
		glGenTextures(1, &mTextureId); HIPPO_CHECK_GL_ERROR;
		glBindTexture(GL_TEXTURE_2D, mTextureId); HIPPO_CHECK_GL_ERROR;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); HIPPO_CHECK_GL_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); HIPPO_CHECK_GL_ERROR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); HIPPO_CHECK_GL_ERROR;
		glBindTexture(GL_TEXTURE_2D, 0); HIPPO_CHECK_GL_ERROR;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0); HIPPO_CHECK_GL_ERROR;

		// Create depth/stencil renderbuffer
		glGenRenderbuffers(1, &mRenderbufferId); HIPPO_CHECK_GL_ERROR;
		glBindRenderbuffer(GL_RENDERBUFFER, mRenderbufferId); HIPPO_CHECK_GL_ERROR;
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight); HIPPO_CHECK_GL_ERROR;
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderbufferId); HIPPO_CHECK_GL_ERROR;

		// Check for completeness
		int32_t completeStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); HIPPO_CHECK_GL_ERROR;
		if (completeStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			HIPPO_ERROR("Failure to create framebuffer. Complete status: {}", completeStatus);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0); HIPPO_CHECK_GL_ERROR;
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &mFbo);
		mFbo = 0;
		mTextureId = 0;
		mRenderbufferId = 0;
	}
}