#pragma once
#include "Ivy.h"
#include <d3d11.h>
#include <wrl.h>
#include "Util/IvyMath.h"
#include "Platform/WIN32/Window.h"

#define GFX_THROW_FAILED(hrcall) if (FAILED( hr = (hrcall))) throw Window::Exception( __LINE__,__FILE__,hr )

/** A singleton class encapsulating a lot of the D3D11 boilerplate setup and draw calls */
class Graphics
{
	friend class Bindable;
public:
	/**
	 * @param hWnd - A handle to the window that is going to be drawn to
	 */
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	/**
	 * Present a frame to the screen
	 */
	void EndFrame();
	/**
	 * Clear the render buffer with
	 * @param red, green, blue - The RGB color to set the screen to
	 */
	void ClearBuffer(float red, float green, float blue) noexcept;
	/**
	 * Make a draw call to the graphics card
	 * @param count - The number of indices in the bound index buffer
	 */
	void DrawIndexed(UINT count);
	/**
	 * Set the projection matrix used in drawing
	 * @param proj - The projection matrix to set to
	 */
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	/**
	 * @return The projection matrix that is currently set
	 */
	DirectX::XMMATRIX GetProjection() const noexcept;
	/**
	 * @return The singleton instance 
	 */
	static Graphics* GetInst()
	{
		return inst;
	}
private:
	static Graphics* inst;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	ID3D11Debug* pDebug;

	DirectX::XMMATRIX projection;
};
