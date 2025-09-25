#include "TextureType.h"

std::string getTextureFile(TextureType type) 
{
    switch (type) 
    {
        case TextureType::Wood:   return "./images/diffuse.bmp";
        case TextureType::Metal: return "./images/specular.bmp";
        case TextureType::Comic:  return "./images/Comic.bmp";
        case TextureType::Abstract:  return "./images/Abstract.bmp";
        default:                   return "./images/diffuse.bmp";
    }
}