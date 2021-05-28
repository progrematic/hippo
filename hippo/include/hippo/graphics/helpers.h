#pragma once

#include "hippo/log.h"

#include "glad/glad.h"
#include <string>

namespace hippo::graphics
{
	void CheckGLError();
}

#ifndef HIPPO_CONFIG_RELEASE
#define HIPPO_CHECK_GL_ERROR hippo::graphics::CheckGLError();
#else
#define HIPPO_CHECK_GL_ERROR (void)0
#endif