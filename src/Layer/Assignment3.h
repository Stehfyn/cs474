#pragma once
#include "Layer.h"
#include "..\Graphics\Image.h"
namespace cs474 {
class Assignment3: public Layer {
public:
	Assignment3();
	~Assignment3();

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnUIRender() override;

private:
	void Experiment1();
	void Experiment2();
	void Experiment3();
};
}

