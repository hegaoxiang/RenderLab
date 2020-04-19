#include "Scene.h"
#include <CJSON/cJsonObject.h>
#include <fstream>
using namespace std;

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

void Scene::Serialize()
{
	neb::CJsonObject oJson;
	oJson.AddEmptySubArray("RenderData");

	for (UINT i = 0;i<Num;i ++)
	{
		if (masks[i] == COMPONENT_NONE)
			continue;
		decltype(oJson) item;
		item.Add("Name", names[i]);
		item.Add("Mask",masks[i]);
		item.Add("Model", modelParts[i].primary);
		oJson["RenderData"].Add(item);

	}
	ofstream file("scene.json");
	file << oJson.ToString();
	file.close();
	return;
}

void Scene::AntiSerialize(ID3D11Device* device)
{
	ifstream file("scene.json");
	if (!file.is_open())
	{
		return;
	}
	neb::CJsonObject oJson;
	string s;
	while (!file.eof())
	{
		char c;
		file >> c;
		s.append(1, c);
	}
	file.close();
	
	oJson.Parse(s);

	auto size = oJson["RenderData"].GetArraySize();

	auto& data = oJson["RenderData"];
	for (int i = 0; i < size; i++)
	{
		data[i].Get("Name", names[i]);
		data[i].Get("Mask", masks[i]);
		data[i].Get("Model", modelParts[i].primary);
		if (modelParts[i].primary == Geometry::PrimaryModel::BOX)
			SetMesh(i, device, Geometry::CreateBox());
	}
	return;
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