#include "Ivy.h"
#include "IvyEngine.h"
#include "Engine/Leaf/LCube.h"
#include "Platform/WIN32/IApplication.h"

IvyEngine::IvyEngine()
	:
	wnd(1280, 720, L"Ivy Main Window")
{
	pGfx = std::make_unique<Graphics>(wnd.GetHWND());
	pGfx->SetProjection(DirectX::XMMatrixOrthographicLH(100.0f, 100.0f * 9.0f / 16.0f, 0.5f, 50.0f));
	//pGfx->SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f));
}

IvyEngine::~IvyEngine()
{
	delete RootLeaf;
	delete EntryApp;
	// Deleting CDOs
	for (std::pair< std::string, Leaf* > p : Leaf::CDOMap)
	{
		delete p.second;
	}
}

int IvyEngine::Go(IApplication* EA)
{


	EntryApp = EA;
	EntryApp->Init(this);

	assert(RootLeaf != nullptr);

	RootLeaf->LeafInit();
	while (true)
	{
		if (const std::optional<WPARAM> ecode = Window::ProcessMessages())
		{
			return (int)*ecode;
		}

		DoFrame();
	}
}

void IvyEngine::DoFrame()
{
	float dt = timer.Mark();

	EntryApp->PreTick(dt, this);

	//RootLeaf->position.x = 5 * ((float)wnd.mouse.GetPosX() / 1280.0f);

	std::vector<IDrawable*> drawables;
	RootLeaf->LeafTick(dt, drawables);

	EntryApp->PreDraw(dt, this);

	pGfx->ClearBuffer(0, 0, 0);
	for (IDrawable* D : drawables)
	{
		D->Draw();
	}

	EntryApp->PostDraw(dt, this);

	pGfx->EndFrame();
}