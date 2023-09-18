#include "..\cs474.pch.h"
#include "ResourceManager.h"
namespace cs474 {
namespace global {
Resource::Resource(const std::any& value) 
  : m_Value(value) {
}
}
}