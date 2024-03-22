#pragma once

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char* argv[])
{    
	auto app = Engine::CreateApplication();
	Engine::Logging::Init(app->GetAppSpecification().LogName);

	NP_ENGINE_LOG_INFO("Engine Initialized");
	NP_LOG_INFO(app->GetAppSpecification().LogName + " Initialized");
	
	app->Run();
	delete app;
    
	return 1;
}