#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <type_traits>

#include "external/glm/glm.hpp"

namespace hippo::graphics
{
	class Shader;
	class Texture;
	class Material
	{
	public:
		Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture = nullptr);
		Material(const Material& other);
		~Material();

		inline Shader* GetShader() const { return mShader.get(); }
		inline Texture* GetTexture() const { return mTexture.get(); }

		void SetShader(std::shared_ptr<Shader> shader);
		void SetTexture(std::shared_ptr<Texture> texture);
		void UpdateShaderUniforms();

#define GETUNIFORMVALUE(mapName, defaultReturn) \
	const auto& it = mapName.find(name);\
	if (it != mapName.end())\
	{\
		return it->second;\
	}\
	return defaultReturn;

		template<typename T>
		inline T GetUniformValue(const std::string& name) const
		{
			if constexpr (std::is_same<T, int>()) { GETUNIFORMVALUE(mUniformInts, 0) }
			else if constexpr (std::is_same<T, float>()) { GETUNIFORMVALUE(mUniformFloats, 0.f) }
			else if constexpr (std::is_same<T, glm::vec2>()) { GETUNIFORMVALUE(mUniformFloat2s, glm::vec2(0.f)) }
			else if constexpr (std::is_same<T, glm::vec3>()) { GETUNIFORMVALUE(mUniformFloat3s, glm::vec3(0.f)) }
			else if constexpr (std::is_same<T, glm::vec4>()) { GETUNIFORMVALUE(mUniformFloat4s, glm::vec4(0.f)) }
			else if constexpr (std::is_same<T, glm::mat3>()) { GETUNIFORMVALUE(mUniformMat3s, glm::mat3(1.f)) }
			else if constexpr (std::is_same<T, glm::mat4>()) { GETUNIFORMVALUE(mUniformMat4s, glm::mat4(1.f)) }
			else
			{
				static_assert(std::is_same<T, std::false_type>(), "Unsupported data type in Material::GetUniformValue()");
			}
		}
#undef GETUNIFORMVALUE

		template<typename T>
		inline void SetUniformValue(const std::string& name, const T& val)
		{
			if constexpr (std::is_same<T, int>()) { mUniformInts[name] = val; }
			else if constexpr (std::is_same<T, float>()) { mUniformFloats[name] = val; }
			else if constexpr (std::is_same<T, glm::vec2>()) { mUniformFloat2s[name] = val; }
			else if constexpr (std::is_same<T, glm::vec3>()) { mUniformFloat3s[name] = val; }
			else if constexpr (std::is_same<T, glm::vec4>()) { mUniformFloat4s[name] = val; }
			else if constexpr (std::is_same<T, glm::mat3>()) { mUniformMat3s[name] = val; }
			else if constexpr (std::is_same<T, glm::mat4>()) { mUniformMat4s[name] = val; }
			else
			{
				static_assert(std::is_same<T, std::false_type>(), "Unsupported data type in Material::SetUniformValue()");
			}
		}

	private:
		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Texture> mTexture;

		// Data
		std::unordered_map<std::string, int> mUniformInts;
		std::unordered_map<std::string, float> mUniformFloats;
		std::unordered_map<std::string, glm::vec2> mUniformFloat2s;
		std::unordered_map<std::string, glm::vec3> mUniformFloat3s;
		std::unordered_map<std::string, glm::vec4> mUniformFloat4s;
		std::unordered_map<std::string, glm::mat3> mUniformMat3s;
		std::unordered_map<std::string, glm::mat4> mUniformMat4s;
	};
}