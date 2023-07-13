#pragma once
int main(int argc, char* argv[]) {
  auto app = IRIS::CreateApp();
  app->Run();
  delete app;
  return 0;
}
