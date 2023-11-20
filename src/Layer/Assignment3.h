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
	bool inspect_sub = false;

private:
	std::vector<uint8_t> GenerateSubject(unsigned int width, 
		unsigned int height, 
		unsigned int sq_width, 
		unsigned int sq_height);
	void GenerateSubject2a();
	void GenerateSubject2b();
	void GenerateSubject2c();
};
}

