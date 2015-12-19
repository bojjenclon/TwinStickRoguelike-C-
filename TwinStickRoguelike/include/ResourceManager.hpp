#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <Thor/Resources/ResourceHolder.hpp>

class ResourceManager
{
public:
  ResourceManager();

  bool loadTexture(std::string p_id, std::string p_path);
  sf::Texture& getTexture(std::string p_id);

private:
  thor::ResourceHolder<sf::Texture, std::string> m_textureHolder;
};

#endif