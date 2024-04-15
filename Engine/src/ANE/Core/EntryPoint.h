#pragma once

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char* argv[])
{
    Engine::Logging::InitEngine();
	ANE_ELOG_INFO("Engine Initialized");

    ANE_PROFILE_BEGIN_SESSION("Startup", "AnemoneProfile-Startup.json");
	auto app = Engine::CreateApplication();
	Engine::Logging::InitApp(app->GetAppSpecification().LogName);
	ANE_LOG_INFO(app->GetAppSpecification().LogName + " Initialized");
    ANE_PROFILE_END_SESSION();

    ANE_PROFILE_BEGIN_SESSION("Runtime", "AnemoneProfile-Runtime.json");
	app->Run();
	delete app;
	ANE_PROFILE_END_SESSION();

	return ANENoError;
}
