#include "Ivy.h"
#include "VertexShader.h"
#include <d3dcompiler.h>


VertexShader::VertexShader(const std::wstring& path)
	: Bindable("VertexShader")
{
	HRESULT hr;

	D3DReadFileToBlob(path.c_str(), &pByteCodeBlob);
	GFX_THROW_FAILED(GetDevice()->CreateVertexShader(
		pByteCodeBlob->GetBufferPointer(),
		pByteCodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	));
}

void VertexShader::Bind()
{
	GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const
{
	return pByteCodeBlob.Get();
}