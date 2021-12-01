#pragma once
#include "Renderer/Graphics.h"

class Bindable
{
	friend class IDrawable;
public:
	Bindable(const char* Class);
	/**
	 * Binds the resource to the pipeline. Usually you would do this before you perform a draw call
	 * on an object that needs this resource
	 */
	virtual void Bind() = 0;
	virtual ~Bindable() = default;
	const char* CLASS;
protected:
	/**
	 * Get the context of the current Graphics objects
	 * @return The current device context
	 */
	static ID3D11DeviceContext* GetContext() noexcept;
	/**
	 * Get the device of the current Graphics object
	 * @return The current graphics device
	 */
	static ID3D11Device* GetDevice() noexcept;
};