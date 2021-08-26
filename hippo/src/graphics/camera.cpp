#include "hippo/graphics/camera.h"
#include "hippo/log.h"

#include "external/glm/gtc/matrix_transform.hpp"

namespace hippo::graphics
{
	Camera::Camera()
		: mProjectionMatrix(1.f)
		, mViewMatrix(1.f)
		, mAspectRatio(16.f / 9.f)
		, mHeight(1080.f)
		, mNear(0.f)
		, mFar(100.f)
	{
		RecalculateProjectionMatrix();
	}

	Camera::Camera(const Camera& other)
		: mAspectRatio(other.mAspectRatio)
		, mHeight(other.mHeight)
		, mNear(other.mNear)
		, mFar(other.mFar)
	{
		RecalculateProjectionMatrix();
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		if (mAspectRatio != aspectRatio)
		{
			mAspectRatio = aspectRatio;
			RecalculateProjectionMatrix();
		}
	}

	void Camera::SetHeight(float height)
	{
		if (mHeight != height)
		{
			mHeight = height;
			RecalculateProjectionMatrix();
		}
	}

	void Camera::SetNear(float _near)
	{
		if (mNear != _near)
		{
			mNear = _near;
			RecalculateProjectionMatrix();
		}
	}

	void Camera::SetFar(float _far)
	{
		if (mFar != _far)
		{
			mFar = _far;
			RecalculateProjectionMatrix();
		}
	}

	void Camera::SetOrtho(float height, float _near /*= 0.f*/, float _far /*= 100.f*/)
	{
		bool shouldRecalculate = false;

		if (mHeight != height)
		{
			mHeight = height;
			shouldRecalculate = true;
		}
		if (mNear != _near)
		{
			mNear = _near;
			shouldRecalculate = true;
		}
		if (mFar != _far)
		{
			mFar = _far;
			shouldRecalculate = true;
		}

		if (shouldRecalculate)
		{
			RecalculateProjectionMatrix();
		}
	}

	void Camera::RecalculateProjectionMatrix()
	{
		float halfwidth = mHeight * mAspectRatio * 0.5f;
		float halfheight = mHeight * 0.5f;
		mProjectionMatrix = glm::ortho(-halfwidth, halfwidth, -halfheight, halfheight, mNear, mFar);
	}

	void Camera::SetViewMatrix(const glm::vec3& pos, float rot)
	{
		glm::mat4 view = glm::mat4(1.f);
		view = glm::translate(view, pos);
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0, 0, 1));
		view = glm::inverse(view);
		SetViewMatrix(view);
	}

}