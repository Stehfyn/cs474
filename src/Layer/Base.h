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
	void DoTableOfContents();
	void DoLandingPage();
	void DoWidgetPanel();

private:
	void LoadImages();
	void BuildDockspace();

private:
	int selected = 0;
	std::string img_key;
	std::string readme;
};
}