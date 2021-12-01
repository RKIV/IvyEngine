#pragma once
#include "Bindable.h"

/** Defines the type and order of primitives that are coming into the pipeline */
class Topology : public Bindable
{
public:
	/**
	 * @param type - Topology type
	 */
	Topology(D3D11_PRIMITIVE_TOPOLOGY type);
	/**
	 * Bind the topology type to the pipeline
	 */
	void Bind() override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};