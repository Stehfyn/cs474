#pragma once
namespace cs474 {
namespace widgets {
struct FontOptions {

};

struct Markdown : public imgui_md {
	ImFont* get_font() const override;
	void open_url() const override;
	bool get_image(image_info& nfo) const override;
	void html_div(const std::string& dclass, bool e) override;
};

void markdown(const std::string& string);
}
}