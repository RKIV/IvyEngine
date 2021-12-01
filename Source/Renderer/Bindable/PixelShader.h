#pragma once
#include "Bindable.h"
#include <string>

/** Pixel shader that can be bound to the pipeline */
class PixelShader : public Bindable
{
public:
	/**
	 * @param path - The file path to the pixel shader cso file
	 */
	PixelShader(const std::wstring& path);
	/**
	 * Bind the pixel shader to the pipeline
	 */
	void Bind() override;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};