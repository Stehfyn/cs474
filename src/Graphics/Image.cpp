#include "..\cs474.pch.h"
#include "Image.h"

namespace cs474 {
namespace graphics {
enum Status {
	Invalid = (size_t)-1
};

Image::Image(const std::vector<uint8_t>& data, uint32_t width, uint32_t height, uint32_t channels)
	: m_RendererID(Status::Invalid), m_Width(width), m_Height(height), m_Data(data) {
	m_RendererID = CreateTexture((unsigned char*)data.data(), width, height, channels);
	Unbind();
}

Image::Image(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels) 
	: m_RendererID(Status::Invalid), m_Width(width), m_Height(height), m_Data(width * height * channels) {
	m_RendererID = CreateTexture(data, width, height, channels);
	Unbind();
	std::memcpy(m_Data.data(), data, m_Data.size());
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

size_t Image::GetWidth() const {
	return m_Width;
}

size_t Image::GetHeight() const {
	return m_Height;
}

GLuint Image::GetRendererID() const{
	return m_RendererID;
}

const std::vector<uint8_t>& Image::GetRawData() const {
	return m_Data;
}

// Something to test around and keep in mind:
// https://github.com/ocornut/imgui/issues/3523
GLuint Image::CreateTexture(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels) {
	// No support for variance between internal and represented format
	int format;
	switch (channels) {
	case 1: {
		format = GL_LUMINANCE;
		break;
	}
	case 4: {
		format = GL_RGBA;
		break;
	}
	default:
		throw std::runtime_error("Unsupported number of channels!");
	}

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
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}

void Image::DeleteTexture(GLuint& renderer_id) {
	glDeleteTextures(1, &renderer_id);
}
ImageRegistry::ImageRegistry() {
}
ImageRegistry::~ImageRegistry() {
}
bool ImageRegistry::AddFileList(const std::string& key, const utils::FileList& list) {
	m_Registry.emplace(key, std::make_pair(list, TextureRegistry_{}));
	return true; //TODO: Add logic for success/failure etc.
}
bool ImageRegistry::AddTexture(const std::string& key, const std::string& sub_key, const Texture& tex) {
	auto it = m_Registry.find(key);

	if (it != m_Registry.end()) {
		const auto& file_list = it->second.first; 

		if (file_list.empty())
			return false; // false if file_list is empty (somehow)

		auto& texture_map = it->second.second;

		auto tit = texture_map.find(sub_key);
		if (tit != texture_map.end()) {
			tit->second = tex;
			return true;
		} else {
			auto result = texture_map.emplace(sub_key, tex);
            return result.second; // result.second is true if insertion is successful, false if sub_key already exists
		}
	}

	return false; // Return false if key does not exist in the registry
}
bool ImageRegistry::LoadDefaultTexturesFromFileList(void) {
	for (const auto& kv : m_Registry) {
		const auto& file_list = kv.second.first;
		for (const auto& file : file_list) {
			// default load texture
			if (!file.has_extension()) {
				throw std::runtime_error("Attempt to load an image without an extension!");
			}

			int desired_channels = 4;

			if (std::string(file.extension()) == ".pgm") 
				desired_channels = 1;
			
			int image_width = 0;
			int image_height = 0;

			const auto& data = utils::SlurpFile(file);
			if (data.has_value()) {
				unsigned char* image_data = stbi_load_from_memory((unsigned char*)data.value().data(), data.value().size(), &image_width, &image_height, NULL, desired_channels);

				if (image_data == NULL) {
					emscripten_log(EM_LOG_CONSOLE, "didnt work lol");
				}

				this->AddTexture(kv.first, std::string(file.extension()), graphics::make_texture(image_data, image_width, image_height, desired_channels));

				stbi_image_free(image_data);
			}
		}
	}


	return false;
}
std::vector<std::string> ImageRegistry::GetKeys() const {
	std::vector<std::string> keys;
	for (const auto& kv : m_Registry) {
		keys.push_back(kv.first);
	}
	return keys;
}
std::optional<utils::FileList> ImageRegistry::GetFileList(const std::string& key) const {
	auto it = m_Registry.find(key);
	if (it != m_Registry.end()) {
		return it->second.first;
	}
	return std::nullopt;
}
std::optional<Texture> ImageRegistry::GetTexture(const std::string& key, const std::string& sub_key) const {
	auto it = m_Registry.find(key);
	if (it != m_Registry.end()) {
		const auto& texture_registry = it->second.second;
		auto tex_it = texture_registry.find(sub_key);
		if (tex_it != texture_registry.end()) {
			return tex_it->second;
		}
	}
	return std::nullopt;
}
}
}