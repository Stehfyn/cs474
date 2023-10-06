#pragma once
namespace cs474 {
namespace utils {
	static std::string s_FetchKey;
	static std::vector<uint8_t> s_FetchBuffer;
	// As of now, must ensure ordering of requests.
	// Solution will create a LUT for fetch buffers in global
	static void fetch_bytes(const std::string& key, const char* url) {
		s_FetchKey = key;

		emscripten_fetch_attr_t attr;
		emscripten_fetch_attr_init(&attr);
		strcpy(attr.requestMethod, "GET");
		attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | EMSCRIPTEN_FETCH_PERSIST_FILE;
		
		attr.onsuccess = [](emscripten_fetch_t* fetch) {
				s_FetchBuffer = std::vector<uint8_t>(fetch->totalBytes);
				std::memcpy(s_FetchBuffer.data(), fetch->data, fetch->totalBytes);
				global::AddResource(s_FetchKey, s_FetchBuffer);
				emscripten_log(EM_LOG_CONSOLE, "Downloading %s succeeded, HTTP status code: %d. %zu bytes\n", fetch->url, fetch->status, fetch->totalBytes);
				emscripten_fetch_close(fetch);
		};
		attr.onerror = [](emscripten_fetch_t* fetch) {
				s_FetchBuffer = std::vector<uint8_t>();
				emscripten_log(EM_LOG_CONSOLE, "Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
				emscripten_fetch_close(fetch);
		};

		char _url[256];
		snprintf(_url, sizeof(_url), "%s", url);
		emscripten_fetch(&attr, _url);
	}
} // namespace utils
} // namespace cs474