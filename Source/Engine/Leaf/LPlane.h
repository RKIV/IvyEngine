#pragma once
#include "Engine/Leaf/LStaticMesh.h"

class LPlane : public LStaticMesh
{
public:
	LPlane();
	LPlane(unsigned int divisions_x, unsigned int divisions_y);
	void Init() override;
protected:
	LPlane(const char*, unsigned int divisions_x, unsigned int divisions_y);
private:
	int divisions_x;
	int divisions_y;
};