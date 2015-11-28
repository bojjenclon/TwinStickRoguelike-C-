#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>

#include <SDL.h>

class ResourceManager
{
public:
  ResourceManager(SDL_Renderer* p_renderer);

  bool loadTexture(const std::string& path);
  bool isTextureLoaded(const std::string& path);
  SDL_Texture* getTexture(const std::string& path);
  bool freeTexture(const std::string& path);

  void freeAllTextures();

private:
  SDL_Renderer* m_renderer;

  std::map<std::string, SDL_Texture*> m_textures;
};

#endif