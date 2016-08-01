#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <algorithm>
#include <chrono>
#include <functional>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

#define DBG( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s << std::endl;                   \
   OutputDebugStringA( os_.str().c_str() );  \
}
