#include "hippo/graphics/helpers.h"

namespace hippo::graphics
{
	void CheckGLError()
	{
		GLenum error = glGetError();
		while (error != GL_NO_ERROR)
		{
			std::string errorstr;
			switch (error)
			{
			case GL_INVALID_OPERATION:				errorstr = "INVALID_OPERATION";		break;
			case GL_INVALID_ENUM:					errorstr = "GL_INVALID_ENUM";		break;
			case GL_INVALID_VALUE:					errorstr = "GL_INVALID_VALUE";		break;
			case GL_OUT_OF_MEMORY:					errorstr = "GL_OUT_OF_MEMORY";		break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:	errorstr = "GL_INVALID_FRAMEBUFFER_OPERATION";		break;
			default:								errorstr = std::to_string(error);	break;
			}

			HIPPO_ERROR("OpenGL Error: {}", errorstr.c_str());
			HIPPO_ASSERT(false, "OpenGL Error!");

			error = glGetError();
		}
	}
}