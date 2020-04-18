 #pragma once


class GameObject
{
public:
	struct ModelPart
	{
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;
		UINT vertexCount;
		UINT indexCount;
		DXGI_FORMAT indexFormat;

		UINT vertexStride;
	};
	
	template<class VertexType, class IndexType>
	void SetMesh(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData);

	template<class VertexType, class IndexType>
	void SetMesh(ID3D11Device* device, const std::vector<VertexType>& vertices, const std::vector<IndexType>& indices);

	void SetMesh(ID3D11Device* device, const void* vertices, UINT vertexSize, UINT vertexCount,
		const void* indices, UINT indexCount, DXGI_FORMAT indexFormat);

	void SetWorldMatrix(const DirectX::XMFLOAT4X4& world);
	void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX world);


	void Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect)const;

	void SetDebugName(const std::string& name);

	std::string Name;
	ModelPart m_Model;
private:
	DirectX::XMFLOAT4X4 m_WorldMatrix;							// 世界矩阵
};
// #include <string>
// #include <vector>
// #include "Graphics/Buffer.h"
// #include <DirectXMath.h>
// using namespace DirectX;
// class GameObject
// {
// 	//uint32_t id;
// 	
// };
// 
// struct VertexPosColor
// {
// 	DirectX::XMFLOAT3 Position;
// 	DirectX::XMFLOAT3 Normal;
// 	DirectX::XMFLOAT2 TexCoords;
// 	
// 	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
// };
// 
// enum Component
// {
// 	COMPONENT_NONE = 0,
// 	COMPONENT_TRANSFORM = 1 << 0
// };
// struct Transform
// {
// 	XMFLOAT3 pos;
// 	XMFLOAT3 angle;
// 	XMFLOAT3 scale;
// };
// struct ModelPart
// {
// 	VertexBuffer vertexBuffer;
// 	IndexBuffer indexBuffer;
// 	UINT vertexCount;
// 	UINT indexCount;
// 	DXGI_FORMAT indexFormat;
// 	
// };
// struct CB
// {
// 	DirectX::XMMATRIX world;
// 	DirectX::XMMATRIX view;
// 	DirectX::XMMATRIX proj;
// };
// struct World
// {
// 	const int num = 100;
// 
// 	std::vector<UINT> mask;
// 
// 	std::vector<Transform> trans;
// 
// 	std::vector<std::string> objPath;
// 
//  	std::vector<ModelPart> model;
//  	
//  	std::vector<ConstantBuffer<CB>> constantBuffer;
// 
// 	World()
// 	{
// 		mask.resize(num);
// 
// 		trans.resize(num);
// 
// 		model.resize(num);
// 
// 		constantBuffer.resize(num);
// 	}
// };
// 
// class GameObject
// {
// 	UINT id;
// 
// };
// 
// struct Name
// {
// 	std::string name;
// };
// 
// size_t CreateEntity(World* world)
// {
// 	size_t i = 0;
// 	for (auto& item : world->mask)
// 	{
// 		if (item == COMPONENT_NONE)
// 			return i;
// 		i++;
// 	}
// }
// size_t CreateTest(World* world, ID3D11Device* device, const void* vertices, UINT vertexSize, UINT vertexCount, const void* indices, UINT indexCount, DXGI_FORMAT indexFormat)
// {
// 	auto id = CreateEntity(world);
// 
// 	world->mask[id] = COMPONENT_TRANSFORM;
// 
// 	world->trans[id].pos = XMFLOAT3(0, 0, 0);
// 	world->trans[id].angle = XMFLOAT3(0, 0, 0);
// 	world->trans[id].scale = XMFLOAT3(1, 1, 1);
// 
// 	world->objPath[id] = objPath;
// 
// 	world->model[id].vertexBuffer.Create(device, vertices, vertexSize, vertexCount);
// 	world->model[id].vertexCount = vertexCount;
// 	world->model[id].indexBuffer.Create(device, indices, indexCount, indexFormat);
// 	world->model[id].indexCount = indexCount;
// 	world->model[id].indexFormat = indexFormat;
// 
// 	// 初始化常量缓冲区的值
// 	world->constantBuffer[id].Create(device); 
// 	
// }
// #define RENDER_MASK (COMPONENT_TRANSFORM)
// 
// void RenderFunction(World* world,ID3D11DeviceContext* context)
// {
// 	int i = 0;
// 	for (auto& item: world->mask)
// 	{
// 		if ((item & RENDER_MASK) == RENDER_MASK)
// 		{
// 			CB m_CBuffer;
// 			m_CBuffer.world = XMMatrixIdentity();	// 单位矩阵的转置是它本身
// 			m_CBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
// 				XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
// 				XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
// 				XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
// 			));
// 			m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, 4.f/3, 1.0f, 1000.0f));
// 
// 			world->constantBuffer[i].SetData(context, m_CBuffer);
// 
// 
// 		}
// 
// 		i++;
// 	}
// }

template<class VertexType, class IndexType>
inline void GameObject::SetMesh(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData)
{
	SetMesh(device, meshData.vertexVec, meshData.indexVec);
}

template<class VertexType, class IndexType>
inline void GameObject::SetMesh(ID3D11Device* device, const std::vector<VertexType>& vertices, const std::vector<IndexType>& indices)
{
	static_assert(sizeof(IndexType) == 2 || sizeof(IndexType) == 4, "The size of IndexType must be 2 bytes or 4 bytes!");
	static_assert(std::is_unsigned<IndexType>::value, "IndexType must be unsigned integer!");

	SetMesh(device,
		vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
		indices.data(), (UINT)indices.size(),
		(sizeof(IndexType) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);
}
