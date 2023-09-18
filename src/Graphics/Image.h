#pragma once
namespace cs474 {
namespace graphics {
class Image {
public:
	Image(std::vector<uint8_t>& data, uint32_t width, uint32_t height, uint32_t channels);
	Image(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels);
	~Image();

public:
	void Bind(void)   const;
	void Unbind(void) const;
	size_t GetWidth() const;
	size_t GetHeight() const;
	GLuint GetRendererID() const;

private:
	GLuint CreateTexture(std::vector<uint8_t>& data, uint32_t width, uint32_t height,
		uint32_t channels);
	GLuint CreateTexture(unsigned char* data, uint32_t width, uint32_t height,
		uint32_t channels);
	void DeleteTexture(GLuint& renderer_id);

private:
	size_t m_Width;
	size_t m_Height;
	GLuint m_RendererID;
};
}
}

