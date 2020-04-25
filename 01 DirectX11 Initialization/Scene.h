#pragma once

#include <memory>
#include "Graphics/Buffer.h"
#include "Graphics/Geometry.h"
#include "Graphics/Effects.h"
#include "GUI/GUI.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/array.hpp>

using namespace std;
using namespace DirectX;

enum class Shape
{
	SPHER,
	BOX,
	CYLINDER,
	PLANE,
	OTHER
};
enum Component
{
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM = 1 << 0,
	COMPONENT_MODEL = 1 << 1,
	COMPONENT_ROTATE = 1 << 2
};

class Scene
{

	friend class boost::serialization::access;

	struct ModelPart
	{
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;
		UINT vertexCount;
		UINT indexCount;
		DXGI_FORMAT indexFormat;

		UINT vertexStride;
	};
	struct Material
	{
		std::string diffuse;
	};
public:
	const static UINT Num = 10;
	Scene() :
		worldMats(Num),
		modelParts(Num),
		names(Num),
		masks(Num),
		modelType(Num),
		materials(Num)
	{}

	friend class Editor;
	friend class LogicSystem;


	void Draw(ID3D11DeviceContext* deviceContext, BasicEffect& effect)const;

	void Serialize()const;

	void AntiSerialize();

protected:
	Scene& operator =(const Scene& s) 
 	{
 		this->names = s.names;
 		this->masks = s.masks;
 		this->modelType = s.modelType;
 		this->names = s.names;
 		this->worldMats = s.worldMats;
		this->materials = s.materials;

 		return *this;
 	}
	Scene(const Scene& s) = delete;
	
	void DrawItem(int i, ID3D11DeviceContext* deviceContext, BasicEffect& effect)const;
public:
	
	vector<array<float,16>> worldMats;
	vector<ModelPart> modelParts;
	vector<UINT> modelType;
	vector<string> names;
	vector<UINT> masks;
	vector<Material> materials;
};