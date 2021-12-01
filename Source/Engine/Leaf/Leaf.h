#pragma once
#include "Util/IvyMath.h"
#include "Engine/Component/Component.h"
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>


class Leaf
{
	friend class IvyEngine;
public:
	Leaf();
	Leaf(const Leaf&) = delete;
	~Leaf();
	virtual DirectX::XMMATRIX GetTransformXM() const;

	template<typename L, typename... Args>
	L* AddSubLeaf(Args&&... args)
	{
		L* new_l = new L(std::forward<Args>(args)...);
		Leaf* new_leaf = static_cast<Leaf*>(new_l);
		if (!CDOMap.contains(new_leaf->CLASS))
		{
			CDOMap[new_leaf->CLASS] = nullptr;
			CDOMap[new_leaf->CLASS] = new L{};
		}

		children.push_back(new_leaf);
		new_leaf->parent = this;
		return new_l;
	}

	template<typename C, typename... Args>
	C* AddComponent(Args&&... args)
	{
		C* new_c = new C(std::forward<Args>(args)...);
		Component* new_component = static_cast<Component*>(new_c);
		Components.push_back(new_component);
		new_component->parent = this;
		return new_c;
	}

	Leaf* GetCDO()
	{
		return CDOMap[CLASS];
	}

protected:
	Leaf(const char* Class);
	virtual void Init();
	/**
	 * The tick function handles is called once every frame and is meant to handle gameplay logic
	 * @param dt - Delta time of the frame
	 */
	virtual void Tick(float dt);
private:
	/**
	 * Recursive Update. Calls all children ticks and then it's own. Also keeps a record of all drawables below it
	 * @param dt - Delta time of the frame
	 */
	void LeafTick(float dt, std::vector<class IDrawable*>& drawables);
	void LeafInit();
public:
	DX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	static std::unordered_map<std::string, Leaf*> CDOMap;
protected:
	std::vector<Leaf*> children;
	std::vector<Component*> Components;
	Leaf* parent = nullptr;
	std::string CLASS;
};


