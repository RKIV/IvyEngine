#pragma once
#include "LStaticMesh.h"

/** Just a cube static mesh */
class LCube : public LStaticMesh
{
	GENERATE_LEAF(LCube)

public:
	LCube();
	void Init() override;
	virtual void GenerateMesh();
};