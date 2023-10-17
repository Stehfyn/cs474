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

	void DoPrewittLenna();
	void DoPrewittSf();
	void DoSobelLenna();
	void DoSobelSf();
	void DoLaplacianLenna();
	void DoLaplacianSf();

private:
	int filterSize = 7;
	bool inspect_avg = false;
	bool inspect_avg2 = false;
	bool is_hovered_avg1 = false;
	bool is_hovered_avg2 = false;
	bool is_hovered_avg3 = false;
	bool is_hovered_avg4 = false;
	bool is_hovered_avg5 = false;
	bool is_hovered_avg6 = false;

	int filterSizeAvg = 7;
	int filterSizeAvg2 = 7;
	int filterSizeMed = 7;
	int filterSizeMed2 = 7;
	float percentSaltPep = 10.0;
	float percentSaltPep2 = 10.0;
	bool inspect_med = false;
	bool inspect_med2 = false;
	bool is_hovered_med1 = false;
	bool is_hovered_med2 = false;
	bool is_hovered_med3 = false;
	bool is_hovered_med4 = false;
	bool is_hovered_med5 = false;
	bool is_hovered_med6 = false;
	bool is_hovered_med7 = false;
	bool is_hovered_med8 = false;

	bool inspect_hbf = false;
	bool inspect_hbf2 = false;
	bool is_hovered_hbf1 = false;
	bool is_hovered_hbf2 = false;
	bool is_hovered_hbf3 = false;
	bool is_hovered_hbf4 = false;
	bool is_hovered_hbf5 = false;
	bool is_hovered_hbf6 = false;
	bool is_hovered_hbf7 = false;
	bool is_hovered_hbf8 = false;

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

	bool inspect_lap3 = false;
	bool is_hovered_lap31 = false;
	bool is_hovered_lap32 = false;
	bool is_hovered_lap33 = false;
	bool is_hovered_lap34 = false;

	bool inspect_lap4 = false;
	bool is_hovered_lap41 = false;
	bool is_hovered_lap42 = false;
	bool is_hovered_lap43 = false;
	bool is_hovered_lap44 = false;

	bool inspect_lap5 = false;
	bool is_hovered_lap51 = false;
	bool is_hovered_lap52 = false;
	bool is_hovered_lap53 = false;
	bool is_hovered_lap54 = false;

	bool inspect_lap6 = false;
	bool is_hovered_lap61 = false;
	bool is_hovered_lap62 = false;
	bool is_hovered_lap63 = false;
	bool is_hovered_lap64 = false;

	float thresh_min_prewitt1 = .65f;
	float thresh_max_prewitt1 = .90f;

	float thresh_min_prewitt2 = .60f;
	float thresh_max_prewitt2 = .65f;

	float thresh_min_sobel1 = .65f;
	float thresh_max_sobel1 = .90f;

	float thresh_min_sobel2 = .60f;
	float thresh_max_sobel2 = .65f;

	int filterSizeAvgLap1 = 7;
	int filterSizeAvgLap2 = 7;

	float thresh_min_lap1 = .60f;
	float thresh_max_lap1 = .90f;

	float thresh_min_lap2 = .60f;
	float thresh_max_lap2 = .95f;
};
void TryMakeSpatialFilteringTexture(bool force);
void ProcessSpatialFilteringData();
}