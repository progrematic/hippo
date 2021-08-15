#pragma once

#include "Gameobjects/Gameobject.h"
#include "Gameobjects/Ball.h"

#include "hippo/graphics/vertex.h"
#include "hippo/graphics/shader.h"

#include "external/glm/glm.hpp"

#include <memory>

namespace factory
{
	// Test Mesh
	static std::vector<float> Vertex1{ 0.5f,  0.5f, 0.f };
	static std::vector<float> Vertex2{ 0.5f, -0.5f, 0.f };
	static std::vector<float> Vertex3{ -0.5f,  -0.5f, 0.f };
	static std::vector<float> Vertex4{ -0.5f,  0.5f, 0.f };
	
	static std::vector<uint32_t> Elements
	{
		0, 3, 1,
		1, 3, 2
	};
	
	static const char* VertexShader = R"(
					#version 410 core
					layout (location = 0) in vec3 position;
					out vec3 vpos;
					uniform mat4 model = mat4(1.0);
					void main()
					{
						vpos = position;
						gl_Position = model * vec4(position, 1.0);
					}
				)";

	static const char* FragmentShader = R"(
					#version 410 core
					out vec4 outColor;

					void main()
					{
						outColor = vec4(vec3(0.0), 1.0);
					}
				)";

	static const char* BallFragmentShader = R"(
					#version 410 core
					out vec4 outColor;
					in vec3 vpos;

					void main()
					{
						if (length(vpos) > 0.5) discard;
						outColor = vec4(vec3(0.0), 1.0);
					}
				)";

	static const char* BGFragmentShader = R"(
					#version 410 core
					out vec4 outColor;
					in vec3 vpos;

					uniform float iTime = 0.0;

					void main()
					{
						vec3 Scale1 = vec3(15, 0.4, 0.975);
						vec3 Scale2 = vec3(25, 0.8, 0.5);
						vec3 Scale3 = vec3(75, 3.2, 0.8);
    
						vec3 Color1 = vec3(1, 0.67256093, 0);
						vec3 Color2 = vec3(1, 0.7411765, 0);
						vec3 Color3 = vec3(1, 0.7411765, 0);
    
						float pos = length(vpos);
						float f1 = sin(pos * Scale1.x - iTime * Scale1.y);
						float f2 = sin(pos * Scale2.x - iTime * Scale2.y);
						float f3 = sin(pos * Scale3.x - iTime * Scale3.y);
   						vec3 col = vec3(1, 0.8078432, 0);

						if (f1 > Scale1.z) {
							col = Color1;
						}              
						else if (f2 > Scale2.z) {
							col = Color2;
						}
						else if (f3 > Scale3.z) {
    						col = Color3;
						}

						// Output to screen
						outColor = vec4(col,1.0);
					}
				)";

	std::shared_ptr<Gameobject> CreatePaddle(const glm::vec2& pos)
	{
		HIPPO_CREATE_VERTEX_BUFFER(vb, float);
		vb->PushVertex(Vertex1);
		vb->PushVertex(Vertex2);
		vb->PushVertex(Vertex3);
		vb->PushVertex(Vertex4);
		vb->SetLayout({ 3 });
		std::shared_ptr<hippo::graphics::VertexArray> va = std::make_shared<hippo::graphics::VertexArray>();
		va->PushBuffer(std::move(vb));
		va->SetElements(Elements);
		va->Upload();

		std::shared_ptr<hippo::graphics::Shader> shader = std::make_shared<hippo::graphics::Shader>(VertexShader, FragmentShader);
		
		glm::vec2 size{ 0.1, 0.3 };
		auto ret = std::make_shared<Gameobject>(va, shader, pos, size);
		return ret;
	}

	std::shared_ptr<Ball> CreateBall(const glm::vec2& pos)
	{
		HIPPO_CREATE_VERTEX_BUFFER(vb, float);
		vb->PushVertex(Vertex1);
		vb->PushVertex(Vertex2);
		vb->PushVertex(Vertex3);
		vb->PushVertex(Vertex4);
		vb->SetLayout({ 3 });
		std::shared_ptr<hippo::graphics::VertexArray> va = std::make_shared<hippo::graphics::VertexArray>();
		va->PushBuffer(std::move(vb));
		va->SetElements(Elements);
		va->Upload();

		std::shared_ptr<hippo::graphics::Shader> shader = std::make_shared<hippo::graphics::Shader>(VertexShader, BallFragmentShader);

		glm::vec2 size{ 0.075, 0.1 };
		auto ret = std::make_shared<Ball>(va, shader, pos, size);
		return ret;
	}

	std::shared_ptr<Gameobject> CreateBG()
	{
		HIPPO_CREATE_VERTEX_BUFFER(vb, float);
		vb->PushVertex(Vertex1);
		vb->PushVertex(Vertex2);
		vb->PushVertex(Vertex3);
		vb->PushVertex(Vertex4);
		vb->SetLayout({ 3 });
		std::shared_ptr<hippo::graphics::VertexArray> va = std::make_shared<hippo::graphics::VertexArray>();
		va->PushBuffer(std::move(vb));
		va->SetElements(Elements);
		va->Upload();

		std::shared_ptr<hippo::graphics::Shader> shader = std::make_shared<hippo::graphics::Shader>(VertexShader, BGFragmentShader);

		glm::vec2 pos{ 0.f ,0.f };
		glm::vec2 size{ 2.f, 2.f };
		auto ret = std::make_shared<Gameobject>(va, shader, pos, size);
		return ret;
	}
}