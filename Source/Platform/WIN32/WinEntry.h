#include "Ivy.h"

#include "Engine/IvyEngine.h"
#include "IApplication.h"

extern IApplication* EntryApplication();

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	try
	{
		int r = IvyEngine{}.Go(EntryApplication());
		return r;
	}
	catch (const IvyException& e)
	{
		wchar_t what[512];
		mbstowcs_s(nullptr, what, 512, e.what(), _TRUNCATE);
		wchar_t type[256];
		mbstowcs_s(nullptr, type, 256, e.GetType(), _TRUNCATE);
		MessageBox(nullptr, what, type, MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		wchar_t what[512];
		mbstowcs_s(nullptr, what, 512, e.what(), _TRUNCATE);
		MessageBox(nullptr, what, L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}