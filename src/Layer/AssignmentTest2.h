#include "Layer.h"
#include "..\Graphics\Image.h"
namespace cs474 {
	class AssignmentTest2 : public Layer {
	public:
		AssignmentTest2();
		~AssignmentTest2();

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnUIRender() override;

	public:
		void Question1();
		void Question2();
	private:
		int factor = 1;
		bool inspect_sub = false;
		float levels = (uint8_t)-1;
		bool inspect_quant = false;
	};
}