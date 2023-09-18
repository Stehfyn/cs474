#pragma once
#include "..\UIElement\UIElement.h"
namespace cs474 {
using UIElementStack = std::vector<std::shared_ptr<cs474::UIElement>>;

class LayerBase {
public:
	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;

	virtual void OnUpdate(float ts) = 0;
	virtual void OnUIRender() = 0;
};

class Layer : public LayerBase {
public:
	virtual ~Layer() = default;

public:
	virtual void OnAttach() override {}
	virtual void OnDetach() override {}

	virtual void OnUpdate(float ts) override {}
	virtual void OnUIRender() override {}

public:
	template <typename T> void PushElement() {
		static_assert(std::is_base_of<cs474::UIElement, T>::value,
			"Pushed type is not subclass of UIElement!");
		m_ElementStack.emplace_back(std::make_shared<T>())->OnAttach();
	}

private:
	UIElementStack m_ElementStack;
};

} // namespace cs474