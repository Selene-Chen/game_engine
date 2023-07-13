#pragma once
namespace IRIS {
  class Application {
   public:
    Application();
    ~Application();
    void Run();
  };
  Application* CreateApp();
}  // namespace IRIS
