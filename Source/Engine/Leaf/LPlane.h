#pragma once
#include "Engine/Leaf/LStaticMesh.h"

class LPlane : public LStaticMesh
{
	GENERATE_LEAF(LPlane)


public:
	LPlane();
	LPlane(unsigned int divisions_x, unsigned int divisions_y);
	virtual void GenerateMesh(unsigned int divisions_x, unsigned int divisions_y);
protected:
	void Init() override;
private:
	int divisions_x;
	int divisions_y;
};