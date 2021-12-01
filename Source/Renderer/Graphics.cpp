#include "Ivy.h"
#include "Graphics.h"
#include "Platform/WIN32/Window.h"
#include <d3dcompiler.h>
#include <cmath>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DcOmpiler.lib")

Graphics* Graphics::inst = nullptr;

Graphics::Graphics(HWND hWnd)
{
	if (inst != nullptr) return;
	inst = this;

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;

	HRESULT hr;

	// Creates Swap Chain, Device, and Device Context
	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,					// IDXGIAdapter
		D3D_DRIVER_TYPE_HARDWARE,	// Driver Type
		nullptr,					// If using software driver, then this is a handle to the software dll
		swapCreateFlags,			// Flags
		nullptr,					// Feature Levels
		0,							// Number of feature levels
		D3D11_SDK_VERSION,			// SDK Version
		&sd,						// Swap Chain Description
		&pSwap,						// Swap Chain interface to be filled
		&pDevice,					// Device Interface to be filled
		nullptr,					// Actual feature level being used
		&pContext					// Device Context Interface to be filled
	));

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), (void**)(&pBackBuffer)));
	GFX_THROW_FAILED(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	));

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_FAILED(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepthTex = {};
	descDepthTex.Width = 1280;
	descDepthTex.Height = 720;
	descDepthTex.MipLevels = 1u;
	descDepthTex.ArraySize = 1u;
	descDepthTex.Format = DXGI_FORMAT_D32_FLOAT;
	descDepthTex.SampleDesc.Count = 1u;
	descDepthTex.SampleDesc.Quality = 0u;
	descDepthTex.Usage = D3D11_USAGE_DEFAULT;
	descDepthTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&descDepthTex, nullptr, &pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 1280.0f;
	vp.Height = 720.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	hr = pDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&pDebug);
}

Graphics::~Graphics()
{
	pContext->ClearState();
	pContext->Flush();
	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	pDebug->Release();
}

void Graphics::EndFrame()
{
	HRESULT hr;

	GFX_THROW_FAILED(pSwap->Present(1u, 0u));

}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());


	const float color[] = { red, green, blue };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count)
{
	pContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}