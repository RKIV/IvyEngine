#include "Ivy.h"
#include "Engine/Leaf/LPlane.h"
#include "Renderer/Bindable/Bindables.h"

LPlane::LPlane()
	: LPlane("LPlane", 1, 1)
{}

LPlane::LPlane(unsigned int divisions_x, unsigned int divisions_y)
	: LPlane("LPlane", divisions_x, divisions_y)
{}


LPlane::LPlane(const char* Class, unsigned int divisions_x, unsigned int divisions_y)
	: LStaticMesh(Class), divisions_x(divisions_x), divisions_y(divisions_y)
{

	assert(divisions_x >= 1);
	assert(divisions_y >= 1);

	constexpr float width = 2.0f;
	constexpr float height = 2.0f;
	const int nVertices_x = divisions_x + 1;
	const int nVertices_y = divisions_y + 1;

	Vertices.resize(nVertices_x * nVertices_y);

	// Generating vertices
	{
		const float side_x = width / 2.0f;
		const float side_y = height / 2.0f;
		const float divisionSize_x = width / float(divisions_x);
		const float divisionSize_y = height / float(divisions_y);
		const auto bottomLeft = DirectX::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

		for (int y = 0, i = 0; y < nVertices_y; y++)
		{
			const float y_pos = float(y) * divisionSize_y;
			for (int x = 0; x < nVertices_x; x++, i++)
			{
				const auto v = DX::XMVectorAdd(
					bottomLeft,
					DX::XMVectorSet(float(x) * divisionSize_x, y_pos, 0.0f, 0.0f)
				);
				DX::XMStoreFloat3(&Vertices[i].pos, v);
			}
		}
	}

	// Generating indices
	Indices.reserve((divisions_x * divisions_y) * 6);
	{
		const auto vxy2i = [nVertices_x](size_t x, size_t y)
		{
			return (unsigned short)(y * nVertices_x + x);
		};
		for (size_t y = 0; y < divisions_y; y++)
		{
			for (size_t x = 0; x < divisions_x; x++)
			{
				Indices.push_back(vxy2i(x, y));
				Indices.push_back(vxy2i(x, y + 1));
				Indices.push_back(vxy2i(x + 1, y));
				Indices.push_back(vxy2i(x + 1, y));
				Indices.push_back(vxy2i(x, y + 1));
				Indices.push_back(vxy2i(x + 1, y + 1));
			}
		}
	}


	PixelShaderPath = L"PixelShader.cso";
}




void LPlane::Init()
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

	if (divisions_x != 1 || divisions_y != 1)
	{
		AddBind(new VertexBuffer(Vertices));
		AddIndexBuffer(new IndexBuffer(Indices));
	}

	LStaticMesh::Init();
}