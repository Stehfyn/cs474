#include "..\cs474.pch.h"
#include "Image.h"

namespace cs474 {
namespace graphics {
enum Status {
	Invalid = (size_t)-1
};

Image::Image(const std::vector<uint8_t>& data, uint32_t width, uint32_t height, uint32_t channels)
	: m_RendererID(Status::Invalid), m_Width(width), m_Height(height) {
	m_RendererID = CreateTexture((unsigned char*)data.data(), width, height, channels);
	Unbind();
}

Image::Image(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels) 
	: m_RendererID(Status::Invalid), m_Width(width), m_Height(height) {
	m_RendererID = CreateTexture(data, width, height, channels);
	Unbind();
}

Image::~Image() {
	DeleteTexture(m_RendererID);
}

void Image::Bind(void) const {
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Image::Unbind(void) const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

size_t Image::GetWidth() const
{
	return m_Width;
}

size_t Image::GetHeight() const
{
	return m_Height;
}

GLuint Image::GetRendererID() const{
	return m_RendererID;
}

// Something to test around and keep in mind:
// https://github.com/ocornut/imgui/issues/3523
GLuint Image::CreateTexture(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels) {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	//https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}

void Image::DeleteTexture(GLuint& renderer_id) {
	glDeleteTextures(1, &renderer_id);
}
}
}