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
		void Question3();
		void Question4();
		void QuestionTest();

	private:
		int factor1 = 1;
		int factor2 = 1;
		bool inspect_sub1 = false;
		bool inspect_sub2 = false;
		bool inspect_sub3 = false;
		bool inspect_sub4 = false;
		bool inspect_sub5 = false;
		int factor = 1;
		bool inspect_sub = false;
		float levels = (uint8_t)-1;
		bool inspect_quant = false;
		bool inspect_eq = false;
		const char* spechistoChoice = "boat";
		const char* imageChoice = "boat";
		int filterSize = 7;
		int filterSizeAvg = 7;
		bool inspect_spec = false;
		float percentSaltPep = 30.0;
		//std::vector<uint8_t> specImageData;

		int ghettoIndex = 0;
	};
}