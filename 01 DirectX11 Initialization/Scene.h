#pragma once
#include <vector>
#include <memory>
#include "Graphics/Buffer.h"
#include "Graphics/Geometry.h"
#include "Graphics/Effects.h"
#include "GUI/GUI.h"
using namespace std;
using namespace DirectX;


enum Component
{
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM = 1 << 0,
	COMPONENT_ROTATE = 1 << 1
};

class Scene
{
	const static UINT Num = 10;
	struct ModelPart
	{
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;
		UINT vertexCount;
		UINT indexCount;
		DXGI_FORMAT indexFormat;

		UINT vertexStride;
	};
	
public:
	Scene() :
		worldMats(Num),
		modelParts(Num),
		names(Num),
		masks(Num)
	{}

	friend class Editor;
	friend class LogicSystem;

	void SetMesh(int i,ID3D11Device* device, const void* vertices, UINT vertexSize, UINT vertexCount,
		const void* indices, UINT indexCount, DXGI_FORMAT indexFormat);

	template<class VertexType, class IndexType>
	void SetMesh(int i, ID3D11Device* device, const std::vector<VertexType>& vertices, const std::vector<IndexType>& indices)
	{
		static_assert(sizeof(IndexType) == 2 || sizeof(IndexType) == 4, "The size of IndexType must be 2 bytes or 4 bytes!");
		static_assert(std::is_unsigned<IndexType>::value, "IndexType must be unsigned integer!");

		SetMesh(i,device,
			vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
			indices.data(), (UINT)indices.size(),
			(sizeof(IndexType) == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);
	}

	template<class VertexType, class IndexType>
	void SetMesh(int i,ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData)
	{
		SetMesh(i,device, meshData.vertexVec, meshData.indexVec);
	}

	void Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect)const;


protected:
	UINT SelectedID = -1;
private:
	vector<XMFLOAT4X4> worldMats;
	vector<ModelPart> modelParts;
	vector<string> names;
	vector<UINT> masks;

	void DrawItem(int i, ID3D11DeviceContext* deviceContext, BasicEffect& effect)const;
};