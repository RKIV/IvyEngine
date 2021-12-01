#pragma once
#include "Engine/Component/Component.h"

class CRandomSpherical : public Component
{
public:
	CRandomSpherical(float r_min = 5.0f, float r_max = 15.0f, float rot_spped = 1.0f, float pos_speed = 5.0f);

protected:
	virtual void Init() override;
	virtual void Tick(float dt) override;
private:

	float r_min;
	float r_max;
	float rot_speed;
	float pos_speed;

	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};