#pragma once
#include "Layer.h"
#include "..\Graphics\Image.h"
namespace cs474 {
class Assignment2 : public Layer {
public:
	Assignment2();
	~Assignment2();

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnUIRender() override;

public:
	void Question1();
	void Question2();
	void Question3();
	void Question4();
	void Question5();

private:
	void DoSpatialFiltering();
};
void TryMakeSpatialFilteringTexture(bool force);
void ProcessSpatialFilteringData();
}