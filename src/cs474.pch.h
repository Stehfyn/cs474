#pragma once

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

// Edison Design Group powers Intellisense, does not handle Clang-based well
// https://stackoverflow.com/a/61810989/22468901
#ifdef __INTELLISENSE__ 
#pragma diag_suppress 312 // cs474::utils::SlurpFile()
#endif

#include "Utils/io.hpp"
#include "Utils/js-helpers.h"
#include "Utils/imgui_helpers.hpp"

