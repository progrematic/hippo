#include <iostream>
#include "hippo/engine.h"
#include "hippo/app.h"
#include "hippo/main.h"
#include "hippo/log.h"

#include "hippo/graphics/mesh.h"
#include "hippo/graphics/shader.h"
#include "hippo/graphics/framebuffer.h"
#include "hippo/graphics/texture.h"

#include "hippo/input/mouse.h"
#include "hippo/input/keyboard.h"
#include "hippo/input/joystick.h"

#include "external/imgui/imgui.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

using namespace hippo;

class Editor : public hippo::App
{
private:
	std::shared_ptr<graphics::Mesh> mMesh;
	std::shared_ptr<graphics::Shader> mShader;
	std::shared_ptr<graphics::Texture> mTexture;
	float xKeyOffset = 0.f;
	float yKeyOffset = 0.f;
	float keySpeed = 0.001f;

	glm::vec2 mRectPos, mRectSize;

public:
	core::WindowProperties GetWindowProperties() override
	{
		core::WindowProperties props;
		props.title = "HippoEditor";
		props.w = 800;
		props.h = 600;
		props.imguiProps.IsDockingEnabled = true;
		return props;
	}

	void Initialize() override
	{
		// Test Mesh
		float vertices[]
		{
			 0.5f,  0.5f, 0.f,
			 0.5f, -0.5f, 0.f,
			-0.5f, -0.5f, 0.f,
			-0.5f,  0.5f, 0.f
		};
		uint32_t elements[]
		{
			0, 3, 1,
			1, 3, 2
		};
		float texcoords[]
		{
			1.f, 1.f,
			1.f, 0.f,
			0.f, 0.f,
			0.f, 1.f
		};
		mMesh = std::make_shared<graphics::Mesh>(&vertices[0], 4, 3, &texcoords[0], &elements[0], 6);

		// Test Shader
		const char* vertexShader = R"(
					#version 410 core
					layout (location = 0) in vec3 position;
					layout (location = 1) in vec2 texcoords;
					out vec3 vpos;
					out vec2 uvs;
					uniform vec2 offset = vec2(0.5);
					uniform mat4 model = mat4(1.0);
					void main()
					{
						uvs = texcoords;
						vpos = position + vec3(offset, 0);
						gl_Position = model * vec4(position, 1.0);
					}
				)";

		const char* fragmentShader = R"(
					#version 410 core
					out vec4 outColor;
					in vec3 vpos;
					in vec2 uvs;

					uniform vec3 color = vec3(0.0);
					uniform float blue = 0.5f;
					uniform sampler2D tex;
					void main()
					{
						//outColor = vec4(vpos.xy, blue, 1.0);
						outColor = texture(tex, uvs);
					}
				)";
		mShader = std::make_shared<graphics::Shader>(vertexShader, fragmentShader);
		mShader->SetUniformFloat3("color", 1, 0, 0);

		mRectPos = glm::vec2(0.f);
		mRectSize = glm::vec2(1.f);

		// Texture
		mTexture = std::make_shared<graphics::Texture>("res/bro.png");
		mTexture->SetTextureFilter(graphics::TextureFilter::Nearest);
    }

	void Shutdown() override
	{

	}
	
	void Update() override
	{
		auto windowSize = Engine::Instance().GetWindow().GetSize();

		float xNorm = (float)input::Mouse::X() / (float)windowSize.x;
		float yNorm = (float)(windowSize.y - input::Mouse::Y()) / (float)windowSize.y;

		if (input::Keyboard::Key(HIPPO_INPUT_KEY_LEFT)) { xKeyOffset -= keySpeed; }
		if (input::Keyboard::Key(HIPPO_INPUT_KEY_RIGHT)) { xKeyOffset += keySpeed; }
		if (input::Keyboard::Key(HIPPO_INPUT_KEY_UP)) { yKeyOffset += keySpeed; }
		if (input::Keyboard::Key(HIPPO_INPUT_KEY_DOWN)) { yKeyOffset -= keySpeed; }

		if (input::Keyboard::KeyDown(HIPPO_INPUT_KEY_LEFT)) { xKeyOffset -= keySpeed * 100; }
		if (input::Keyboard::KeyDown(HIPPO_INPUT_KEY_RIGHT)) { xKeyOffset += keySpeed * 100; }

		if (input::Joystick::IsJoystickAvailable(0))
		{
			if (input::Joystick::GetButton(0, input::Joystick::Button::DPAD_Left)) { xKeyOffset -= keySpeed; }
			if (input::Joystick::GetButton(0, input::Joystick::Button::DPAD_Right)) { xKeyOffset += keySpeed; }
			if (input::Joystick::GetButton(0, input::Joystick::Button::DPAD_Up)) { yKeyOffset += keySpeed; }
			if (input::Joystick::GetButton(0, input::Joystick::Button::DPAD_Down)) { yKeyOffset -= keySpeed; }

			float blue = input::Joystick::GetAxis(0, input::Joystick::Axis::LeftTrigger);
			mShader->SetUniformFloat("blue", blue);
		}

		mShader->SetUniformFloat2("offset", xNorm + xKeyOffset, yNorm + yKeyOffset);

		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, { mRectPos.x, mRectPos.y, 0.f });
		model = glm::scale(model, { mRectSize.x, mRectSize.y, 0.f });
		mShader->SetUniformMat4("model", model);
	}

	void Render() override
	{
		Engine::Instance().GetRenderManager().Submit(HIPPO_SUBMIT_RC(RenderMeshTextured, mMesh, mTexture, mShader));
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::Begin("Rect Pos"))
		{
			ImGui::DragFloat2("Rect Pos", glm::value_ptr(mRectPos), 0.01f);
		}
		ImGui::End();

		if (ImGui::Begin("RectSize"))
		{
			ImGui::DragFloat2("Rect Size", glm::value_ptr(mRectSize), 0.01f);
		}

		ImGui::End();

		if (ImGui::Begin("GameView"))
		{
			if (ImGui::IsWindowHovered())
			{
				ImGui::CaptureMouseFromApp(false);
			}

			auto& window = Engine::Instance().GetWindow();

			ImVec2 size = { 480, 320 };
			ImVec2 uv0 = { 0, 1 };
			ImVec2 uv1 = { 1, 0 };
			ImGui::Image((void*)(intptr_t)window.GetFramebuffer()->GetTextureId(), size, uv0, uv1);
		}
		ImGui::End();
	}
};

hippo::App* CreateApp()
{
	return new Editor();
}
