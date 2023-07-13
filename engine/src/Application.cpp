#include "engine/Application.h"

#include <iostream>
namespace IRIS {
  Application::Application() {}
  Application::~Application() {}
  void Application::Run() { std::cout << "hello iris engine!" << std::endl; }
}  // namespace IRIS
