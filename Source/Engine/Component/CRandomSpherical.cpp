#include "Ivy.h"
#include "CRandomSpherical.h"
#include "Util/IvyMath.h"
#include "Engine/Leaf/Leaf.h"
#include <random>



CRandomSpherical::CRandomSpherical(float r_min /*= 5.0f*/, float r_max /*= 15.0f*/, float rot_speed /*= 1.0f*/, float pos_speed /*= 5.0f*/)
	: r_min(r_min), r_max(r_max), rot_speed(rot_speed), pos_speed(pos_speed)
{
}

void CRandomSpherical::Init()
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 1.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.08f);
	std::uniform_real_distribution<float> rdist(r_min, r_max);

	r = rdist(rng);
	droll = ddist(rng);
	dpitch = ddist(rng);
	dyaw = ddist(rng);
	dphi = odist(rng);
	dtheta = odist(rng);
	dchi = odist(rng);
	chi = adist(rng);
	theta = adist(rng);
	phi = adist(rng);
}

void CRandomSpherical::Tick(float dt)
{
	roll += droll * dt * rot_speed;
	pitch += dpitch * dt * rot_speed;
	yaw += dyaw * dt * rot_speed;
	theta += dtheta * dt * pos_speed;
	phi += dphi * dt * pos_speed;
	chi += dchi * dt * pos_speed;

	parent->position.x = r * sin(phi) * cos(theta);
	parent->position.y = r * sin(phi) * sin(theta);
	parent->position.z = r * cos(phi);
	parent->rotation.x = roll;
	parent->rotation.y = pitch;
	parent->rotation.z = yaw;
}
