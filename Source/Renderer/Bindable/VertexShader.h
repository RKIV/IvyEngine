#pragma once
#include "Bindable.h"
#include <string>

/** Vertex buffer that can be bound to the pipeline */
class VertexShader : public Bindable
{
public:
	/**
	 * @param path - The filepath to the shader cso file
	 */
	VertexShader(const std::wstring& path);
	/**
	 * Bind the vertex shader to the pipeline
	 */
	void Bind() override;
	/**
	 * @return A pointer to the loaded in blob representing the vertex shader
	 */
	ID3DBlob* GetBytecode() const;
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};