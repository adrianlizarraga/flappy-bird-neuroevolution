#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>

struct TextureKey {
    std::string filename;
    bool smooth;
    bool repeated;

    TextureKey(const std::string& filename, bool smooth = false, bool repeated = false);
};

bool operator<(const TextureKey& k1, const TextureKey& k2);

class AssetManager {
private:
    std::map<TextureKey, std::unique_ptr<sf::Texture> > textures;
    std::map<std::string, std::unique_ptr<sf::Font> > fonts;
public:
    AssetManager();
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    
    bool loadTexture(const TextureKey& key);
    bool loadTexture(const std::string& filename, bool smooth = false, bool repeated = false);
    bool loadFont(const std::string& filename);
    
    bool hasTexture(const TextureKey& key) const;
    bool hasTexture(const std::string& filename, bool smooth = false, bool repeated = false) const;
    bool hasFont(const std::string& filename) const;
    
    sf::Texture& getTexture(const TextureKey& key);
    sf::Texture& getTexture(const std::string& filename, bool smooth = false, bool repeated = false);
    sf::Font& getFont(const std::string& filename);
};