#include "engine/Core.h"

class Sanbox : public IRIS::Application {
 public:
  Sanbox() {}
  ~Sanbox() {}
};

IRIS::Application* IRIS::CreateApp() { return new Sanbox(); }
