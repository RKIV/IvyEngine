#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	/**
	 * Update the constant buffer
	 * @param consts - The data the buffer will be updated with
	 */
	void Update(const C& consts)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext()->Map(
			pConstantBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&msr
		);
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext()->Unmap(pConstantBuffer.Get(), 0u);
	}

	/**
	 * Creates and initializes a constant buffer
	 * @param consts - The data to initialize the constant buffer with
	 */
	ConstantBuffer(const C& consts)
		: Bindable("ConstantBuffer")
	{
		HRESULT hr;

		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0u;
		desc.ByteWidth = sizeof(consts);
		desc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = &consts;
		GFX_THROW_FAILED(GetDevice()->CreateBuffer(&desc, &sd, &pConstantBuffer));
	}

	/**
	 * Creates a constant buffer
	 */
	ConstantBuffer()
		: Bindable("ConstantBuffer")
	{
		HRESULT hr;

		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0u;
		desc.ByteWidth = sizeof(C);
		desc.StructureByteStride = 0u;

		GFX_THROW_FAILED(GetDevice()->CreateBuffer(&desc, nullptr, &pConstantBuffer));
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

/** A constant buffer that binds to the vertex shader */
template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	VertexConstantBuffer()
	{
		Bindable::CLASS = "VertexConstantBuffer";
	}
	VertexConstantBuffer(const C& consts)
		: ConstantBuffer<C>::ConstantBuffer(consts)
	{
		Bindable::CLASS = "VertexConstantBuffer";
	}
public:
	using ConstantBuffer<C>::ConstantBuffer;
	/**
	 * Bind the this vertex constant buffer to the pipeline
	 */
	void Bind() override
	{
		GetContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

/** A constant buffer that bins to the pixel shader */
template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	PixelConstantBuffer()
	{
		Bindable::CLASS = "PixelConstantBuffer";
	}
	PixelConstantBuffer(const C& consts)
		: ConstantBuffer<C>::ConstantBuffer(consts)
	{
		Bindable::CLASS = "PixelConstantBuffer";
	}
public:
	using ConstantBuffer<C>::ConstantBuffer;
	/**
	 * Bind the pixel constant buffer to the pipeline
	 */
	void Bind() override
	{
		GetContext()->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};