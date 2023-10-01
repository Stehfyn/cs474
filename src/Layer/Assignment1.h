#pragma once
#include "Layer.h"
#include "..\Graphics\Image.h"
namespace cs474 {
class Assignment1 : public Layer {
public:
	Assignment1();
	~Assignment1();

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

private:
	int factor1 = 1;
	int factor2 = 1;
	bool inspect_sub1 = false;
	bool inspect_sub2 = false;

	float levels1 = UINT8_MAX;
	float levels2 = UINT8_MAX;
	bool inspect_quant1 = false;
	bool inspect_quant2 = false;

	bool inspect_eq1 = false;
	bool inspect_eq2 = false;

	std::string spec1 = "boat";
	std::string spec2 = "f16";
	bool inspect_spec1 = false;
	bool inspect_spec2 = false;
};
}