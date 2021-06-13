#include "hippo/engine.h"
#include "hippo/main.h"
#include "hippo/log.h"

#include "hippo/graphics/framebuffer.h"

#include "hippo/input/keyboard.h"

#include "Factory.h"

#include <memory>

#include "external/imgui/imgui.h"

using namespace hippo;

class PongV1 : public App
{
private:
	std::shared_ptr<Gameobject> mBG;
	std::shared_ptr<Gameobject> mLeftPaddle, mRightPaddle;
	int mLeftPaddleUp, mLeftPaddleDown, mRightPaddleUp, mRightPaddleDown;
	float mPaddleSpeed;

	std::shared_ptr<Ball> mBall;
	bool mBallCollisionLeft;
	float mBallSpeed;

	float mBallMult = 0.1f;
	int mLeftScore = 0;
	int mRightScore = 0;
	int mPointsToWin = 5;

	bool mInGameplay = false;
	std::string mWinnerText = "";

public:
	core::WindowProperties GetWindowProperties() override
	{
		core::WindowProperties props;
		props.w = 800;
		props.h = 700;
		props.imguiProps.IsDockingEnabled = true;

		return props;
	}

	void Initialize() override
	{
		mBG = factory::CreateBG();
		mLeftPaddle = factory::CreatePaddle({ -1.f, 0.f });
		mRightPaddle = factory::CreatePaddle({ 1.f, 0.f });

		mLeftPaddleUp = HIPPO_INPUT_KEY_W;
		mLeftPaddleDown = HIPPO_INPUT_KEY_S;
		mRightPaddleUp = HIPPO_INPUT_KEY_UP;
		mRightPaddleDown = HIPPO_INPUT_KEY_DOWN;

		mBall = factory::CreateBall({ 0.f, 0.f });

		Reset(true);
	}

	void Shutdown() override
	{

	}
	
	void Update() override
	{
		if (input::Keyboard::KeyDown(HIPPO_INPUT_KEY_ESCAPE))
		{
			Engine::Instance().Quit();
			return;
		}

		mBG->Update();
		if (mInGameplay)
		{
			mLeftPaddle->Update();
			mRightPaddle->Update();
			mBall->Update();

			float mLeftMaxY = 1 - mLeftPaddle->GetSize().y / 2;
			float mLeftMinY = -1 + mLeftPaddle->GetSize().y / 2;
			float mRightMaxY = 1 - mRightPaddle->GetSize().y / 2;
			float mRightMinY = -1 + mRightPaddle->GetSize().y / 2;

			if (input::Keyboard::Key(mLeftPaddleUp) && mLeftPaddle->GetPos().y < mLeftMaxY)
			{
				mLeftPaddle->Move({ 0.f, mPaddleSpeed });
			}
			if (input::Keyboard::Key(mLeftPaddleDown) && mLeftPaddle->GetPos().y > mLeftMinY)
			{
				mLeftPaddle->Move({ 0.f, -mPaddleSpeed });
			}

			if (input::Keyboard::Key(mRightPaddleUp) && mRightPaddle->GetPos().y < mRightMaxY)
			{
				mRightPaddle->Move({ 0.f, mPaddleSpeed });
			}
			if (input::Keyboard::Key(mRightPaddleDown) && mRightPaddle->GetPos().y > mRightMinY)
			{
				mRightPaddle->Move({ 0.f, -mPaddleSpeed });
			}


			float mBallMaxY = 1 - mBall->GetSize().y / 2;
			float mBallMinY = -1 + mBall->GetSize().y / 2;
			mBall->Move(mBall->GetVel());
			if (mBall->GetPos().y >= mBallMaxY ||
				mBall->GetPos().y <= mBallMinY)
			{
				mBall->FlipVelY();
			}

			if (IsColliding(mBall->GetPos(), mBall->GetSize(), mLeftPaddle->GetPos(), mLeftPaddle->GetSize()) && !mBallCollisionLeft)
			{
				mBall->FlipVelX();
				mBallCollisionLeft = !mBallCollisionLeft;

				HandlePaddleCollision(mLeftPaddleUp, mLeftPaddleDown);
			}

			if (IsColliding(mBall->GetPos(), mBall->GetSize(), mRightPaddle->GetPos(), mRightPaddle->GetSize()) && mBallCollisionLeft)
			{
				mBall->FlipVelX();
				mBallCollisionLeft = !mBallCollisionLeft;

				HandlePaddleCollision(mRightPaddleUp, mRightPaddleDown);
			}

			if (mBall->GetPos().x - mBall->GetSize().x / 2 < -1.f)
			{
				mRightScore++;
				Reset(false);
				if (mRightScore >= mPointsToWin)
				{
					mWinnerText = "Right paddle wins!";
					mInGameplay = false;
				}
			}

			if (mBall->GetPos().x + mBall->GetSize().x / 2 > 1.f)
			{
				mLeftScore++;
				Reset(true);
				if (mLeftScore >= mPointsToWin)
				{
					mWinnerText = "Left paddle wins!";
					mInGameplay = false;
				}
			}
		}
	}

