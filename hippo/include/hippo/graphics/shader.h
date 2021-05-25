#pragma once

#include <string>
#include <unordered_map>

namespace hippo::graphics
{
	class Shader
	{
	public:
		Shader(const std::string& vertex, const std::string& fragment);
		~Shader();

		void Bind();
		void Unbind();

		void SetUniformInt(const std::string& name, int val);
		void SetUniformFloat(const std::string& name, float val);
		void SetUniformFloat2(const std::string& name, float val1, float val2);
		void SetUniformFloat3(const std::string& name, float val1, float val2, float val3);
		void SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4);

	private:
		int GetUniformLocation(const std::string& name);

	private:
		uint32_t mProgramId;

		std::unordered_map<std::string, int> mUniformLocations;
	};
}