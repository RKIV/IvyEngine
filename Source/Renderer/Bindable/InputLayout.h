#pragma once

#include <vector>
#include "Bindable.h"

/**
 * An input layout determines what the format of the vertex data being passed into the vertex shader will be.
 */
class InputLayout : public Bindable
{
public:
	/**
	 * Creates an input layout resource
	 * @param layout - A vector of descriptors describing the vertex data layout, see https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
	 * @param pVertexShaderBytecode - The bytecode of the vertex shader, when the input layout is created it is compared against the bytecode and produces warnings if there is a possible incompatability
	 */
	InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
	/**
	 * Bind input layout to pipeline
	 */
	virtual void Bind() override;
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};