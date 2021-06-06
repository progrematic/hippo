#include <iostream>
#include "hippo/engine.h"
#include "hippo/app.h"
#include "hippo/main.h"
#include "hippo/log.h"

#include "hippo/graphics/mesh.h"
#include "hippo/graphics/shader.h"
#include "hippo/graphics/framebuffer.h"

#include "hippo/input/mouse.h"
#include "hippo/input/keyboard.h"
#include "hippo/input/joystick.h"

#include "external/imgui/imgui.h"

using namespace hippo;

class Editor : public hippo::App
{
private:
	std::shared_ptr<graphics::Mesh> mMesh;
	std::shared_ptr<graphics::Shader> mShader;
	float xKeyOffset = 0.f;
	float yKeyOffset = 0.f;
	float keySpeed = 0.001f;

public:
	core::WindowProperties GetWindowProperties()
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
		mMesh = std::make_shared<graphics::Mesh>(&vertices[0], 4, 3, &elements[0], 6);

		// Test Shader
		const char* vertexShader = R"(
					#version 410 core
					layout (location = 0) in vec3 position;
					out vec3 vpos;
					uniform vec2 offset = vec2(0.5);
					void main()
					{
						vpos = position + vec3(offset, 0);
						gl_Position = vec4(position, 1.0);
					}
				)";

		const char* fragmentShader = R"(
					#version 410 core
					out vec4 outColor;
					in vec3 vpos;

					uniform vec3 color = vec3(0.0);
					uniform float blue = 0.5f;
					void main()
					{
						outColor = vec4(vpos.xy, blue, 1.0);
					}
				)";
		mShader = std::make_shared<graphics::Shader>(vertexShader, fragmentShader);
		mShader->SetUniformFloat3("color", 1, 0, 0);
    }

	void Shutdown() override
	{

	}
	
	void Update() override
	{
		int windowW = 0;
		int windowH = 0;
		Engine::Instance().GetWindow().GetSize(windowW, windowH);

		float xNorm = (float)input::Mouse::X() / (float)windowW;
		float yNorm = (float)(windowH - input::Mouse::Y()) / (float)windowH;

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
	}

	void Render() override
	{
		Engine::Instance().GetRenderManager().Submit(HIPPO_SUBMIT_RC(RenderMesh, mMesh, mShader));
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::Begin("RectPosX"))
		{
			ImGui::DragFloat("Rect Pos X", &xKeyOffset, 0.01f);
		}
		ImGui::End();

		if (ImGui::Begin("RectPosY"))
		{
			ImGui::DragFloat("Rect Pos Y", &yKeyOffset, 0.01f);
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
