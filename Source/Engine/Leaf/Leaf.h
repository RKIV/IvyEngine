#pragma once
#include "Util/IvyMath.h"
#include "Engine/Component/Component.h"
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>

#define GENERATE_LEAF(Class)\
public:\
	static Leaf* GetCDOStatic()\
	{\
		if(!CDOMap.contains(#Class)) Class##CDOInit();\
		return CDOMap[#Class]; \
	}\
	static LeafClass GetClassStatic()\
	{\
		return MakeLeafClass(GetCDOStatic());\
	}\
	virtual Leaf* Instantiate()\
	{\
		return new Class;\
	}\
protected:\
	static void Class##CDOInit()\
	{\
		CDOMap[#Class] = new Class;\
	}\
private:\
	virtual std::string GetCDO_ID()\
	{\
		if(!CDOMap.contains(#Class)) Class##CDOInit();\
		return #Class;\
	}


struct LeafClass
{
	friend class Leaf;
	Leaf* GetCDO()
	{
		return CDO;
	}
private:
	LeafClass(Leaf* CDO)
		:CDO(CDO)
	{}
	Leaf* CDO;
};


class Leaf
{
	GENERATE_LEAF(Leaf)

	friend class IvyEngine;
public:
	Leaf() = default;
	Leaf(const Leaf&) = delete;
	~Leaf();
	virtual DirectX::XMMATRIX GetTransformXM() const;

	template<typename L, typename... Args>
	L* AddSubLeaf(const char* SubName, Args&&... args)
	{
		L* new_l = new L(std::forward<Args>(args)...);
		Leaf* new_leaf = static_cast<Leaf*>(new_l);
		strcpy_s(new_leaf->Name, MAX_NAME_LENGTH, SubName);

		children.push_back(new_leaf);
		new_leaf->parent = this;
		return new_l;
	}

	Leaf* AddSubLeaf(LeafClass lc, const char* SubName);

	Leaf* AddSubLeaf(Leaf* lf)
	{
		assert(lf != nullptr);
		if (lf->parent == this) return lf;
		if (lf->parent != nullptr)
		{
			lf->parent->children.erase(std::remove(lf->parent->children.begin(), lf->parent->children.end(), lf), lf->parent->children.end());
		}
		lf->parent = this;
		children.push_back(lf);
		return lf;
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

	Leaf* GetCDO()\
	{
		return CDOMap[GetCDO_ID()];
	}

	LeafClass GetClass()
	{
		return LeafClass(GetCDO());
	}


protected:
	virtual void Init();
	/**
	 * The tick function handles is called once every frame and is meant to handle gameplay logic
	 * @param dt - Delta time of the frame
	 */
	virtual void Tick(float dt);
	static LeafClass MakeLeafClass(Leaf* CDO)
	{
		return LeafClass(CDO);
	}
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
protected:
	std::vector<Leaf*> children;
	std::vector<Component*> Components;
	Leaf* parent = nullptr;
	static constexpr unsigned int MAX_NAME_LENGTH = 30;
	char Name[MAX_NAME_LENGTH];
	static std::unordered_map<std::string, Leaf*> CDOMap;
private:
};




