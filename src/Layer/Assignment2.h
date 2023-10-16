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

private:
	int filterSize = 7;
	bool inspect_avg = false;
	bool is_hovered_avg1 = false;
	bool is_hovered_avg2 = false;
	bool is_hovered_avg3 = false;

	int filterSizeAvg = 7;
	int filterSizeMed = 7;
	float percentSaltPep = 10.0;
	bool inspect_med = false;
	bool is_hovered_med1 = false;
	bool is_hovered_med2 = false;
	bool is_hovered_med3 = false;
	bool is_hovered_med4 = false;

	bool inspect_hbf = false;
	bool is_hovered_hbf1 = false;
	bool is_hovered_hbf2 = false;
	bool is_hovered_hbf3 = false;
	bool is_hovered_hbf4 = false;

	bool inspect_lap1 = false;
	bool is_hovered_lap11 = false;
	bool is_hovered_lap12 = false;
	bool is_hovered_lap13 = false;
	bool is_hovered_lap14 = false;

	bool inspect_lap2 = false;
	bool is_hovered_lap21 = false;
	bool is_hovered_lap22 = false;
	bool is_hovered_lap23 = false;
	bool is_hovered_lap24 = false;
};
void TryMakeSpatialFilteringTexture(bool force);
void ProcessSpatialFilteringData();
}