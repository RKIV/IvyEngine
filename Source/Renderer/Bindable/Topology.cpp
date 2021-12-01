#include "Ivy.h"
#include "Topology.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	:type(type), Bindable("Topology")
{}

void Topology::Bind()
{
	GetContext()->IASetPrimitiveTopology(type);
}
