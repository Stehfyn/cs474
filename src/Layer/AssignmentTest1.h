#pragma once
#include "Layer.h"
namespace cs474 {
	class AssignmentTest1 : public Layer {
	public:
		AssignmentTest1();
		~AssignmentTest1();

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnUIRender() override;
	};
}