#ifndef TILED_TILE_MAP_HPP
#define TILED_TILE_MAP_HPP

#include <tiled/TiledTileLayer.hpp>
#include <tiled/TiledTileset.hpp>

class TiledMap
{
public:
  TiledMap();

  void addTileLayer(TiledTileLayer p_layer);
  TiledTileLayer getTileLayer(int p_index) const;

  void addTileset(TiledTileset p_tileset);
  TiledTileset getTileset(int p_index) const;
  TiledTileset findTilesetFromGid(int p_gid) const;

  int getTileId(int p_layer, int p_x, int p_y) const;

  static TiledMap loadFromJson(std::string p_path);

private:
  std::vector<TiledTileLayer> m_tileLayers;
  std::vector<TiledTileset> m_tilesets;
};

#endif
