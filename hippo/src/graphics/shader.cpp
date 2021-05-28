#include "hippo/graphics/shader.h"
#include "hippo/graphics/helpers.h"
#include "hippo/log.h"

#include "glad/glad.h"

namespace hippo::graphics
{
	Shader::Shader(const std::string& vertex, const std::string& fragment)
	{
		mProgramId = glCreateProgram(); HIPPO_CHECK_GL_ERROR;

		int status = GL_FALSE;
		char errorLog[512];

		uint32_t vertexShaderId = glCreateShader(GL_VERTEX_SHADER); HIPPO_CHECK_GL_ERROR;
		{
			const GLchar* glSource = vertex.c_str();
			glShaderSource(vertexShaderId, 1, &glSource, NULL); HIPPO_CHECK_GL_ERROR;
			glCompileShader(vertexShaderId); HIPPO_CHECK_GL_ERROR;
			glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &status); HIPPO_CHECK_GL_ERROR;
			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(vertexShaderId, sizeof(errorLog), NULL, errorLog); HIPPO_CHECK_GL_ERROR;
				HIPPO_ERROR("Vertex shader compilation error: {}", errorLog);
			}
			else
			{
				glAttachShader(mProgramId, vertexShaderId); HIPPO_CHECK_GL_ERROR;
			}
		}

		uint32_t fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); HIPPO_CHECK_GL_ERROR;
		if (status == GL_TRUE)
		{
			const GLchar* glSource = fragment.c_str();
			glShaderSource(fragmentShaderId, 1, &glSource, NULL); HIPPO_CHECK_GL_ERROR;
			glCompileShader(fragmentShaderId); HIPPO_CHECK_GL_ERROR;
			glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &status); HIPPO_CHECK_GL_ERROR;
			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(fragmentShaderId, sizeof(errorLog), NULL, errorLog); HIPPO_CHECK_GL_ERROR;
				HIPPO_ERROR("Fragment shader compilation error: {}", errorLog);
			}
			else
			{
				glAttachShader(mProgramId, fragmentShaderId); HIPPO_CHECK_GL_ERROR;
			}
		}

		HIPPO_ASSERT(status == GL_TRUE, "Error compiling shader");
		if (status == GL_TRUE)
		{
			glLinkProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
			glValidateProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
			glGetProgramiv(mProgramId, GL_LINK_STATUS, &status); HIPPO_CHECK_GL_ERROR;
			if (status != GL_TRUE)
			{
				glGetProgramInfoLog(mProgramId, sizeof(errorLog), NULL, errorLog); HIPPO_CHECK_GL_ERROR;
				HIPPO_ERROR("Shader link error: {}", errorLog); HIPPO_CHECK_GL_ERROR;
				glDeleteProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
				mProgramId = -1;
			}
		}

		glDeleteShader(vertexShaderId); HIPPO_CHECK_GL_ERROR;
		glDeleteShader(fragmentShaderId); HIPPO_CHECK_GL_ERROR;
	}

	Shader::~Shader()
	{
		glUseProgram(0); HIPPO_CHECK_GL_ERROR;
		glDeleteProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
	}

	void Shader::Bind()
	{
		glUseProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
	}

	void Shader::Unbind()
	{
		glUseProgram(0); HIPPO_CHECK_GL_ERROR;
	}

	void Shader::SetUniformInt(const std::string& name, int val)
	{
		glUseProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
		glUniform1i(GetUniformLocation(name), val); HIPPO_CHECK_GL_ERROR;
	}

	void Shader::SetUniformFloat(const std::string& name, float val)
	{
		glUseProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
		glUniform1f(GetUniformLocation(name), val); HIPPO_CHECK_GL_ERROR;
	}

	void Shader::SetUniformFloat2(const std::string& name, float val1, float val2)
	{
		glUseProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
		glUniform2f(GetUniformLocation(name), val1, val2); HIPPO_CHECK_GL_ERROR;
	}

	void Shader::SetUniformFloat3(const std::string& name, float val1, float val2, float val3)
	{
		glUseProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
		glUniform3f(GetUniformLocation(name), val1, val2, val3); HIPPO_CHECK_GL_ERROR;
	}

	void Shader::SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4)
	{
		glUseProgram(mProgramId); HIPPO_CHECK_GL_ERROR;
		glUniform4f(GetUniformLocation(name), val1, val2, val3, val4); HIPPO_CHECK_GL_ERROR;
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		auto it = mUniformLocations.find(name);
		if (it == mUniformLocations.end())
		{
			mUniformLocations[name] = glGetUniformLocation(mProgramId, name.c_str()); HIPPO_CHECK_GL_ERROR;
		}

		return mUniformLocations[name];
	}
}