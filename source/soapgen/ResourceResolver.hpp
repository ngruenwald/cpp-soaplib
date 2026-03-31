#pragma once

#include <string>
#include <vector>
#include <memory>
#include <set>

namespace soapgen {

/// Resolves and loads resources (WSDL, XSD) from local or remote locations.
class ResourceResolver {
public:
    ResourceResolver();
    ~ResourceResolver();

    /// Loads a resource from the given URI.
    /// @param[in] uri The URI to load (file path or URL)
    /// @param[in] baseUri The base URI for relative path resolution
    /// @returns The content of the resource as a string
    std::string Load(const std::string& uri, const std::string& baseUri = "");

    /// Resolves a relative URI against a base URI.
    std::string Resolve(const std::string& uri, const std::string& baseUri);

    /// Checks if a URI has already been loaded to prevent cycles.
    bool IsAlreadyLoaded(const std::string& absoluteUri) const;

private:
    std::string LoadFile(const std::string& path);
    std::string LoadUrl(const std::string& url);

private:
    std::set<std::string> loadedUris_;
};

} // namespace soapgen
