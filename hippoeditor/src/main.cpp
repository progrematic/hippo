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
#include "hippo/graphics/camera.h"
#include "hippo/graphics/material.h"

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
	bool mImguiEnabled = true;
	std::shared_ptr<graphics::Camera> mCamera;
	glm::vec3 mCameraPos;
	float mCameraRot;

	std::shared_ptr<graphics::VertexArray> mVA;
	std::shared_ptr<graphics::Material> mMaterial1;
	std::shared_ptr<graphics::Material> mMaterial2;

	glm::vec2 mRectPos, mRectSize;

	// Asset Libraries
	core::AssetLibrary<graphics::VertexArray> mVALibrary;
	core::AssetLibrary<graphics::Shader> mShaderLibrary;
	core::AssetLibrary<graphics::Texture> mTextureLibrary;
	core::AssetLibrary<graphics::Material> mMaterialLibrary;

public:
	core::WindowProperties GetWindowProperties() override
	{
		core::WindowProperties props;
		props.title = "HippoEditor";
		props.w = 1280;
		props.h = 720;
		props.imguiProps.IsDockingEnabled = true;
		return props;
	}

	void Initialize() override
	{
		auto& window = Engine::Instance().GetWindow();
		window.SetShouldRenderToScreen(false);

		InitializeLibraries();

		mCamera = std::make_shared<graphics::Camera>();
		mCamera->SetHeight(2.f);
		mCamera->SetViewMatrix(mCameraPos, mCameraRot);
		
		mRectPos = glm::vec2(0.f);
		mRectSize = glm::vec2(1.f);

		mVA = mVALibrary.Get("Rect");
		mMaterial1 = mMaterialLibrary.Get("RectRed");
		mMaterial2 = mMaterialLibrary.Get("RectGreen");
    }

	void Shutdown() override
	{

	}
	
	void Update() override
	{
		if (input::Keyboard::KeyDown(HIPPO_INPUT_KEY_GRAVE))
		{
			mImguiEnabled = !mImguiEnabled;
			auto& window = Engine::Instance().GetWindow();
			window.SetShouldRenderToScreen(!mImguiEnabled);
		}
	}

	void Render() override
	{
		Engine::Instance().GetRenderManager().Submit(HIPPO_SUBMIT_RC(PushCamera, mCamera));

		{
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, { mRectPos.x, mRectPos.y, 0.f });
			model = glm::scale(model, { mRectSize.x, mRectSize.y, 0.f });
			Engine::Instance().GetRenderManager().Submit(HIPPO_SUBMIT_RC(RenderVertexArrayMaterial, mVA, mMaterial1, model));
		}
		{
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, { mRectPos.x + 2.f, mRectPos.y, 0.f });
			model = glm::scale(model, { mRectSize.x, mRectSize.y, 0.f });
			Engine::Instance().GetRenderManager().Submit(HIPPO_SUBMIT_RC(RenderVertexArrayMaterial, mVA, mMaterial2, model));
		}

		Engine::Instance().GetRenderManager().Submit(HIPPO_SUBMIT_RC(PopCamera));
	}

	void ImguiRender() override
	{
		if (mImguiEnabled)
		{
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
			if (ImGui::Begin("Controls"))
			{
				ImGui::DragFloat2("Rect Pos", glm::value_ptr(mRectPos), 0.01f);
				ImGui::DragFloat2("Rect Size", glm::value_ptr(mRectSize), 0.01f);

				ImGui::Separator();
				float camheight = mCamera->GetHeight();
				ImGui::DragFloat("Camera Height", &camheight, 0.5f);
				mCamera->SetHeight(camheight);
				glm::vec3 cameraPos = mCameraPos;
				float cameraRot = mCameraRot;
				ImGui::DragFloat3("Camera Pos", glm::value_ptr(cameraPos), 0.01f);
				ImGui::DragFloat("Camera Rot", &cameraRot, 1.f);
				if (cameraPos != mCameraPos || cameraRot != mCameraRot)
				{
					mCameraPos = cameraPos;
					mCameraRot = cameraRot;
					mCamera->SetViewMatrix(cameraPos, cameraRot);
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
				ImVec2 winsize = ImGui::GetWindowSize();
				glm::ivec2 arsize = window.GetSizeInAspectRatio((int)winsize.x - 15, (int)winsize.y - 35);
				ImVec2 size = { (float)arsize.x, (float)arsize.y };
				ImVec2 pos = {
					(winsize.x - size.x) * 0.5f,
					((winsize.y - size.y) * 0.5f) + 7
				};
				ImVec2 uv0 = { 0, 1 };
				ImVec2 uv1 = { 1, 0 };
				ImGui::SetCursorPos(pos);
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
					
					uniform mat4 proj = mat4(1.0);
					uniform mat4 view = mat4(1.0);
					uniform mat4 model = mat4(1.0);
					void main()
					{
						gl_Position = proj * view * model * vec4(position, 1.0);
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
					
					uniform mat4 proj = mat4(1.0);
					uniform mat4 view = mat4(1.0);
					uniform mat4 model = mat4(1.0);
					void main()
					{
						uvs = texcoords;
						gl_Position = proj * view * model * vec4(position, 1.0);
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

		// Material
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(mShaderLibrary.Get("Rect"));
			mat->SetUniformValue("col", glm::vec4(1, 0, 0, 1));
			mMaterialLibrary.Load("RectRed", mat);
		}
		{
			std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(mShaderLibrary.Get("Rect"));
			mat->SetUniformValue("col", glm::vec4(0, 1, 0, 1));
			mMaterialLibrary.Load("RectGreen", mat);
		}
	}
};

hippo::App* CreateApp()
{
	return new Editor();
}
