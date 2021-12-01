#pragma once
#include "Platform/WIN32/Window.h"
#include "Util/ChiliTimer.h"
#include "Renderer/Graphics.h"

/** The application interface with lower level details such as graphics */
class IvyEngine
{
public:
	IvyEngine();
	/** Start the game loop
	* @return Error code on exit
	*/
	int Go(class IApplication* EntryApp);
	~IvyEngine();
private:
	/** Tick one frame */
	void DoFrame();
public:
	class Leaf* RootLeaf;
private:
	Window wnd;
	std::unique_ptr<Graphics> pGfx;
	ChiliTimer timer;
	class IApplication* EntryApp;
};