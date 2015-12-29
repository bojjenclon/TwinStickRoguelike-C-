#include <tiled/TiledTileset.hpp>

TiledTileset::TiledTileset()
{
  
}

TiledTileset::TiledTileset(TilesetOptions p_options) :
  m_firstGid(p_options.firstGid), m_imagePath(p_options.imagePath), m_name(p_options.name), 
  m_tileWidth(p_options.tileWidth), m_tileHeight(p_options.tileHeight), m_tileCount(p_options.tileCount),
  m_imageWidth(p_options.imageWidth), m_imageHeight(p_options.imageHeight),
  m_spacing(p_options.spacing), m_margin(p_options.margin)
{
  
}

int TiledTileset::getFirstGid() const
{
  return m_firstGid;
}

std::string TiledTileset::getImagePath() const
{
  return m_imagePath;
}

std::string TiledTileset::getName() const
{
  return m_name;
}

int TiledTileset::getTileWidth() const
{
  return m_tileWidth;
}

int TiledTileset::getTileHeight() const
{
  return m_tileHeight;
}

int TiledTileset::getTileCount() const
{
  return m_tileCount;
}

int TiledTileset::getImageWidth() const
{
  return m_imageWidth;
}

int TiledTileset::getImageHeight() const
{
  return m_imageHeight;
}

int TiledTileset::getSpacing() const
{
  return m_spacing;
}

int TiledTileset::getMargin() const
{
  return m_margin;
}

int TiledTileset::getTileIndex(int p_gid) const
{
  return p_gid - m_firstGid;
}
