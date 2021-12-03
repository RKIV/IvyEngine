#pragma once
#include "Graphics.h"
#include "Renderer/Bindable/Bindable.h"
#include <memory>
#include <vector>
#include <unordered_map>

/** An interface for anything that needs to be drawn to the screen. DONT directly implement from IDrawable. Instead implement IDRawableBase<T> as the interface */
class IDrawable
{
public:
	IDrawable(class Leaf* Parent);
	IDrawable(const IDrawable&) = delete;
	/**
	 * Bind all necessary resources and draw the object to the screen
	 */
	void Draw();
	/**
	 * Add a resource that needs to be bound when about to be drawn to the screen
	 * @param bind - The Bindable to be bound
	 */
	void AddBind(Bindable* bind) noexcept;
	/**
	 * Add an index buffer to be bound when about to draw to the screen
	 * @param ibuf - The index buffer to be bound
	 */
	void AddIndexBuffer(class IndexBuffer* ibuf) noexcept;

	void AddBindCDO(Bindable* bind);
	void AddIndexBufferCDO(IndexBuffer* ibuf);
	void ClearBinds();
	virtual ~IDrawable();
private:
	const IndexBuffer* pIndexBuffer = nullptr;
	class Leaf* parent;
	std::unordered_map<std::string, Bindable*> binds;
};