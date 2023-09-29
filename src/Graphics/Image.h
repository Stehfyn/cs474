#pragma once
namespace cs474 {
namespace graphics {
class Image {
public:
	Image(const std::vector<uint8_t>& data, uint32_t width, uint32_t height, uint32_t channels);
	Image(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels);
	~Image();

public:
	void Bind(void)   const;
	void Unbind(void) const;
	size_t GetWidth() const;
	size_t GetHeight() const;
	GLuint GetRendererID() const;
	const std::vector<uint8_t>& GetRawData() const;

private:
	GLuint CreateTexture(unsigned char* data, uint32_t width, uint32_t height,
		uint32_t channels);
	void DeleteTexture(GLuint& renderer_id);

private:
	size_t m_Width;
	size_t m_Height;
	GLuint m_RendererID;
	std::vector<uint8_t> m_Data;
};

using Texture = std::shared_ptr<Image>;

template <class... Args>
Texture make_texture(Args&&... args) {
	return std::make_shared<Image>(std::forward<Args>(args)...);
}

// ImageRegistry:
//		   (key)
//		{ "lenna" => 
//				{ {assets/images/raw/lenna.gif, assets/images/pgm/lenna.pgm},
//					 (sub_key)
//				  { "lenna_gif" =>
//						Texture },
//					"lenna_pgm" =>
//						Texture }
//				}
//		}, ...

class ImageRegistry {
	using TextureRegistry_ = std::unordered_map<std::string, Texture>;
	using ImageData = std::pair<utils::FileList, TextureRegistry_>;

public:
	ImageRegistry();
	~ImageRegistry();

public:
	bool AddFileList(const std::string& key, const utils::FileList& list);
	bool AddTexture(const std::string& key, const std::string& sub_key, const Texture& tex);
	bool LoadDefaultTexturesFromFileList(void);

	std::vector<std::string> GetKeys() const;
	std::optional<utils::FileList> GetFileList(const std::string& key) const;
	std::optional<Texture> GetTexture(const std::string& key, const std::string& sub_key) const;

private:
	std::unordered_map<std::string, ImageData> m_Registry;
};

using TextureRegistry = std::vector<Texture>;
}
}

