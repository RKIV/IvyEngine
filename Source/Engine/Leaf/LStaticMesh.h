#pragma once
#include "Leaf.h"
#include "Renderer/IDrawable.h"
#include <string>

struct VertexBase
{
	DX::XMFLOAT3 pos;
};

class LStaticMesh : public Leaf, public IDrawable
{
public:
	LStaticMesh();
protected:
	LStaticMesh(const char*);
	virtual void Init() override;
protected:

	std::vector<VertexBase> Vertices;
	std::vector<unsigned short> Indices;
	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexInputLayout =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	D3D11_PRIMITIVE_TOPOLOGY TopologyType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	std::wstring VertexShaderPath = L"VMissing.cso";
	std::wstring PixelShaderPath = L"PMissing.cso";
};