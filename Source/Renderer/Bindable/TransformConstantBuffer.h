#pragma once
#include "ConstantBuffers.h"
#include "Util/IvyMath.h"

/** A special type of constant buffer that will be bound to the vertex shader and represent the transform of the object */
class TransformConstantBuffer : public Bindable
{
public:
	/**
	 * @param parent - A pointer to the leaf that this transform will represent
	 */
	TransformConstantBuffer(class Leaf* parent);
	/**
	 * Bind the transform matrix to the pipeline
	 */
	void Bind() override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Leaf* parent;
};