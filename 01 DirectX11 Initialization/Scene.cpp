#include "Scene.h"
#include "IMGUI/imgui_stdlib.h"
#include "IMGUI/ImGuizmo.h"

void Scene::SetMesh(int i, ID3D11Device* device, const void* vertices, UINT vertexSize, UINT vertexCount, const void* indices, UINT indexCount, DXGI_FORMAT indexFormat)
{
	auto& m_Model = modelParts[i];
	m_Model.vertexStride = vertexSize;

	m_Model.vertexCount = vertexCount;
	m_Model.indexCount = indexCount;
	m_Model.indexFormat = indexFormat;

	m_Model.vertexBuffer.Create(device, vertices, vertexSize, vertexCount);

	m_Model.indexBuffer.Create(device, indices, indexCount, indexFormat);
}



void Scene::OnGUI()
{

	ImGui::Begin("Hierarchy",nullptr,ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Box"))
			{
				
			}
			
			ImGui::EndMenu();
		}
		
		ImGui::EndMenuBar();
	}
	UINT i = 0;
	for (auto& item : names)
	{
		if (ImGui::Selectable(item.c_str(), SelectedID == i))
			SelectedID = i;
		i++;
	}

	ImGui::End();

	ImGui::Begin("Inspector");
	
	static bool showTransform = false;
	if (SelectedID != -1)
	{
		ImGui::InputText("Name", &names[SelectedID]);

		auto mask = masks[SelectedID];

		if (mask & COMPONENT_TRANSFORM)
		{
			showTransform = true;
		}
		/*if(mask )*/
	}
	ImGui::End();

	if (showTransform)
	{
		ImGui::Begin("TransForm");

		ImGui::Text("transform");
		ImGui::NewLine();
		

		static float translation[3];
		static float rotation[3];
		static float scale[3];
		ImGuizmo::DecomposeMatrixToComponents((float*)worldMats[SelectedID].m, translation, rotation, scale);
		ImGui::InputFloat3("Tr", translation, 3);
		ImGui::InputFloat3("Rt", rotation, 3);
		ImGui::InputFloat3("Sc", scale, 3);
		ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, (float*)worldMats[SelectedID].m);

		ImGui::End();
	}
	
}

void Scene::Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect)const
{
	int i = 0;
	for (auto& item : masks)
	{
		if (item & COMPONENT_TRANSFORM)
		{
			DrawItem(i, deviceContext, effect);
		}
		i++;
	}
}

void Scene::DrawItem(int i, ID3D11DeviceContext* deviceContext, BasicEffect& effect)const
{
	auto& modelPart = modelParts[i];
	auto& worldMat = worldMats[i];

	UINT strides = modelPart.vertexStride;
	UINT offsets = 0;

	// 设置顶点/索引缓冲区
	deviceContext->IASetVertexBuffers(0, 1, modelPart.vertexBuffer.GetBufferAddress(), &strides, &offsets);
	deviceContext->IASetIndexBuffer(modelPart.indexBuffer.GetBuffer(), modelPart.indexFormat, 0);

	// 更新数据并应用
	effect.SetWorldMatrix(XMLoadFloat4x4(&worldMat));

	effect.Apply(deviceContext);

	deviceContext->DrawIndexed(modelPart.indexCount, 0, 0);
}

UINT Scene::CreateEnity()
{
	for (size_t i = 0; i < Num; i++)
	{
		if (masks[i] == COMPONENT_NONE)
		{
			return i;
		}
	}

	HR(0);
}

UINT Scene::CreateBox(ID3D11Device* device)
{
	UINT i = CreateEnity();

	names[i] = "test" + i;
	
	XMStoreFloat4x4(&worldMats[i] ,XMMatrixIdentity());

	static int q = 0;
	if (q == 0)
	{
		q = 1;
		SetMesh(i, device, Geometry::CreateBox<VertexPosColor>());
	}
	else
	{
		std::vector<VertexPosColor>vertices =
		{
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
		};

		std::vector<DWORD> indices = {
			// 正面
			0, 1, 2,
			2, 3, 0,
			// 左面
			4, 5, 1,
			1, 0, 4,
			// 顶面
			1, 5, 6,
			6, 2, 1,
			// 背面
			7, 6, 5,
			5, 4, 7,
			// 右面
			3, 2, 6,
			6, 7, 3,
			// 底面
			4, 0, 3,
			3, 7, 4
		};
		SetMesh(i, device, vertices, indices);
		//AddComponent(i, COMPONENT_ROTATE);
	}
	AddComponent(i, COMPONENT_TRANSFORM);
	
	return i;
}
