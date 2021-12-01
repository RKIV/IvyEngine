#include "Ivy.h"
#include "PixelShader.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(const std::wstring& path)
	: Bindable("PixelShader")
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	GFX_THROW_FAILED(GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
}

void PixelShader::Bind()
{
	GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
