#include "Ivy.h"
#include "Window.h"
#include <sstream>

// Declaring Static Singleton Instance
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	: hInst(GetModuleHandle(nullptr))
{
	// Filling initialization data structure to register a windows class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = IvyWndProcSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	// Creates a windows class (different from a c++ class) which can be used to instantiate windows
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetName(), GetInstance());
}

const LPCWSTR Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(int width, int height, LPCWSTR name)
	: height(height), width(width)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// Create an instance of the windows class we registered
	hWnd = CreateWindowEx(
		0,															// Extended Style Bit Flags
		WindowClass::GetName(),										// Windows Class to instantiate
		name,														// Name of Window
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,					// Standard Style Bit Flags
		CW_USEDEFAULT, CW_USEDEFAULT,								// Window Position
		wr.right - wr.left, wr.bottom - wr.top,						// Window Size
		nullptr,													// Window Parent (would be another window)
		nullptr,													// Menu
		WindowClass::GetInstance(),									// Handle to current instance
		this														// Custom parameter
	);

	// Display the windows to the screen
	ShowWindow(hWnd, SW_SHOW);
	// Create graphics object
}

void Window::SetTitle(const std::string& title)
{
	SetWindowText(hWnd, (LPCWSTR)title.c_str());
}

std::optional<WPARAM> Window::ProcessMessages()
{
	// The message to be filled by GetMessage
	MSG msg;

	// GetMessage will only return 0 when it receives a quit message
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		// Ask windows to generate any aditional messages associated with this message
		// E.x. generating char messages from keydown messages
		TranslateMessage(&msg);
		// Send message to the WndProc (defined when we created the IvyWindow class) to be handeled
		DispatchMessage(&msg);
	}

	return {};
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::IvyWndProcSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (Msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// Set USERDATA so that we can access our (c++) window class from our custom WndProc
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// Set WndProc to be IvyWndProcThunk instead
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::IvyWndProcThunk));
		return pWnd->IvyWndProc(hWnd, Msg, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK Window::IvyWndProcThunk(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// Get Our Window Class instance from USERDATA
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// Forward the Message Handling to our member function
	return pWnd->IvyWndProc(hWnd, Msg, wParam, lParam);
}

LRESULT Window::IvyWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (Msg)
	{
	case WM_CLOSE:
		// Since we only have one window we also want to quit when we close the window
		PostQuitMessage(13);
		return 0;

	case WM_KILLFOCUS:
		kbd.ClearState();
		break;

	// We need to check SYSKEY as well since keys like ALT and F10 don't show up in regular keydown messagees

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		// Bit 30 of lParam represents whether or not this is an autorepeat keydown or an actual keydown
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;


	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// in client region -> log move, and log enter + capture mouse
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (mouse.LeftIsPressed() || mouse.RightIsPressed())
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}

	}


	// To get all default functionality we also call DefWindowProc at the end of our WndProc
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

// Window Exception Stuff
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	IvyException(line, file),
	hr(hr)
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Ivy Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	LPWSTR pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		pMsgBuf, 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	char msg[512];
	wcstombs_s(nullptr, msg, 512, pMsgBuf, _TRUNCATE);
	std::string errorString = msg;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}