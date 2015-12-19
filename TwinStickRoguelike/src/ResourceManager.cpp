#include <ResourceManager.hpp>

ResourceManager::ResourceManager(const sf::RenderTarget& p_renderTarget) : m_renderTarget(p_renderTarget)
{
}

bool ResourceManager::loadTexture(const std::string& path)
{
  sf::Texture* texture = new sf::Texture();
  bool success = texture->loadFromFile(path);

  if (success)
  {
    m_textures[path] = texture;
  }

  return success;
}

bool ResourceManager::isTextureLoaded(const std::string& path) const
{
  return m_textures.count(path) > 0;
}

sf::Texture* ResourceManager::getTexture(const std::string& path)
{
  if (!isTextureLoaded(path))
  {
    printf("Error: Texture located at %s is not loaded. Please call loadTexture first.\n", path.c_str());

    return nullptr;
  }

  return m_textures[path];
}

bool ResourceManager::freeTexture(const std::string& path)
{
  if (!isTextureLoaded(path))
  {
    return false;
  }

  m_textures.erase(path);

  return true;
}

void ResourceManager::freeAllTextures()
{
  m_textures.clear();
}