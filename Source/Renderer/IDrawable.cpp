#include "Ivy.h"
#include "IDrawable.h"
#include "Renderer/Bindable/IndexBuffer.h"
#include <cassert>
#include <unordered_set>
#include "Engine/Leaf/Leaf.h"

IDrawable::IDrawable(Leaf* Parent)
	:parent(Parent)
{
}

void IDrawable::Draw()
{
	IDrawable* CDO = dynamic_cast<IDrawable*>(parent->GetCDO());
	std::unordered_set<std::string> bound;
	for (auto& b : binds)
	{
		bound.insert(b.first);
		b.second->Bind();
	}
	for (auto& b : CDO->binds)
	{
		if (!bound.contains(b.first))
			b.second->Bind();
	}
	if (pIndexBuffer == nullptr)
	{
		Graphics::GetInst()->DrawIndexed(CDO->pIndexBuffer->GetCount());
	}
	else
	{
		Graphics::GetInst()->DrawIndexed(pIndexBuffer->GetCount());
	}
}

void IDrawable::AddBind(Bindable* bind) noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	if (binds.contains(bind->CLASS))
	{
		delete binds[bind->CLASS];
	}
	binds[bind->CLASS] = bind;
}

void IDrawable::AddBindCDO(Bindable* bind)
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	IDrawable* CDO = dynamic_cast<IDrawable*>(parent->GetCDO());
	// CDO should only be bound once
	if (CDO->binds.contains(bind->CLASS)) return;
	CDO->AddBind(bind);
}

void IDrawable::AddIndexBuffer(IndexBuffer* ibuf) noexcept
{
	pIndexBuffer = ibuf;
	if (binds.contains(ibuf->CLASS))
	{
		delete binds[ibuf->CLASS];
	}
	binds[ibuf->CLASS] = ibuf;
}

void IDrawable::AddIndexBufferCDO(IndexBuffer* ibuf)
{
	IDrawable* CDO = dynamic_cast<IDrawable*>(parent->GetCDO());
	// CDO should only be bound once
	if (CDO->binds.contains(ibuf->CLASS)) return;
	CDO->AddIndexBuffer(ibuf);
}

IDrawable::~IDrawable()
{
	for (auto& bind : binds)
	{
		delete bind.second;
	}
}

