#pragma once
#include "Application.h"

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char* argv[])
{
    Engine::Logging::InitEngine();
	ANE_ELOG_INFO("Engine Initialized");
    const std::string appName = Engine::GetApplicationName();
	Engine::Logging::InitApp(appName);
	ANE_LOG_INFO(appName + " Initialized");

    ANE_PROFILE_BEGIN_SESSION("Startup", appName + "-Profile-Startup.json");
	auto app = Engine::CreateApplication();
    ANE_PROFILE_END_SESSION();

    ANE_PROFILE_BEGIN_SESSION("Runtime", appName + "-Profile-Runtime.json");
	app->Run();
	delete app;
	ANE_PROFILE_END_SESSION();

	return ANENoError;
}
