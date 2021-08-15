#include "Gameobject.h"

#include "hippo/engine.h"
#include "external/glm/gtc/matrix_transform.hpp"

void Gameobject::Update()
{
	static int frames = 0;
	mShader->SetUniformFloat("iTime", frames * 0.01f);
	frames++;
}

void Gameobject::Render()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, { mPos.x, mPos.y, 0.f });
	model = glm::scale(model, { mSize.x, mSize.y, 1.f });
	mShader->SetUniformMat4("model", model);
	hippo::Engine::Instance().GetRenderManager().Submit(HIPPO_SUBMIT_RC(RenderVertexArray, mVertexArray, mShader));
}