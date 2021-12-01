#include "Ivy.h"
#include "VertexBuffer.h"
void VertexBuffer::Bind()
{
	const UINT offset = 0u;
	GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}
