#ifndef TILED_TILE_LAYER_HPP
#define TILED_TILE_LAYER_HPP

#include <vector>
#include <tiled/TiledTile.hpp>

class TiledMap;
typedef std::vector<std::vector<TiledTile>> TileCollection;

class TiledTileLayer
{
public:
  TiledTileLayer();
  TiledTileLayer(TileCollection p_tiles);
  TiledTileLayer(int p_width, int p_height);

  TiledTile getTile(int p_x, int p_y) const;
  int getTileGid(int p_x, int p_y) const;
  int getTileId(int p_x, int p_y) const;

  int getWidth() const;
  int getHeight() const;

  static TiledTileLayer createFromData(const TiledMap& p_map, std::string p_data, int p_width, int p_height);

private:
  TileCollection m_tiles;
  int m_width;
  int m_height;
};

#endif
