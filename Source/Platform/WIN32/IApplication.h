#pragma once

#define ENTRYAPP(x) IApplication* EntryApplication() { return new x;}

class IvyEngine;

class IApplication
{
public:
	IApplication();
	virtual ~IApplication() {};

public:
	virtual void Init(IvyEngine* World) = 0;
	virtual void PreTick(float dt, IvyEngine* World) = 0;
	virtual void PreDraw(float dt, IvyEngine* World) = 0;
	virtual void PostDraw(float dt, IvyEngine* World) = 0;
};

IApplication* EntryApplication();