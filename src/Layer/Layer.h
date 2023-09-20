#pragma once
namespace cs474 {
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

};

} // namespace cs474