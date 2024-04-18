#pragma once
#include "Application.h"

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char* argv[])
{
    Engine::Logging::Init(Engine::GetApplicationName());
	ANE_ELOG_INFO("Logging initialized");

    ANE_PROFILE_BEGIN_SESSION("Startup", appName + "-Profile-Startup.json");
	auto app = Engine::CreateApplication();
    ANE_PROFILE_END_SESSION();

    ANE_PROFILE_BEGIN_SESSION("Runtime", appName + "-Profile-Runtime.json");
	app->Run();
	delete app;
	ANE_PROFILE_END_SESSION();

	return ANENoError;
}
