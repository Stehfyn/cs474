CXX = external\emsdk\upstream\emscripten\emcc.bat
AR = external\emsdk\upstream\emscripten\emar.bat

CONFIG_DEBUG = Debug
CONFIG_RELEASE = Release

PLATFORM_WIN32 = x64
PLATFORM_EMSCRIPTEN = Emscripten

BIN = ci-bin
BIN-INT = ci-bin-int

EXTERNAL_DIR = external
IMGUI_PRJ = imgui

CS474_DIR = include
CS474_PRJ = cs474

CLIENT_DIR = src
CLIENT_PRJ = cs474-client

# external
IMGUI_DIR = $(EXTERNAL_DIR)/imgui
IMGUI_BACKENDS_DIR = $(IMGUI_DIR)/backends
IMGUI_MD_DIR = $(EXTERNAL_DIR)/imgui_md
MD4C_DIR = $(EXTERNAL_DIR)/md4c/src
STB_DIR = $(EXTERNAL_DIR)/stb
COOLBAR_DIR = $(EXTERNAL_DIR)/ImCoolbar
SPINNER_DIR = $(EXTERNAL_DIR)/imspinner
PLOT_DIR =  $(EXTERNAL_DIR)/implot
TWEEN_DIR = $(EXTERNAL_DIR)/ImTween
FA_DIR = $(EXTERNAL_DIR)/IconFontCppHeaders

IMGUI_OUTPUT = lib$(IMGUI_PRJ).a
IMGUI_EMSCRIPTEN_RELEASE_SUFFIX = $(IMGUI_PRJ)\$(PLATFORM_EMSCRIPTEN)\$(CONFIG_RELEASE)
IMGUI_EMSCRIPTEN_RELEASE_DIR = $(BIN)\$(IMGUI_EMSCRIPTEN_RELEASE_SUFFIX)
IMGUI_EMSCRIPTEN_RELEASE_INT_DIR = $(BIN-INT)\$(IMGUI_EMSCRIPTEN_RELEASE_SUFFIX)
IMGUI_EMSCRIPTEN_RELEASE_OUTPUT = $(IMGUI_EMSCRIPTEN_RELEASE_DIR)\$(IMGUI_OUTPUT)

IMGUI_INCLUDE = $(IMGUI_DIR) $(MD4C_DIR)

IMGUI_SOURCES = \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_BACKENDS_DIR)/imgui_impl_glfw.cpp \
	$(IMGUI_BACKENDS_DIR)/imgui_impl_opengl3.cpp \
	$(IMGUI_MD_DIR)/imgui_md.cpp \
	$(MD4C_DIR)/md4c.c \
	$(COOLBAR_DIR)/ImCoolbar.cpp \
	$(PLOT_DIR)/implot.cpp \
	$(PLOT_DIR)/implot_demo.cpp \
	$(PLOT_DIR)/implot_items.cpp \

IMGUI_OBJECTS = $(IMGUI_SOURCES:.cpp=.o)
IMGUI_OBJECTS := $(IMGUI_OBJECTS:.c=.o)

IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS := $(patsubst $(IMGUI_BACKENDS_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_DIR)/%,$(IMGUI_OBJECTS))
IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS := $(patsubst $(IMGUI_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS))
IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS := $(patsubst $(IMGUI_MD_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS))
IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS := $(patsubst $(MD4C_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS))
IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS := $(patsubst $(COOLBAR_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS))
IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS := $(patsubst $(PLOT_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_DIR)/%,$(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS))

# include
GRAPHICS_DIR = $(CLIENT_DIR)\Graphics
LAYER_DIR = $(CLIENT_DIR)\Layer
UTILS_DIR = $(CLIENT_DIR)\Utils
WIDGETS_DIR = $(CLIENT_DIR)\Widgets

