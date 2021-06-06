#pragma once

#include "hippo/engine.h"
#include "hippo/app.h"

// To be implemented in client app
// The client returns a pointer to an instance of a class derived from hippo::App
// The ownership of the returned pointer belongs to Hippo, and will be managed as such
//
// Example:
// class ClientApp : public hippo::App {};
// hippo::App* CreateApp() { return new ClientApp(); }
hippo::App* CreateApp();

int main()
{
	hippo::App* app = CreateApp();
	hippo::Engine::Instance().Run(app);
	delete app;
	return 0;
}