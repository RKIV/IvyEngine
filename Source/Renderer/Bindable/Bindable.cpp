#include "Ivy.h"
#include "Bindable.h"

Bindable::Bindable(const char* Class)
	:CLASS(Class)
{}

ID3D11DeviceContext* Bindable::GetContext() noexcept
{
	return Graphics::inst->pContext.Get();
}

ID3D11Device* Bindable::GetDevice() noexcept
{
	return Graphics::inst->pDevice.Get();
}