#include "..\cs474.pch.h"
#include "Assignment3.h"
namespace cs474 {
Assignment3::Assignment3() {
}
Assignment3::~Assignment3() {
}
void Assignment3::OnAttach() {
	global::UpdateResource("g_Assignment3ScrollToExperiment 1", false);
	global::UpdateResource("g_Assignment3ScrollToExperiment 2", false);
	global::UpdateResource("g_Assignment3ScrollToExperiment 3", false);
}
void Assignment3::OnDetach() {
}
void Assignment3::OnUpdate(float ts) {
}
void Assignment3::OnUIRender() {
	if (global::GetResourceUnwrapped("g_ShowAssignment3")) {
		bool open = true;
		ImGuiWindowFlags window_flags = utils::GetBaseLayerWindowFlags();

		ImGui::Begin("Assignment 3", &open);
		{
			this->Experiment1();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();

			this->Experiment2();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();

			this->Experiment3();
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::NewLine();
		}
		ImGui::End();

		if (!open) global::UpdateResource("g_ShowAssignment3", false);
	}
}
void Assignment3::Experiment1() {
	if (global::GetResourceUnwrapped("g_Assignment3ScrollToExperiment 1")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment3ScrollToExperiment 1", false);
	}
	widgets::markdown("# Experiment 1");
}
void Assignment3::Experiment2() {
	if (global::GetResourceUnwrapped("g_Assignment3ScrollToExperiment 2")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment3ScrollToExperiment 2", false);
	}
	widgets::markdown("# Experiment 2");
}
void Assignment3::Experiment3() {
	if (global::GetResourceUnwrapped("g_Assignment3ScrollToExperiment 3")) {
		ImGui::SetScrollHereY(.0f);
		global::UpdateResource("g_Assignment3ScrollToExperiment 3", false);
	}
	widgets::markdown("# Experiment 3");
}
}