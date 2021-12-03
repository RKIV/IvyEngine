#include "Ivy.h"
#include "LCube.h"
#include "Renderer/Bindable/Bindables.h"

LCube::LCube()
{
	GenerateMesh();
}

void LCube::Init()
{

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{ 1.0f,0.0f,1.0f },
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f },
			{ 1.0f,1.0f,0.0f },
			{ 0.0f,1.0f,1.0f },
		}
	};
	AddBind(new PixelConstantBuffer<ConstantBuffer2>(cb2));

	LStaticMesh::Init();
}

void LCube::GenerateMesh()
{
	Vertices =
	{
		{ std::move(DX::XMFLOAT3(-1.0f, -1.0f, -1.0f)) },
		{ std::move(DX::XMFLOAT3(1.0f, -1.0f, -1.0f)) },
		{ std::move(DX::XMFLOAT3(-1.0f, 1.0f, -1.0f)) },
		{ std::move(DX::XMFLOAT3(1.0f, 1.0f, -1.0f)) },
		{ std::move(DX::XMFLOAT3(-1.0f, -1.0f, 1.0f)) },
		{ std::move(DX::XMFLOAT3(1.0f, -1.0f, 1.0f)) },
		{ std::move(DX::XMFLOAT3(-1.0f, 1.0f, 1.0f)) },
		{ std::move(DX::XMFLOAT3(1.0f, 1.0f, 1.0f)) }
	};

	Indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	PixelShaderPath = L"PixelShader.cso";
}
