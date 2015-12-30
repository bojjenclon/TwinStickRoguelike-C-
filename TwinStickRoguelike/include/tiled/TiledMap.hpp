#ifndef TILED_TILE_MAP_HPP
#define TILED_TILE_MAP_HPP

#include <tiled/TiledTileLayer.hpp>
#include <tiled/TiledTileset.hpp>
#include <Box2D/Box2D.h>
#include <ecstasy/core/Engine.h>
#include <json/json.hpp>

using json = nlohmann::json;

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

  void initCollision(std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine);

  static TiledMap loadFromJson(std::string p_path);

private:
  struct CollisionShape
  {
    b2BodyDef bodyDef;
    std::string type;
    void* data;
  };

  void addCollisionShape(CollisionShape p_shape);

private:
  std::vector<CollisionShape> m_collisionShapes;
  std::vector<TiledTileLayer> m_tileLayers;
  std::vector<TiledTileset> m_tilesets;
};

#endif
