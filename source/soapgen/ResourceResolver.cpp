#include "ResourceResolver.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <httplib.h>
#include <iostream>

namespace fs = std::filesystem;

namespace soapgen {

ResourceResolver::ResourceResolver() {}
ResourceResolver::~ResourceResolver() {}

static bool isUrl(const std::string& uri) {
    return uri.find("http://") == 0 || uri.find("https://") == 0;
}

std::string ResourceResolver::Resolve(const std::string& uri, const std::string& baseUri) {
    if (uri.empty()) return "";
    if (isUrl(uri)) return uri;
    if (fs::path(uri).is_absolute()) return uri;
    
    if (baseUri.empty()) {
        return fs::absolute(uri).string();
    }

    if (isUrl(baseUri)) {
        // Simple URL joining: find last slash
        auto lastSlash = baseUri.find_last_of('/');
        if (lastSlash == std::string::npos) return uri;
        return baseUri.substr(0, lastSlash + 1) + uri;
    } else {
        auto baseDir = fs::path(baseUri).parent_path();
        return (baseDir / uri).lexically_normal().string();
    }
}

std::string ResourceResolver::Load(const std::string& uri, const std::string& baseUri) {
    std::string absoluteUri = Resolve(uri, baseUri);
    
    if (loadedUris_.count(absoluteUri)) {
        return ""; // Already loaded
    }
    
    loadedUris_.insert(absoluteUri);
    
    if (isUrl(absoluteUri)) {
        return LoadUrl(absoluteUri);
    } else {
        return LoadFile(absoluteUri);
    }
}

bool ResourceResolver::IsAlreadyLoaded(const std::string& absoluteUri) const {
    return loadedUris_.count(absoluteUri) > 0;
}

std::string ResourceResolver::LoadFile(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

std::string ResourceResolver::LoadUrl(const std::string& url) {
    // Basic URL parsing for httplib
    std::string host;
    std::string path;
    bool isHttps = url.find("https://") == 0;
    size_t start = isHttps ? 8 : 7;
    size_t slash = url.find('/', start);
    
    if (slash == std::string::npos) {
        host = url.substr(start);
        path = "/";
    } else {
        host = url.substr(start, slash - start);
        path = url.substr(slash);
    }

    // Note: This is a simplified downloader.
    // For production we might need to handle redirects, etc.
    if (isHttps) {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        httplib::SSLClient cli(host);
        auto res = cli.Get(path.c_str());
        if (res && res->status == 200) return res->body;
#else
        throw std::runtime_error("HTTPS support not enabled in httplib");
#endif
    } else {
        httplib::Client cli(host);
        auto res = cli.Get(path.c_str());
        if (res && res->status == 200) return res->body;
    }

    throw std::runtime_error("Could not download URL: " + url);
}

} // namespace soapgen
