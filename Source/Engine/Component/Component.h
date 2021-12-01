#pragma once

class Component
{
	friend class Leaf;
public:
	Component() = default;
	~Component() = default;
protected:
	virtual void Init();
	/**
	 * The tick function handles is called once every frame and is meant to handle gameplay logic
	 * @param dt - Delta time of the frame
	 */
	virtual void Tick(float dt);
protected:
	class Leaf* parent;
};