#ifndef TILED_TILE_HPP
#define TILED_TILE_HPP

#include <tiled/TiledTileset.hpp>

class TiledTile
{
public:
  TiledTile();
  TiledTile(TiledTileset p_tileset, int p_gid);

  int getGid() const;
  int getId() const;

private:
  TiledTileset m_tileset;
  int m_gid;
  int m_id;
};

#endif
