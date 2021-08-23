#include <iostream>
#include "hippo/engine.h"
#include "hippo/app.h"
#include "hippo/main.h"
#include "hippo/log.h"

#include "hippo/core/assetlibrary.h"

#include "hippo/graphics/shader.h"
#include "hippo/graphics/vertex.h"
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
	std::shared_ptr<graphics::VertexArray> mVA;
	std::shared_ptr<graphics::Shader> mShader;
	std::shared_ptr<graphics::Texture> mTexture;
	float xKeyOffset = 0.f;
	float yKeyOffset = 0.f;
	float keySpeed = 0.001f;

	glm::vec2 mRectPos, mRectSize;

	// Asset Libraries
	core::AssetLibrary<graphics::VertexArray> mVALibrary;
	core::AssetLibrary<graphics::Shader> mShaderLibrary;
	core::AssetLibrary<graphics::Texture> mTextureLibrary;

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
		InitializeLibraries();
		
		mRectPos = glm::vec2(0.f);
		mRectSize = glm::vec2(1.f);

		mVA = mVALibrary.Get("Rect");
		mShader = mShaderLibrary.Get("Rect");
		mTexture = mTextureLibrary.Get("Bro");
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
		Engine::Instance().GetRenderManager().Submit(HIPPO_SUBMIT_RC(RenderVertexArrayTextured, mVA, mTexture, mShader));
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::Begin("Rect Controles"))
		{
			ImGui::DragFloat2("Rect Pos", glm::value_ptr(mRectPos), 0.01f);
			ImGui::DragFloat2("Rect Size", glm::value_ptr(mRectSize), 0.01f);
		}
		ImGui::End();

		if (ImGui::Begin("Options"))
		{
			if (ImGui::Button("Rect"))
			{
				mVA = mVALibrary.Get("Rect");
				mShader = mShaderLibrary.Get("Rect");
			}
			ImGui::SameLine();
			if (ImGui::Button("RectTextured"))
			{
				mVA = mVALibrary.Get("TexturedRect");
				mShader = mShaderLibrary.Get("TexturedRect");
			}
			if (ImGui::Button("Bro"))
			{
				mTexture = mTextureLibrary.Get("Bro");
			}
			ImGui::SameLine();
			if (ImGui::Button("Bro2"))
			{
				mTexture = mTextureLibrary.Get("Bro2");
			}
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

		if (ImGui::Begin("Asset Library Viewer"))
		{
			ImVec4 datacol(0, 1, 0, 1);
			ImVec4 errorcol(1, 0, 0, 1);
			if (ImGui::TreeNode("Texture Library"))
			{
				for (const auto& kv : mTextureLibrary.GetAll())
				{
					std::string tmpstr = kv.first + "##AssetLibraryViewer.TextureLibrary";
					if (ImGui::TreeNode(tmpstr.c_str()))
					{
						graphics::Texture* tex = kv.second.get();
						if (tex)
						{
							ImGui::TextColored(datacol, "Use count: "); ImGui::SameLine();
							ImGui::Text("%03d", (int)kv.second.use_count());
							ImGui::TextColored(datacol, "Size: "); ImGui::SameLine();
							ImGui::Text("%dx%d", tex->GetWidth(), tex->GetHeight());
							ImGui::TextColored(datacol, "Channels: "); ImGui::SameLine();
							ImGui::Text("%d", tex->GetNumChannels());
							ImGui::TextColored(datacol, "Path: "); ImGui::SameLine();
							ImGui::Text("%s", tex->GetPath().c_str());
							ImVec2 size{ (float)tex->GetWidth(), (float)tex->GetHeight() };
							ImGui::Image((void*)(intptr_t)tex->GetId(), size, { 0, 1 }, { 1, 0 });
						}
						else
						{
							ImGui::TextColored(errorcol, "Invalid texture: %s", kv.first.c_str());
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Shader Library"))
			{
				for (const auto& kv : mShaderLibrary.GetAll())
				{
					std::string tmpstr = kv.first + "##AssetLibraryViewer.ShaderLibrary";
					if (ImGui::TreeNode(tmpstr.c_str()))
					{
						graphics::Shader* shader = kv.second.get();
						if (shader)
						{
							ImGui::TextColored(datacol, "Use count: "); ImGui::SameLine();
							ImGui::Text("%03d", (int)kv.second.use_count());
							tmpstr = "Vertex Source##AssetLibraryViewer.ShaderLibrary." + kv.first;
							if (ImGui::TreeNode(tmpstr.c_str()))
							{
								ImGui::TextWrapped("%s", shader->GetVertexShaderSource().c_str());
								ImGui::TreePop();
							}
							tmpstr = "Fragment Source##AssetLibraryViewer.ShaderLibrary." + kv.first;
							if (ImGui::TreeNode(tmpstr.c_str()))
							{
								ImGui::TextWrapped("%s", shader->GetFragmentShaderSource().c_str());
								ImGui::TreePop();
							}
						}
						else
						{
							ImGui::TextColored(errorcol, "Invalid Shader: %s", kv.first.c_str());
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

	void InitializeLibraries()
	{
		// VertexArray
		{
			std::shared_ptr<graphics::VertexArray> va = std::make_shared<graphics::VertexArray>();
			{
				HIPPO_CREATE_VERTEX_BUFFER(vb, float);
				vb->PushVertex({ 0.5f, 0.5f, 0.f });
				vb->PushVertex({ 0.5f, -0.5f, 0.f });
				vb->PushVertex({ -0.5f, -0.5f, 0.f });
				vb->PushVertex({ -0.5f, 0.5f, 0.f });
				vb->SetLayout({ 3 });
				va->PushBuffer(std::move(vb));
			}

			va->SetElements({ 0, 3, 1, 1, 3, 2 });
			va->Upload();

			mVALibrary.Load("Rect", va);
		}
		{
			std::shared_ptr<graphics::VertexArray> va = std::make_shared<graphics::VertexArray>();
			{
				HIPPO_CREATE_VERTEX_BUFFER(vb, float);
				vb->PushVertex({ 0.5f, 0.5f, 0.f });
				vb->PushVertex({ 0.5f, -0.5f, 0.f });
				vb->PushVertex({ -0.5f, -0.5f, 0.f });
				vb->PushVertex({ -0.5f, 0.5f, 0.f });
				vb->SetLayout({ 3 });
				va->PushBuffer(std::move(vb));
			}
			{
				HIPPO_CREATE_VERTEX_BUFFER(vb, short);
				vb->PushVertex({ 1, 1 });
				vb->PushVertex({ 1, 0 });
				vb->PushVertex({ 0, 0 });
				vb->PushVertex({ 0, 1 });
				vb->SetLayout({ 2 });
				va->PushBuffer(std::move(vb));
			}

			va->SetElements({ 0, 3, 1, 1, 3, 2 });
			va->Upload();

			mVALibrary.Load("TexturedRect", va);
		}

		// Shader
		{
			const char* vertexShader = R"(
					#version 410 core
					layout (location = 0) in vec3 position;
					
					uniform mat4 model = mat4(1.0);
					void main()
					{
						gl_Position = model * vec4(position, 1.0);
					}
				)";

			const char* fragmentShader = R"(
					#version 410 core
					out vec4 outColor;

					uniform vec4 col = vec4(1.0);
					void main()
					{
						outColor = col;
					}
				)";
			mShaderLibrary.Load("Rect", std::make_shared<graphics::Shader>(vertexShader, fragmentShader));
		}
		{
			const char* vertexShader = R"(
					#version 410 core
					layout (location = 0) in vec3 position;
					layout (location = 1) in vec2 texcoords;
					out vec2 uvs;
					
					uniform mat4 model = mat4(1.0);
					void main()
					{
						uvs = texcoords;
						gl_Position = model * vec4(position, 1.0);
					}
				)";

			const char* fragmentShader = R"(
					#version 410 core
					out vec4 outColor;
					in vec2 uvs;

					uniform sampler2D tex;
					void main()
					{
						outColor = texture(tex, uvs);
					}
				)";
			mShaderLibrary.Load("TexturedRect", std::make_shared<graphics::Shader>(vertexShader, fragmentShader));
		}

		// Texture
		{
			std::shared_ptr<graphics::Texture> tex = std::make_shared<graphics::Texture>("res/bro.png");
			tex->SetTextureFilter(graphics::TextureFilter::Nearest);
			mTextureLibrary.Load("Bro", tex);
		}
		{
			std::shared_ptr<graphics::Texture> tex = std::make_shared<graphics::Texture>("res/bro2.png");
			tex->SetTextureFilter(graphics::TextureFilter::Nearest);
			mTextureLibrary.Load("Bro2", tex);
		}
	}
};

hippo::App* CreateApp()
{
	return new Editor();
}
