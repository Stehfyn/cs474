#pragma once
#include "Layer.h"
namespace cs474 {
	class AssignmentTest2 : public Layer {
	public:
		AssignmentTest2();
		~AssignmentTest2();

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnUIRender() override;
	};
}