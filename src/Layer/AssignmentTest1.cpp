#include "..\cs474.pch.h"
#include "AssignmentTest1.h"
namespace cs474 {
AssignmentTest1::AssignmentTest1() {
}
AssignmentTest1::~AssignmentTest1() {
}

void AssignmentTest1::OnAttach() {
}

void AssignmentTest1::OnDetach() {
}

void AssignmentTest1::OnUpdate(float ts) {
}

void AssignmentTest1::OnUIRender() {
	ImGui::BringWindowToDisplayFront(ImGui::FindWindowByName("AssignmentTest1"));
	ImGui::Begin("AssignmentTest1");
	if (ImGui::Button("click")) {

	}
	ImGui::End();
}
}