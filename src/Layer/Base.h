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
	void DoStatusBar();
	void LoadImages();

private:
	bool first_frame = false;
	float m_FrameTime;
	std::vector<std::filesystem::path> files;
	std::vector<std::shared_ptr<graphics::Image>> images;
	int selected = 0;
};
}