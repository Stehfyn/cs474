#include "..\cs474.pch.h"
#include "Test.h"
#include "..\UIElement\Markdown.h"
namespace cs474 {
Test::Test() {
}
Test::~Test() {
}
void Test::OnAttach() {
}
void Test::OnDetach() {
}
void Test::OnUpdate(float ts) {
}
void Test::OnUIRender() {
	DoMarkdownTest();
}
void Test::DoMarkdownTest() {
	ImGui::Begin("Markdown test window");
	ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow()); //bring to front without focusing
	markdown(R"(
       # Title
       Some content...
    )");

	markdown(R"(Name &nbsp; &nbsp; &nbsp; &nbsp; | Multiline &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<br>header  | [Link&nbsp;](#link1)
:------|:-------------------|:--
Value-One | Long <br>explanation <br>with \<br\>\'s|1
~~Value-Two~~ | __text auto wrapped__\: long explanation here |25 37 43 56 78 90
**etc** | [~~Some **link**~~](https://github.com/mekhontsev/imgui_md)|3)");
	ImGui::End();
}
}
// https://github.com/pthom/imgui_bundle/tree/main/external/imgui_md/imgui_md_wrapper