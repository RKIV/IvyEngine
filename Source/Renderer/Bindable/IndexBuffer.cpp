#include "Ivy.h"
#include "IndexBuffer.h"
#include "Platform/WIN32/Window.h"


IndexBuffer::IndexBuffer(const std::vector<unsigned short>& indices)
	: count((UINT)indices.size()), Bindable("IndexBuffer")
{
	HRESULT hr;

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = UINT(count * sizeof(unsigned short));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0u;
	desc.MiscFlags = 0u;
	desc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = indices.data();

	GFX_THROW_FAILED(GetDevice()->CreateBuffer(&desc, &sd, &pIndexBuffer));

}

void IndexBuffer::Bind()
{
	GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
