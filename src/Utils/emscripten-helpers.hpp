#pragma once
namespace cs474 {
namespace utils {
	static std::mutex s_FetchMutex;
	static std::vector<uint8_t> s_FetchBuffer;
	static std::condition_variable s_cv;
	static bool ready = false;
	static void fetch_bytes(const char* url, std::vector<uint8_t>& data) {
		emscripten_fetch_attr_t attr;
		emscripten_fetch_attr_init(&attr);
		strcpy(attr.requestMethod, "GET");
		attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | EMSCRIPTEN_FETCH_PERSIST_FILE;

		attr.onsuccess = [](emscripten_fetch_t* fetch) {
				s_FetchBuffer = std::vector<uint8_t>(fetch->totalBytes);
				std::memcpy(s_FetchBuffer.data(), fetch->data, fetch->totalBytes);
				emscripten_log(EM_LOG_CONSOLE, "Downloading %s succeeded, HTTP status code: %d. %zu bytes\n", fetch->url, fetch->status, fetch->totalBytes);
				emscripten_fetch_close(fetch);
				ready = true;
		};
		attr.onerror = [](emscripten_fetch_t* fetch) {
				s_FetchBuffer = std::vector<uint8_t>();
				emscripten_log(EM_LOG_CONSOLE, "Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
				emscripten_fetch_close(fetch);
				ready = true;
		};

		char _url[256];
		snprintf(_url, sizeof(_url), "%s", url);
		emscripten_fetch(&attr, _url);
		emscripten_log(EM_LOG_CONSOLE, "Fetch has concluded\n");
		while (!ready);
		ready = false;
		data.resize(s_FetchBuffer.size());
		std::memcpy(data.data(), s_FetchBuffer.data(), s_FetchBuffer.size());
		emscripten_log(EM_LOG_CONSOLE, "Fetch buffer copied\n");
	}

	static void fetch_as_string(const char* url, std::string& data) {
		std::vector<uint8_t> fetch_data;
		fetch_bytes(url, fetch_data);
		emscripten_log(EM_LOG_CONSOLE, "Fetch Data size %zu\n", fetch_data.size());
		data = std::string(reinterpret_cast<const char*>(fetch_data.data()), fetch_data.size());
		emscripten_log(EM_LOG_CONSOLE, "Data size %zu\n", data.size());
	}
} // namespace utils
} // namespace cs474