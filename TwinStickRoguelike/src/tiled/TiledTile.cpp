#include <tiled/TiledTile.hpp>
#include <tiled/TiledTileset.hpp>

TiledTile::TiledTile()
{
}

TiledTile::TiledTile(TiledTileset p_tileset, int p_gid) : m_tileset(p_tileset), m_gid(p_gid)
{
  m_id = m_gid - m_tileset.getFirstGid();
}

int TiledTile::getGid() const
{
  return m_gid;
}

int TiledTile::getId() const
{
  return m_id;
}
