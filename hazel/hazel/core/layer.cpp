#include "hazel/core/layer.h"

#include <string>

namespace Hazel
{
    Layer::Layer(std::string name) : m_debug_name(std::move(name)) {}
}  // namespace Hazel