CS474_OUTPUT = lib$(CS474_PRJ).a
CS474_EMSCRIPTEN_RELEASE_SUFFIX = $(CS474_PRJ)\$(PLATFORM_EMSCRIPTEN)\$(CONFIG_RELEASE)
CS474_EMSCRIPTEN_RELEASE_DIR = $(BIN)\$(CS474_EMSCRIPTEN_RELEASE_SUFFIX)
CS474_EMSCRIPTEN_RELEASE_INT_DIR = $(BIN-INT)\$(CS474_EMSCRIPTEN_RELEASE_SUFFIX)
CS474_EMSCRIPTEN_RELEASE_OUTPUT = $(CS474_EMSCRIPTEN_RELEASE_DIR)\$(CS474_OUTPUT)

CS474_SOURCES = $(foreach x, $(CS474_DIR), $(wildcard $(addprefix $(x)/*,.cpp*)))
CS474_OBJECTS = $(CS474_SOURCES:.cpp=.o)
CS474_EMSCRIPTEN_RELEASE_INT_OBJECTS := $(patsubst $(CS474_DIR)/%,$(CS474_EMSCRIPTEN_RELEASE_INT_DIR)/%,$(CS474_OBJECTS))

# src
CLIENT_OUTPUT = $(CLIENT_PRJ).js
CLIENT_EMSCRIPTEN_RELEASE_SUFFIX = $(CLIENT_PRJ)\$(PLATFORM_EMSCRIPTEN)\$(CONFIG_RELEASE)
CLIENT_EMSCRIPTEN_RELEASE_DIR = $(BIN)\$(CLIENT_EMSCRIPTEN_RELEASE_SUFFIX)
CLIENT_EMSCRIPTEN_RELEASE_INT_DIR = $(BIN-INT)\$(CLIENT_EMSCRIPTEN_RELEASE_SUFFIX)
CLIENT_EMSCRIPTEN_RELEASE_OUTPUT = $(CLIENT_EMSCRIPTEN_RELEASE_DIR)\$(CLIENT_OUTPUT)

CLIENT_SOURCES = \
	$(CLIENT_DIR)/cs474.pch.cpp \
	$(CLIENT_DIR)/cs474-client.cpp \
	$(CLIENT_DIR)/Application.cpp \
	$(GRAPHICS_DIR)/Image.cpp \
	$(LAYER_DIR)/Assignment1.cpp \
	$(LAYER_DIR)/Assignment2.cpp \
	$(LAYER_DIR)/Assignment3.cpp \
	$(LAYER_DIR)/Assignment4.cpp \
	$(LAYER_DIR)/AssignmentTest1.cpp \
	$(LAYER_DIR)/AssignmentTest2.cpp \
	$(WIDGETS_DIR)/Markdown.cpp \
	$(UTILS_DIR)/js-helpers.cpp \
	$(LAYER_DIR)/Base.cpp

CLIENT_OBJECTS = $(CLIENT_SOURCES:.cpp=.o)
CLIENT_EMSCRIPTEN_RELEASE_INT_OBJECTS := $(patsubst %,$(CLIENT_EMSCRIPTEN_RELEASE_INT_DIR)/%,$(notdir $(CLIENT_OBJECTS)))

blank :=
define newline

$(blank)
endef
define tab
$(blank)$(blank)
endef

define list_elements
	$(eval $@_PRINT = $(1) $(2) $(3) $(4))
	$(info $(blank))
	$(info $(foreach obj,$($@_PRINT),$(obj)$(newline)))
endef

DEFINES = -DEMSCRIPTEN=1 -D__EMSCRIPTEN__=1
INCLUDE_FLAGS = -I$(IMGUI_DIR) -I$(IMGUI_MD_DIR) -I$(MD4C_DIR) -I$(STB_DIR) -I$(COOLBAR_DIR) -I$(TWEEN_DIR) -I$(FA_DIR) -I$(CS474_DIR) -I$(SPINNER_DIR) -I$(PLOT_DIR)
EXTENSIONS = c cpp pch.cpp

define compile_rule
$1/%.o: $2/%.$3
	$(CXX) $(INCLUDE_FLAGS) -g0 -o $$@ -O2 $(DEFINES) -pthread -c -MD $$<
endef

.PHONY: makedir
makedir:
	-mkdir $(IMGUI_EMSCRIPTEN_RELEASE_DIR)
	-mkdir $(IMGUI_EMSCRIPTEN_RELEASE_INT_DIR)
	-mkdir $(CS474_EMSCRIPTEN_RELEASE_DIR)
	-mkdir $(CS474_EMSCRIPTEN_RELEASE_INT_DIR)
	-mkdir $(CLIENT_EMSCRIPTEN_RELEASE_DIR)
	-mkdir $(CLIENT_EMSCRIPTEN_RELEASE_INT_DIR)

.PHONY: Release
Release: \
makedir \
$(CS474_EMSCRIPTEN_RELEASE_INT_OBJECTS) \
$(CS474_EMSCRIPTEN_RELEASE_OUTPUT) \
$(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS) \
$(IMGUI_EMSCRIPTEN_RELEASE_OUTPUT) \
$(CLIENT_EMSCRIPTEN_RELEASE_INT_OBJECTS ) \
$(CLIENT_EMSCRIPTEN_RELEASE_OUTPUT)

# cs474
$(foreach dir,$(CS474_DIR),$(foreach ext,$(EXTENSIONS),$(eval $(call compile_rule,$(CS474_EMSCRIPTEN_RELEASE_INT_DIR),$(dir),$(ext)))))
$(CS474_EMSCRIPTEN_RELEASE_OUTPUT): $(CS474_EMSCRIPTEN_RELEASE_INT_OBJECTS)
	@$(call list_elements,Linking...,$(CS474_EMSCRIPTEN_RELEASE_INT_OBJECTS),into...,$(CS474_OUTPUT))
	$(AR) rcs $(CS474_EMSCRIPTEN_RELEASE_OUTPUT) $(CS474_EMSCRIPTEN_RELEASE_INT_OBJECTS)

# imgui
$(foreach dir,$(IMGUI_BACKENDS_DIR) $(IMGUI_DIR) $(IMGUI_MD_DIR) $(MD4C_DIR) $(COOLBAR_DIR) $(PLOT_DIR),$(foreach ext,$(EXTENSIONS),$(eval $(call compile_rule,$(IMGUI_EMSCRIPTEN_RELEASE_INT_DIR),$(dir),$(ext)))))
$(IMGUI_EMSCRIPTEN_RELEASE_OUTPUT): $(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS)
	@$(call list_elements,Linking...,$(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS),into...,$(IMGUI_OUTPUT))
	$(AR) rcs $(IMGUI_EMSCRIPTEN_RELEASE_OUTPUT) $(IMGUI_EMSCRIPTEN_RELEASE_INT_OBJECTS)

# client
$(foreach dir,$(CLIENT_DIR) $(LAYER_DIR) $(GRAPHICS_DIR) $(UTILS_DIR) $(WIDGETS_DIR) $(SPINNER_DIR) $(PLOT_DIR),$(foreach ext,$(EXTENSIONS),$(eval $(call compile_rule,$(CLIENT_EMSCRIPTEN_RELEASE_INT_DIR),$(dir),$(ext)))))
$(CLIENT_EMSCRIPTEN_RELEASE_OUTPUT): $(CLIENT_EMSCRIPTEN_RELEASE_INT_OBJECTS)
	@$(call list_elements,Linking...,$(CLIENT_EMSCRIPTEN_RELEASE_INT_OBJECTS),into...,$(CLIENT_EMSCRIPTEN_RELEASE_OUTPUT))
	$(CXX) $(CLIENT_EMSCRIPTEN_RELEASE_INT_OBJECTS) -lcs474 -limgui \
	 -g0 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3 -s WASM=1 -s ALLOW_MEMORY_GROWTH -s FETCH=1 -s WASM_WORKERS=1 -sENVIRONMENT=web,worker -pthread \
	 --preload-file assets/ \
	 -O3 -o $(CLIENT_EMSCRIPTEN_RELEASE_OUTPUT) \
	 -LD:$(CS474_EMSCRIPTEN_RELEASE_DIR) -LD:$(IMGUI_EMSCRIPTEN_RELEASE_DIR)

.PHONY: clean
clean:
	-rmdir /s /q $(BIN)
	-rmdir /s /q $(BIN-INT)