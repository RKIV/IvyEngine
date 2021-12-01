#include "Ivy.h"
#include "InputLayout.h"
#include <cmath>



InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
	: Bindable("InputLayout")
{
	HRESULT hr;
	GFX_THROW_FAILED(GetDevice()->CreateInputLayout(layout.data(), (UINT)layout.size(), pVertexShaderBytecode->GetBufferPointer(), pVertexShaderBytecode->GetBufferSize(), &pInputLayout));
}

void InputLayout::Bind()
{
	GetContext()->IASetInputLayout(pInputLayout.Get());
}
