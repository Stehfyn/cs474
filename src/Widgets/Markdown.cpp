#include "..\cs474.pch.h"
#include "Markdown.h"
namespace cs474 {
namespace widgets {
struct MarkdownEmphasis {
	bool italic = false;
	bool bold = false;

	bool operator==(const MarkdownEmphasis& rhs) const {
		return (rhs.italic == italic) && (rhs.bold == bold);
	}

	bool IsDefault() { return  (!italic && !bold); }

};

struct MarkdownTextStyle
{
	MarkdownEmphasis emphasis;
	int headerLevel = 0;

	bool operator==(const MarkdownTextStyle& rhs) const {
		return (rhs.emphasis == emphasis) && (rhs.headerLevel == headerLevel);
	}

	bool IsDefault() { return (headerLevel == 0) && emphasis.IsDefault(); }
};

ImFont* Markdown::get_font() const
{
	ImFont* default_font = ImGui::GetDefaultFont();
	if (m_is_table_header) {
		//return g_font_bold;
		return (ImFont*)global::GetResourceUnwrapped("g_MyriadPro-Semibold_18.0");
	}

	switch (m_hlevel)
	{
	case 0:
		//return m_is_strong ? g_font_bold : g_font_regular;
		return m_is_strong ? (ImFont*)global::GetResourceUnwrapped("g_MyriadPro-Semibold_18.0") :
			(ImFont*)global::GetResourceUnwrapped("g_MyriadPro-Light_18.0");
	case 1:
		//return g_font_bold_large;
		return (ImFont*)global::GetResourceUnwrapped("g_MyriadPro-Semibold_23.0");
	default:
		//return g_font_bold;
		return (ImFont*)global::GetResourceUnwrapped("g_MyriadPro-Semibold_18.0");
	}
}
void Markdown::open_url() const
{
	utils::openUrlInNewTab(m_href.c_str());
}
bool Markdown::get_image(image_info& nfo) const
{
	nfo.texture_id = (ImTextureID)0;
	nfo.size = { 40,20 };
	nfo.uv0 = { 0,0 };
	nfo.uv1 = { 1,1 };
	nfo.col_tint = { 1,1,1,1 };
	nfo.col_border = { 0,0,0,0 };
	return true;
}
void Markdown::html_div(const std::string& dclass, bool e)
{
	if (dclass == "red") {
		if (e) {
			m_table_border = false;
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		}
		else {
			ImGui::PopStyleColor();
			m_table_border = true;
		}
	}
}

void markdown(const std::string& string)
{
	static Markdown s_printer;
	const char* start = string.c_str();
	const char* end = start + string.size();
	s_printer.print(start, end);
}
}
}