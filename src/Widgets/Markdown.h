#pragma once
namespace cs474 {
namespace widgets {

struct FontOptions {

};

struct MarkdownImage
{
    ImTextureID	texture_id;
    ImVec2	size;
    ImVec2	uv0;
    ImVec2	uv1;
    ImVec4	col_tint;
    ImVec4	col_border;
};

using VoidFunction = std::function<void(void)>;
using StringFunction = std::function<void(std::string)>;
using HtmlDivFunction = std::function<void(const std::string& divClass, bool openingDiv)>;
using MarkdownImageFunction = std::function<std::optional<MarkdownImage>(const std::string&)>;

struct Markdown : public imgui_md {
	ImFont* get_font() const override;
	void open_url() const override;
	bool get_image(image_info& nfo) const override;
	void html_div(const std::string& dclass, bool e) override;
};

void markdown(const std::string& string);
}
}