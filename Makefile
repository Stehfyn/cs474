CXX = external\emsdk\upstream\emscripten\emcc.bat# Compiler
AR = external\emsdk\upstream\emscripten\emar.bat

BIN = ci-bin
BIN-INT = ci-bin-int

CS474_DIR = include # Source files directory
CS474_SOURCES = $(foreach x, $(CS474_DIR), $(wildcard $(addprefix $(x)/*,.cpp*)))
CS474_OBJECTS = $(CS474_SOURCES:.cpp=.o)
CS474_OUTPUT = libcs474.a

EXTERNAL_DIR = external
IMGUI_DIR = $(EXTERNAL_DIR)/imgui
IMGUI_BACKENDS_DIR = $(EXTERNAL_DIR)/imgui/backends
IMGUI_MD_DIR = $(EXTERNAL_DIR)/imgui_md
MD4C_DIR = $(EXTERNAL_DIR)/md4c/src

IMGUI_OUTPUT = $(BIN)/imgui/Release/libimgui.a
IMGUI_INCLUDE = $(IMGUI_DIR) $(MD4C_DIR)

IMGUI_SOURCES = \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
	$(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp \
	$(IMGUI_MD_DIR)/imgui_md.cpp \
	$(MD4C_DIR)/md4c.c

IMGUI_OBJECTS = $(IMGUI_SOURCES:.cpp=.o)
IMGUI_OBJECTS := $(IMGUI_OBJECTS:.c=.o)

IMGUI_INT_OBJECTS := $(patsubst $(IMGUI_BACKENDS_DIR)/%,$(BIN-INT)/imgui/Release/%,$(IMGUI_OBJECTS))
IMGUI_INT_OBJECTS := $(patsubst $(IMGUI_DIR)/%,$(BIN-INT)/imgui/Release/%,$(IMGUI_INT_OBJECTS))
IMGUI_INT_OBJECTS := $(patsubst $(IMGUI_MD_DIR)/%,$(BIN-INT)/imgui/Release/%,$(IMGUI_INT_OBJECTS))
IMGUI_INT_OBJECTS := $(patsubst $(MD4C_DIR)/%,$(BIN-INT)/imgui/Release/%,$(IMGUI_INT_OBJECTS))

CLIENT_OUTPUT = $(BIN)/cs474-client/Release/cs474-client.js
CLIENT_DIR = src
CLIENT_SOURCES = \
	$(CLIENT_DIR)/cs474.pch.cpp \
	$(CLIENT_DIR)/cs474-client.cpp \
	$(CLIENT_DIR)/Application.cpp \
	$(CLIENT_DIR)/Graphics/Image.cpp \
	$(CLIENT_DIR)/Layer/Test.cpp \
	$(CLIENT_DIR)/Widgets/Markdown.cpp \
	$(CLIENT_DIR)/Utils/js-helpers.cpp \
	$(CLIENT_DIR)/Layer/Base.cpp

CLIENT_SOURCES_NON_PCH = $(filter-out $(CLIENT_DIR)/cs474.pch.cpp, $(CLIENT_SOURCES))
CLIENT_OBJECTS := $(patsubst $(CLIENT_DIR)/%.cpp,$(BIN-INT)/cs474-client/Release/%.o,$(CLIENT_SOURCES_NON_PCH))
CLIENT_OBJECTS += $(BIN-INT)/cs474-client/Release/cs474.pch.o
CLIENT_INT_OBJECTS := $(addprefix $(BIN-INT)/cs474-client/Release/, $(notdir $(CLIENT_OBJECTS)))

DEFINES = -DEMSCRIPTEN=1 -D__EMSCRIPTEN__=1
INCLUDE_FLAGS = -I$(IMGUI_DIR) -I$(IMGUI_MD_DIR) -I$(MD4C_DIR)

.PHONY: makedir
makedir:
	-@mkdir $(BIN)\cs474\Release 2>/dev/null
	-@mkdir $(BIN-INT)\cs474\Release 2>/dev/null
	-@mkdir $(BIN)\imgui\Release 2>/dev/null
	-@mkdir $(BIN-INT)\imgui\Release 2>/dev/null
	-@mkdir $(BIN)\cs474-client\Release 2>/dev/null
	-@mkdir $(BIN-INT)\cs474-client\Release 2>/dev/null

.PHONY: Release
Release: makedir $(CS474_OBJECTS) $(IMGUI_INT_OBJECTS) $(IMGUI_OUTPUT) $(CLIENT_OBJECTS) $(CLIENT_INT_OBJECTS) $(CLIENT_OUTPUT)
$(CS474_OBJECTS): $(CS474_SOURCES)
	@$(CXX) -g0 -o $(BIN-INT)/cs474/Release/$(patsubst include/%,%,$@) -O2 -DEMSCRIPTEN=1 -D__EMSCRIPTEN__=1 -c -MD $<
	@$(AR) rcs $(BIN)/cs474/Release/$(CS474_OUTPUT) $(BIN-INT)/cs474/Release/$(patsubst include/%,%,$@)

$(BIN-INT)/imgui/Release/%.o: $(IMGUI_BACKENDS_DIR)/%.cpp
	$(CXX) $(INCLUDE_FLAGS) -g0 -o $(BIN-INT)/imgui/Release/$(subst backends/,,$(@F)) -O2 $(DEFINES) -c -MD $<

$(BIN-INT)/imgui/Release/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(INCLUDE_FLAGS) -g0 -o $(BIN-INT)/imgui/Release/$(subst backends/,,$(@F)) -O2 $(DEFINES) -c -MD $<

$(BIN-INT)/imgui/Release/%.o: $(IMGUI_MD_DIR)/%.cpp
	$(CXX) $(INCLUDE_FLAGS) -g0 -o $@ -O2 $(DEFINES) -c -MD $<

$(BIN-INT)/imgui/Release/%.o: $(MD4C_DIR)/%.c
	$(CXX) $(INCLUDE_FLAGS) -g0 -o $@ -O2 $(DEFINES) -c -MD $<

$(IMGUI_OUTPUT): $(IMGUI_INT_OBJECTS)
	$(AR) rcs $(IMGUI_OUTPUT) $(IMGUI_INT_OBJECTS)

$(BIN-INT)/cs474-client/Release/%.o: $(CLIENT_DIR)/%.cpp
	$(CXX) $(INCLUDE_FLAGS) -g0 -o $(BIN-INT)/cs474-client/Release/$(notdir $@) -O2 $(DEFINES) -c -MD $<

$(BIN-INT)/cs474-client/Release/cs474.pch.o: $(CLIENT_DIR)/cs474.pch.cpp
	$(CXX) $(INCLUDE_FLAGS) -g0 -o $@ -O2 $(DEFINES) -c -MD $<

$(CLIENT_OUTPUT): $(CLIENT_INT_OBJECTS)
	$(CXX) $(CLIENT_INT_OBJECTS) -lcs474 -limgui -g0 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3 -s WASM=1 -s ALLOW_MEMORY_GROWTH --preload-file assets/ -O3 -o $(CLIENT_OUTPUT) -LD:$(BIN)/cs474/Release -LD:$(BIN)/imgui/Release

.PHONY: clean
clean:
	@.\scripts\CI\rmdir.bat $(BIN)
	@.\scripts\CI\rmdir.bat $(BIN-INT)