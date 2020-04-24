#include "CameraController.h"

#include <IMGUI/imgui.h>
#include <DXOthers/d3dUtil.h>
#include <memory>

void CameraController::UpdataCamera(Camera* camera, CameraMode mode, float dt)
{
	auto cam1st = dynamic_cast<FirstPersonCamera*>(camera);
	auto cam3rd = dynamic_cast<ThirdPersonCamera*>(camera);


	auto& io = ImGui::GetIO();
	if (io.MouseDown[1])
	{
		// 第一人称/自由摄像机的操作

		// 方向移动
		if (ImGui::IsKeyDown(Keys::W))
		{
			if (mode == CameraMode::FirstPerson)
				cam1st->Walk(dt * 3.0f);
			else
				cam1st->MoveForward(dt * 3.0f);
		}
		if (ImGui::IsKeyDown(Keys::S))
		{
			if (mode == CameraMode::FirstPerson)
				cam1st->Walk(dt * -3.0f);
			else
				cam1st->MoveForward(dt * -3.0f);
		}
		if (ImGui::IsKeyDown(Keys::A))
			cam1st->Strafe(dt * -3.0f);
		if (ImGui::IsKeyDown(Keys::D))
			cam1st->Strafe(dt * 3.0f);

		// 将位置限制在[-8.9f, 8.9f]的区域内
		// 不允许穿地

		// 视野旋转，防止开始的差值过大导致的突然旋转
		cam1st->Pitch(io.MouseDelta.y * dt * 1.25f);
		cam1st->RotateY(io.MouseDelta.x * dt * 1.25f);
	}
	// 	else if (m_CameraMode == CameraMode::ThirdPerson)
	// 	{
	// 		// 第三人称摄像机的操作
	// 
	// 		cam3rd->SetTarget(m_WoodCrate.GetPosition());
	// 
	// 		// 绕物体旋转
	// 		cam3rd->RotateX(mouseState.y * dt * 1.25f);
	// 		cam3rd->RotateY(mouseState.x * dt * 1.25f);
	// 		cam3rd->Approach(-mouseState.scrollWheelValue / 120 * 1.0f);
	// 	}

		// 更新观察矩阵
	camera->UpdateViewMatrix();

}

