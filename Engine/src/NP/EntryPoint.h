#pragma once

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char* argv[])
{
    Engine::Logging::InitEngine();
	NP_ENGINE_LOG_INFO("Engine Initialized");

	auto app = Engine::CreateApplication();

	Engine::Logging::InitApp(app->GetAppSpecification().LogName);
	NP_LOG_INFO(app->GetAppSpecification().LogName + " Initialized");

	app->Run();
	delete app;

	return NPNoError;
}
