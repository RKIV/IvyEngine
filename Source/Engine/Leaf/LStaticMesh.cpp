#include "Ivy.h"
#include "LStaticMesh.h"
#include "Renderer/Bindable/Bindables.h"

LStaticMesh::LStaticMesh(const char* Class)
	:Leaf(Class), IDrawable((Leaf *)this)
{}

LStaticMesh::LStaticMesh()
	: LStaticMesh("LStaticMesh")
{}

void LStaticMesh::Init()
{
	AddBindCDO(new VertexBuffer(Vertices));

	ID3DBlob* pvsbc;
	{
		VertexShader* pvs = new VertexShader(VertexShaderPath);
		pvsbc = pvs->GetBytecode();
		AddBindCDO(pvs);
	}

	AddBindCDO(new PixelShader(PixelShaderPath));

	AddIndexBufferCDO(new IndexBuffer(Indices));

	AddBindCDO(new InputLayout(VertexInputLayout, pvsbc));

	AddBindCDO(new Topology(TopologyType));

	AddBind(new TransformConstantBuffer(this));
}
