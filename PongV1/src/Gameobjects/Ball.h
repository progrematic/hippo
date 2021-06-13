#pragma once

#include "Gameobject.h"

class Ball : public Gameobject
{
public:
	Ball(std::shared_ptr<hippo::graphics::Mesh> mesh, std::shared_ptr<hippo::graphics::Shader> shader, const glm::vec2& pos, const glm::vec2& size)
		: Gameobject(mesh, shader, pos, size)
		, mVel(0.f)
	{}

	void SetVel(const glm::vec2& vel) { mVel = vel; }
	const glm::vec2& GetVel() const { return mVel; }

	void FlipVelX() { mVel.x *= -1; }
	void FlipVelY() { mVel.y *= -1; }

private:
	glm::vec2 mVel;
};