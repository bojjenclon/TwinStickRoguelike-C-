#include "ResourceManager.hpp"

#include <SDL_image.h>

ResourceManager::ResourceManager(SDL_Renderer* p_renderer)
{
  m_renderer = p_renderer;
}

bool ResourceManager::loadTexture(const std::string& path)
{
  //The final texture
  SDL_Texture* newTexture = NULL;

  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL)
  {
    printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());

    return false;
  }
  else
  {
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
    if (newTexture == NULL)
    {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

      return false;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  m_textures[path] = newTexture;

  return true;
}

bool ResourceManager::isTextureLoaded(const std::string& path)
{
  return m_textures.count(path) > 0;
}

SDL_Texture* ResourceManager::getTexture(const std::string& path)
{
  if (!isTextureLoaded(path)) {
    printf("Error: Texture located at %s is not loaded. Please call loadTexture first.\n", path.c_str());

    return nullptr;
  }

  return m_textures[path];
}

bool ResourceManager::freeTexture(const std::string& path)
{
  if (!isTextureLoaded(path)) {
    return false;
  }

  SDL_Texture* tex = m_textures[path];
  m_textures.erase(path);
  SDL_DestroyTexture(tex);
  tex = nullptr;

  return true;
}

void ResourceManager::freeAllTextures()
{
  for (std::map<std::string, SDL_Texture*>::iterator itr = m_textures.begin(); itr != m_textures.end(); itr++)
  {
    SDL_DestroyTexture(itr->second);
  }

  m_textures.clear();
}