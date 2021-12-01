#pragma once
#include "Bindable.h"
#include <vector>

/** A buffer which represents the list of vertices that will be used */
class VertexBuffer : public Bindable
{
public:
	/**
	 * @param vertices - A vector of vertices templated on a type V
	 */
	template<typename V>
	VertexBuffer(const std::vector<V>& vertices)
		: stride(sizeof(V)), Bindable("VertexBuffer")
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GFX_THROW_FAILED(GetDevice()->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}
	/**
	 * Bind the vertex buffer to the pipeline
	 */
	virtual void Bind() override;

private:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};