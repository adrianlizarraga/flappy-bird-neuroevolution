#include "AssetManager.h"
#include <iostream>

TextureKey::TextureKey(const std::string &filename, bool smooth, bool repeated) : filename(filename), smooth(smooth), repeated(repeated) {}

bool operator<(const TextureKey &k1, const TextureKey &k2) {
    int nameComparison = k1.filename.compare(k2.filename);

    if (nameComparison != 0) {
        return nameComparison < 0;
    } else if (k1.smooth != k2.smooth) {
        return k1.smooth < k2.smooth;
    } else {
        return k1.repeated < k2.repeated;
    }
}

AssetManager::AssetManager() {}

bool AssetManager::hasTexture(const TextureKey &key) const {
    auto it = this->textures.find(key);

    return it != this->textures.end();
}

bool AssetManager::hasTexture(const std::string &filename, bool smooth, bool repeated) const {
    TextureKey key(filename, smooth, repeated);

    return this->hasTexture(key);
}

bool AssetManager::hasFont(const std::string &filename) const {
    auto it = this->fonts.find(filename);

    return it != this->fonts.end();
}

bool AssetManager::loadTexture(const TextureKey &key) {
    if (this->hasTexture(key)) {
        return true;
    }

    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
    bool loaded = texture->loadFromFile(key.filename);

    if (loaded) {
        texture->setRepeated(key.repeated);
        texture->setSmooth(key.smooth);

        this->textures.emplace(std::make_pair(key, std::move(texture)));
    }

    return loaded;
}

bool AssetManager::loadTexture(const std::string &filename, bool smooth, bool repeated) {
    TextureKey key(filename, smooth, repeated);

    return this->loadTexture(key);
}

bool AssetManager::loadFont(const std::string &filename) {
    if (this->hasFont(filename)) {
        return true;
    }

    std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();
    bool loaded = font->loadFromFile(filename);

    if (loaded) {
        this->fonts.emplace(std::make_pair(filename, std::move(font)));
    }

    return loaded;
}

sf::Texture &AssetManager::getTexture(const TextureKey &key) {
    this->loadTexture(key);
    return *this->textures.at(key);
}

sf::Texture &AssetManager::getTexture(const std::string &filename, bool smooth, bool repeated) {
    TextureKey key(filename, smooth, repeated);

    return this->getTexture(key);
}

sf::Font &AssetManager::getFont(const std::string &filename) {
    this->loadFont(filename);
    return *this->fonts.at(filename);
}