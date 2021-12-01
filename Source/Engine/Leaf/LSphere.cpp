#include "Ivy.h"
#include "Engine/Leaf/LSphere.h"
#include "Renderer/Bindable/Bindables.h"

unsigned int LSphere::latDivDefault = 9;
unsigned int LSphere::longDivDefault = 9;

LSphere::LSphere()
	: LSphere("LSphere", latDivDefault, longDivDefault)
{}

LSphere::LSphere(unsigned int latDiv, unsigned int longDiv)
	: LSphere("LSphere", latDiv, longDiv)
{}

LSphere::LSphere(const char* Class, unsigned int latDiv, unsigned int longDiv)
	: LStaticMesh(Class), latDiv(latDiv), longDiv(longDiv)
{
	assert(latDiv >= 3);
	assert(longDiv >= 3);

	constexpr float radius = 1.0f;
	const auto base = DX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
	const float lattitudeAngle = PI / latDiv;
	const float longitudeAngle = 2.0f * PI / longDiv;

	for (unsigned int iLat = 1; iLat < latDiv; iLat++)
	{
		const auto latBase = DX::XMVector3Transform(
			base,
			DX::XMMatrixRotationX(lattitudeAngle * iLat)
		);
		for (unsigned int iLong = 0; iLong < longDiv; iLong++)
		{
			Vertices.emplace_back();
			auto v = DX::XMVector3Transform(
				latBase,
				DX::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			DX::XMStoreFloat3(&Vertices.back().pos, v);
		}
	}

	// Cap Vertices
	const unsigned short iNorthPole = (unsigned short)Vertices.size();
	Vertices.emplace_back();
	DX::XMStoreFloat3(&Vertices.back().pos, base);
	const unsigned short iSouthPole = (unsigned short)Vertices.size();
	Vertices.emplace_back();
	DX::XMStoreFloat3(&Vertices.back().pos, DX::XMVectorNegate(base));

	const auto calcIdx = [latDiv, longDiv](unsigned short iLat, unsigned short iLong)
	{ return iLat * longDiv + iLong; };
	for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
	{
		for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
		{
			Indices.push_back(calcIdx(iLat, iLong));
			Indices.push_back(calcIdx(iLat + 1, iLong));
			Indices.push_back(calcIdx(iLat, iLong + 1));
			Indices.push_back(calcIdx(iLat, iLong + 1));
			Indices.push_back(calcIdx(iLat + 1, iLong));
			Indices.push_back(calcIdx(iLat + 1, iLong + 1));
		}
		// Wrap Band
		Indices.push_back(calcIdx(iLat, longDiv - 1));
		Indices.push_back(calcIdx(iLat + 1, longDiv - 1));
		Indices.push_back(calcIdx(iLat, 0));
		Indices.push_back(calcIdx(iLat, 0));
		Indices.push_back(calcIdx(iLat + 1, longDiv - 1));
		Indices.push_back(calcIdx(iLat + 1, 0));
	}

	// Cap fans
	for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
	{
		// north
		Indices.push_back(iNorthPole);
		Indices.push_back(calcIdx(0, iLong));
		Indices.push_back(calcIdx(0, iLong + 1));
		// south
		Indices.push_back(calcIdx(latDiv - 2, iLong + 1));
		Indices.push_back(calcIdx(latDiv - 2, iLong));
		Indices.push_back(iSouthPole);
	}
	// Wrap triangles
	// North
	Indices.push_back(iNorthPole);
	Indices.push_back(calcIdx(0, longDiv - 1));
	Indices.push_back(calcIdx(0, 0));
	// South
	Indices.push_back(calcIdx(latDiv - 2, 0));
	Indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
	Indices.push_back(iSouthPole);

	PixelShaderPath = L"PixelShader.cso";
}

void LSphere::Init()
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

	if (latDiv != 9 || longDiv != 9)
	{
		AddBind(new VertexBuffer(Vertices));
		AddIndexBuffer(new IndexBuffer(Indices));
	}

	LStaticMesh::Init();
}