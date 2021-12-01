#pragma once
#include "Engine/Leaf/LStaticMesh.h"

class LSphere : public LStaticMesh
{
public:
	LSphere();
	LSphere(unsigned int latDiv, unsigned int longDiv);
	void Init() override;
protected:
	LSphere(const char*, unsigned int latDiv, unsigned int longDiv);
private:
	unsigned int latDiv;
	unsigned int longDiv;
	static unsigned int latDivDefault;
	static unsigned int longDivDefault;
};