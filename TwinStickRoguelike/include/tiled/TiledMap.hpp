#ifndef TILED_TILE_MAP_HPP
#define TILED_TILE_MAP_HPP

#include <tiled/TiledTileLayer.hpp>
#include <tiled/TiledTileset.hpp>
#include <Box2D/Box2D.h>
#include <ecstasy/core/Engine.h>

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

  static TiledMap loadFromJson(std::string p_path, std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine);

private:
  std::vector<TiledTileLayer> m_tileLayers;
  std::vector<TiledTileset> m_tilesets;
};

#endif
