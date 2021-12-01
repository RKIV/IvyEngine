#include "Ivy.h"
#include "Leaf.h"
#include "Renderer/IDrawable.h"

std::unordered_map<std::string, Leaf*> Leaf::CDOMap = {};


void Leaf::LeafTick(float dt, std::vector<IDrawable*>& drawables)
{
	for (Leaf* L : children)
	{
		L->LeafTick(dt, drawables);
	}
	IDrawable* d = dynamic_cast<IDrawable*>(this);
	if (d != nullptr)
	{
		drawables.push_back(d);
	}
	for (Component* C : Components)
	{
		C->Tick(dt);
	}
	Tick(dt);
}

void Leaf::LeafInit()
{
	for (Leaf* L : children)
	{
		L->LeafInit();
	}
	for (Component* C : Components)
	{
		C->Init();
	}
	if(CDOMap.contains(CLASS)) CDOMap[CLASS]->Init();
	Init();
}


Leaf::Leaf()
	: CLASS("Leaf")
{}

Leaf::~Leaf()
{
	for (Leaf* L : children)
	{
		delete L;
	}
	for (Component* C : Components)
	{
		delete C;
	}
}

DirectX::XMMATRIX Leaf::GetTransformXM() const
{

	if (parent == nullptr)
	{
		return
			DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotation)) *
			DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));
	}
	else
	{
		return
			DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotation)) *
			parent->GetTransformXM() *
			DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));
	}
}

Leaf::Leaf(const char* Class)
	:CLASS(Class)
{}

void Leaf::Init()
{
	return;
}

void Leaf::Tick(float dt)
{
	return;
}
