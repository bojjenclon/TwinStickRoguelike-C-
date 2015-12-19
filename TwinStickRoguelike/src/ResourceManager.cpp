#include <ResourceManager.hpp>

#include <iostream>
#include <Thor/Resources/SfmlLoaders.hpp>

ResourceManager::ResourceManager()
{
  
}

bool ResourceManager::loadTexture(std::string p_id, std::string p_path)
{
  try
  {
    m_textureHolder.acquire(p_id, thor::Resources::fromFile<sf::Texture>(p_path));
  }
  catch (thor::ResourceLoadingException& e)
  {
    std::cout << "Error: " << e.what() << std::endl;

    return false;
  }

  return true;
}

sf::Texture& ResourceManager::getTexture(std::string p_id)
{
  return m_textureHolder[p_id];
}


