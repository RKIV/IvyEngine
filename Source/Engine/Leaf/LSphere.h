#pragma once
#include "Engine/Leaf/LStaticMesh.h"

class LSphere : public LStaticMesh
{
	GENERATE_LEAF(LSphere)

public:
	LSphere();
	LSphere(unsigned int latDiv, unsigned int longDiv);
	void Init() override;
	virtual void GenerateMesh(unsigned int latDiv, unsigned int longDiv);
private:
	unsigned int latDiv;
	unsigned int longDiv;
	static unsigned int latDivDefault;
	static unsigned int longDivDefault;
};