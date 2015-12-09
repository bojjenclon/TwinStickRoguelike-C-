#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
  ResourceManager(const sf::RenderTarget& p_renderTarget);

  bool loadTexture(const std::string& path);
  bool isTextureLoaded(const std::string& path) const;
  sf::Texture* getTexture(const std::string& path);
  bool freeTexture(const std::string& path);

  void freeAllTextures();

private:
  const sf::RenderTarget& m_renderTarget;

  std::map<std::string, sf::Texture*> m_textures;
};

#endif