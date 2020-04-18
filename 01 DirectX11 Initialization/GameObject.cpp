#include "GameObject.h"

// 
// const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[3] = {
// 	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
// };

void GameObject::SetMesh(ID3D11Device* device, const void* vertices, UINT vertexSize, UINT vertexCount, const void* indices, UINT indexCount, DXGI_FORMAT indexFormat)
{
	m_Model.vertexStride = vertexSize;

	m_Model.vertexCount = vertexCount;
	m_Model.indexCount = indexCount;
	m_Model.indexFormat = indexFormat;
// 	m_Model
// 	m_Model.material.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
// 	m_Model.material.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
// 	m_Model.material.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_Model.vertexBuffer.Create(device, vertices, vertexSize, vertexCount);

	m_Model.indexBuffer.Create(device, indices, indexCount, indexFormat);
	// 设置索引缓冲区描述
// 	D3D11_BUFFER_DESC ibd;
// 	ZeroMemory(&ibd, sizeof(ibd));
// 	ibd.Usage = D3D11_USAGE_IMMUTABLE;
// 	if (indexFormat == DXGI_FORMAT_R16_UINT)
// 	{
// 		ibd.ByteWidth = indexCount * (UINT)sizeof(WORD);
// 	}
// 	else
// 	{
// 		ibd.ByteWidth = indexCount * (UINT)sizeof(DWORD);
// 	}
// 	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
// 	ibd.CPUAccessFlags = 0;
// 	// 新建索引缓冲区
// 	InitData.pSysMem = indices;
// 	HR(device->CreateBuffer(&ibd, &InitData, modelParts[0].indexBuffer.ReleaseAndGetAddressOf()));
}

void GameObject::SetWorldMatrix(const DirectX::XMFLOAT4X4& world)
{
	m_WorldMatrix = world;
}

void XM_CALLCONV GameObject::SetWorldMatrix(DirectX::FXMMATRIX world)
{
	XMStoreFloat4x4(&m_WorldMatrix, world);
}


void GameObject::Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect)
{
	UINT strides = m_Model.vertexStride;
	UINT offsets = 0;

	auto& part = m_Model;
	{
		// 设置顶点/索引缓冲区
		deviceContext->IASetVertexBuffers(0, 1, part.vertexBuffer.GetBufferAddress(), &strides, &offsets);
		deviceContext->IASetIndexBuffer(part.indexBuffer.GetBuffer(), part.indexFormat, 0);

		// 更新数据并应用
		effect.SetWorldMatrix(XMLoadFloat4x4(&m_WorldMatrix));
// 		effect.SetTextureDiffuse(part.texDiffuse.Get());
// 		effect.SetMaterial(part.material);

		effect.Apply(deviceContext);

		deviceContext->DrawIndexed(part.indexCount, 0, 0);
	}
}


void GameObject::SetDebugName(const std::string& name)
{
	m_Model.vertexBuffer.SetBufferName(name);

	m_Model.indexBuffer.SetBufferName(name);
}
