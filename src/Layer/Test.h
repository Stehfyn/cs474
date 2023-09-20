#pragma once
#include "Layer.h"
namespace cs474 {
	class Test : public Layer {
	public:
		Test();
		~Test();

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnUIRender() override;

	private:
		void DoMarkdownTest();
	};
}