	void Render() override
	{
		mBG->Render();
		mLeftPaddle->Render();
		mRightPaddle->Render();
		mBall->Render();
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::Begin("GameView"))
		{
			if (ImGui::IsWindowHovered())
			{
				ImGui::CaptureMouseFromApp(false);
			}

			auto& window = Engine::Instance().GetWindow();

			ImGui::SetCursorPosX(25);
			ImVec2 size = { 750, 545 };
			ImVec2 uv0 = { 0, 1 };
			ImVec2 uv1 = { 1, 0 };
			ImGui::Image((void*)(intptr_t)window.GetFramebuffer()->GetTextureId(), size, uv0, uv1);
		}
		ImGui::End();

		if (ImGui::Begin("Info"))
		{
			ImGui::Text("Left Score: %d", mLeftScore);
			ImGui::Text("Right Score: %d", mRightScore);
			ImGui::Text("Ball Vel: %0.5f, %0.5f", mBall->GetVel().x, mBall->GetVel().y);
		}
		ImGui::End();

		if (ImGui::Begin("Menu"))
		{
			if (!mInGameplay)
			{
				if (ImGui::Button("Start Game"))
				{
					StartGame();
				}
				ImGui::Text("%s", mWinnerText.c_str());
			}
		}
		ImGui::End();
	}

private:
	bool IsColliding(const glm::vec2& posA, const glm::vec2& sizeA, const glm::vec2& posB, const glm::vec2& sizeB)
	{
		float mALeft	= posA.x - sizeA.x / 2;
		float mARight	= posA.x + sizeA.x / 2;
		float mATop		= posA.y + sizeA.y / 2;
		float mABot		= posA.y - sizeA.y / 2;

		float mBLeft	= posB.x - sizeB.x / 2;
		float mBRight	= posB.x + sizeB.x / 2;
		float mBTop		= posB.y + sizeB.y / 2;
		float mBBot		= posB.y - sizeB.y / 2;

		return (
			mALeft < mBRight && mARight > mBLeft &&
			mATop > mBBot && mABot < mBTop
		);
	}

	void HandlePaddleCollision(int upKey, int downKey)
	{
		if (mBall->GetVel().y > 0 && input::Keyboard::Key(upKey) ||
			mBall->GetVel().y < 0 && input::Keyboard::Key(downKey))
		{
			mBall->SetVel(mBall->GetVel() * (1 + mBallMult));
		}

		if (mBall->GetVel().y > 0 && input::Keyboard::Key(downKey) ||
			mBall->GetVel().y < 0 && input::Keyboard::Key(upKey))
		{
			mBall->SetVel(mBall->GetVel() * (1 - mBallMult));
		}
	}

	void StartGame()
	{
		mLeftScore = 0;
		mRightScore = 0;
		mInGameplay = true;
		mWinnerText = "";
	}

	void Reset(bool leftScored)
	{
		mPaddleSpeed = 0.01f;
		mBallSpeed = 0.005f;
		mBallCollisionLeft = leftScored;

		mBall->SetVel({ mBallSpeed * (leftScored ? 1 : -1), mBallSpeed });
		mBall->SetPos({ 0.f, 0.f });
		mLeftPaddle->SetPos({ -1.f, 0.f });
		mRightPaddle->SetPos({ 1.f, 0.f });
	}
};

App* CreateApp()
{
	return new PongV1();
}