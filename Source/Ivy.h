#pragma once

#ifdef WIN32
#include "Platform/WIN32/IvyWin.h"
#endif

#ifdef BUILD_DLL
	#define IVY_API __declspec(dllexport)
#else
	#define IVY_API __declspec(dllimport)
#endif