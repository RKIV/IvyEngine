#include "Ivy.h"
#include "TransformConstantBuffer.h"
#include "Engine/Leaf/Leaf.h"

TransformConstantBuffer::TransformConstantBuffer(Leaf* parent)
	: vcbuf(), parent(parent), Bindable("TransformConstantBuffer")
{}

void  TransformConstantBuffer::Bind()
{
	vcbuf.Update(
		DirectX::XMMatrixTranspose(
			parent->GetTransformXM() * Graphics::GetInst()->GetProjection()
		)
	);
	vcbuf.Bind();
}