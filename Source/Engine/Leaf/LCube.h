#pragma once
#include "LStaticMesh.h"

/** Just a cube static mesh */
class LCube : public LStaticMesh
{
public:
	LCube();
	void Init() override;
protected:
	LCube(const char*);
};