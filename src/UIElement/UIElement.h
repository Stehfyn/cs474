#pragma once
namespace cs474 {
class UIElement {
public:
	virtual ~UIElement() = default;

public:
	virtual void OnAttach() {}
	virtual void OnDetach() {}

	virtual void OnUpdate(float ts) {}
	virtual void OnUIRender() {}
};
}