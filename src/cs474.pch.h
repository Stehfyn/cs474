#pragma once

#include <atomic>
#include <cstdio>
#include <string>
#include <memory>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <map>
#include <any>
#include <mutex>
#include <utility>
#include <typeindex>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_md.h"

// #include "ImTween.h" // need to fix for -std=c++2a
#include "ImCoolbar.h"
#include "IconsFontAwesome6.h"
#include "stb_image.h"

// Edison Design Group powers Intellisense, does not handle Clang-based well
// https://stackoverflow.com/a/61810989/22468901
#ifdef __INTELLISENSE__ 
#pragma diag_suppress 312 // cs474::utils::SlurpFile()
#pragma diag_suppress 415 // cs474::utils::SlurpFile()
#endif

#include "Utils/atomic_mutex.hpp"
#include "Global/ResourceManager.hpp"

#include "Utils/io.hpp"
#include "Utils/js-helpers.h"

#include "Utils/imgui_helpers.hpp"

#include "Widgets/StatusBar.h"

#include "cs474.h"

namespace cs474 {
	using FileRegistry = std::vector<std::filesystem::path>;
}
