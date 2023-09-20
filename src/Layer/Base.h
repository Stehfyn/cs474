#pragma once
#include "Layer.h"
#include "..\Graphics\Image.h"
namespace cs474 {
class Base : public Layer {
public:
	Base();
	~Base();

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnUIRender() override;

private:
	void DoMainMenuBar();
	void LoadImages();

private:
	int selected = 0;
};
}