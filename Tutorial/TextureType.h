#ifndef TEXTURE_TYPE_H
#define TEXTURE_TYPE_H
#include <string>

// Enumeration for available textures
enum class TextureType
{
    Comic,
    Metal,
    Wood,
    Abstract,
    Default // fallback texture
};

std::string getTextureFile(TextureType type);

#endif // TEXTURE_TYPE_H
