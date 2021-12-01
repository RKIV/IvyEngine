#pragma once

#include "Bindable.h"
#include <vector>

/** A buffer storing a list of indices into the vertex buffer that will formulate triangles */
class IndexBuffer : public Bindable
{
public:
	/**
	 * Creates an IndexBuffer
	 * @param indices - A vector of indices into the vertex buffer
	 */
	IndexBuffer(const std::vector<unsigned short>& indices);
	/**
	 * Binds the index buffer to the Input Assembler
	 */
	virtual void Bind() override;
	/**
	* Index count getter
	 * @return The number of indices in the index buffer
	 */
	UINT GetCount() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	UINT count;
};
