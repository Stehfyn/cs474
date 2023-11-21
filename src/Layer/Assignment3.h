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

private:
	void Process2DFFT(const graphics::Texture& img, const std::string& currentImage);

	std::vector<uint8_t> GenerateSubject(unsigned int width, 
		unsigned int height, 
		unsigned int sq_width, 
		unsigned int sq_height);

	std::pair<std::vector<uint8_t>, std::vector<uint8_t>> ProcessSubject2(
		const std::vector<uint8_t>& data,
		unsigned int width,
		unsigned int height);

	void GenerateSubject2a();
	void GenerateSubject2b();
	void GenerateSubject2c();
private:
	bool inspect_check = false;
	bool is_hovered_check1 = false;
	bool is_hovered_check2 = false;

	bool inspect_exp3 = false;
	bool is_hovered_exp3_1 = false;
	bool is_hovered_exp3_2 = false;
	bool is_hovered_exp3_3 = false;

	float yStart{};
	float yEnd{};
	float yOffset{};
};
}

