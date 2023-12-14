#include "Layer.h"
#include "..\Graphics\Image.h"
namespace cs474 {
class Assignment4 : public Layer {
public:
	Assignment4();
	~Assignment4();

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnUIRender() override;

private:
	void Question1();
	void Question2();
	void Question3();

private:
	int filterSize = 7;

};
}