#pragma once
#include "Platform/WIN32/IvyWin.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "Util/IvyException.h"
#include <memory>
#include <optional>


class Window
{
public:
	class Exception : public IvyException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	// Represents the Registered Windows Class
	// It's a singleton since we only need one Windows Class for our purposes
	class WindowClass
	{
	public:
		/**
		 * @returns The name of the windows class
		 */
		static const LPCWSTR GetName() noexcept;
		/**
		 * @returns The current hInstance
		 */
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		// Delete Copy Constructor
		WindowClass(const WindowClass&) = delete;
		// Delete Copy Assignment Operator
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const LPCWSTR wndClassName = L"Chili Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, LPCWSTR name);
	~Window();
	// Delete Copy Constructor
	Window(const Window&) = delete;
	// Delete Copy Assignemnt Operator
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<WPARAM> ProcessMessages();
	HWND GetHWND()
	{
		return hWnd;
	}
private:
	static LRESULT CALLBACK IvyWndProcSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK IvyWndProcThunk(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT IvyWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd;
	Mouse mouse;
private:
	int width;
	int height;
	HWND hWnd;
};


// error exception helper macro
#define IVYWND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define IVYWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )