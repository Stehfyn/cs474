#pragma once
namespace cs474 {
namespace global {
class Resource {
public:
    Resource() = default;
    Resource(std::any value) : m_Value(std::move(value)) {}

public:
    template<typename T>
    operator T() const {
        try {
            return std::any_cast<T>(m_Value);
        }
        catch (const std::bad_any_cast& e) {
            // Should log it, then forward the throw
            throw;
        }
    }

private:
    std::any m_Value;
};

class ResourceManager {
public:
    static ResourceManager& Instance() {
        static ResourceManager instance;
        return instance;
    }

public:
    template<typename T>
    void AddResource(const std::string& name, T&& resource) {
        utils::atomic_guard lock(m_ResourceMutex);
        auto it = m_ResourceMap.find(name);
        if (it != m_ResourceMap.end()) {
            if (it->second.second != typeid(T)) {
                throw std::runtime_error("Type mismatch, cannot override existing key with new type");
            }
            else {
                it->second.first = Resource(std::any(std::forward<T>(resource)));
            }
        }
        else {
            // Force In-place piece-wise construction... thanks chatgpt
            m_ResourceMap.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(name),
                std::forward_as_tuple(Resource(std::any(std::forward<T>(resource))), std::type_index(typeid(T)))
            );
        }
    }

    std::optional<std::reference_wrapper<const Resource>> GetResource(const std::string& name) {
        utils::atomic_guard lock(m_ResourceMutex);
        auto it = m_ResourceMap.find(name);
        if (it != m_ResourceMap.end()) {
            return std::cref(it->second.first);
        }
        else {
            return std::nullopt;
        }
        
    }

    std::optional<std::reference_wrapper<Resource>> GetResourceMut(const std::string& name) {
        utils::atomic_guard lock(m_ResourceMutex);
        auto it = m_ResourceMap.find(name);
        if (it != m_ResourceMap.end()) {
            return std::ref(it->second.first);
        }
        else {
            return std::nullopt;
        }
    }

private:
    ResourceManager() {}
    ResourceManager(const ResourceManager&) = delete;
    void operator=(const ResourceManager&) = delete;

    std::map<std::string, std::pair<Resource, std::type_index>> m_ResourceMap;
    utils::atomic_mutex m_ResourceMutex;
};

template <class... Args>
static void AddResource(Args&&... args) {
    global::ResourceManager::Instance().AddResource(std::forward<Args>(args)...);
}

template <class... Args>
static void UpdateResource(Args&&... args) {
    AddResource(std::forward<Args>(args)...);
}

static std::optional<std::reference_wrapper<const Resource>> GetResource(const std::string& name) {
    return global::ResourceManager::Instance().GetResource(name);
}

static std::optional<std::reference_wrapper<Resource>> GetResourceMut(const std::string& name) {
    return global::ResourceManager::Instance().GetResourceMut(name);
}

static const Resource& GetResourceUnwrapped(const std::string& name) {
    return GetResource(name).value();
}

static Resource& GetResourceMutUnwrapped(const std::string& name) {
    return GetResourceMut(name).value();
}
}
}