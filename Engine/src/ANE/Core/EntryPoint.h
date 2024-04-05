#pragma once

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char* argv[])
{
    Engine::Logging::InitEngine();
	ANE_ENGINE_LOG_INFO("Engine Initialized");

	auto app = Engine::CreateApplication();

	Engine::Logging::InitApp(app->GetAppSpecification().LogName);
	ANE_LOG_INFO(app->GetAppSpecification().LogName + " Initialized");

	app->Run();
	delete app;

	return ANENoError;
}
