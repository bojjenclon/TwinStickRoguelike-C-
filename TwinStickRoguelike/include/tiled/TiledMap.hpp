#ifndef TILED_TILE_MAP_HPP
#define TILED_TILE_MAP_HPP

#include <tiled/TiledTileLayer.hpp>
#include <tiled/TiledTileset.hpp>
#include <Box2D/Box2D.h>
#include <ecstasy/core/Engine.h>
#include <json/json.hpp>
#include <pathfinding/micropather.h>
#include <SFML/Graphics.hpp>
#include <pathfinding/MicropatherNode.hpp>

using json = nlohmann::json;
using namespace micropather;

class TiledMap : public Graph
{
private:
  struct CollisionNode
  {
    int type = 0;
    int clearance = 1;
  };

public:
  TiledMap();

  int getWidth() const;
  int getHeight() const;

  void addTileLayer(TiledTileLayer p_layer);
  TiledTileLayer getTileLayer(int p_index) const;

  void addTileset(TiledTileset p_tileset);
  TiledTileset getTileset(int p_index) const;
  TiledTileset findTilesetFromGid(int p_gid) const;

  int getTileId(int p_layer, int p_x, int p_y) const;

  bool isCollisionAdded() const;
  bool addCollision(std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine, bool p_generateCollisionMap = true);
  bool removeCollision(std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine);

  MicroPatherNode* getPatherNode(unsigned int p_x, unsigned int p_y, int p_width = 1, int p_height = 1);

#ifdef _DEBUG
  // FOR DEBUGGING, SHOULD PROBABLY BE REMOVED LATER
  CollisionNode getCollisionTile(int p_x, int p_y) const;
  void drawCollisionMap(sf::RenderTarget& p_renderTarget) const;
#endif

  static TiledMap loadFromJson(std::string p_path);

  // Micropather Functions
public:
  int Passable(int nx, int ny, int clearance = 1) const;

  virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd) override;
  virtual void AdjacentCost(void* node, MPVector< StateCost > *neighbors) override;
  virtual void PrintStateInfo(void* node) override;

private:
  struct CollisionShape
  {
    b2BodyDef bodyDef;
    std::string type;
    void* data = nullptr;
    b2Body* body = nullptr;

    CollisionShape(b2BodyDef p_bodyDef, std::string p_type, void* p_data = nullptr, b2Body* p_body = nullptr)
      : bodyDef(p_bodyDef), type(p_type), data(p_data), body(p_body)
    {
      
    }
  };

  void addCollisionShape(CollisionShape p_shape);
  void calculateCollisionMap();

private:
  int m_width = 1;
  int m_height = 1;
  int m_tileWidth = 32;
  int m_tileHeight = 32;
  int m_collisionMapWidth = 1;
  int m_collisionMapHeight = 1;
  
  std::vector<CollisionShape> m_collisionShapes;
  std::vector<TiledTileLayer> m_tileLayers;
  std::vector<TiledTileset> m_tilesets;

  std::vector<std::vector<CollisionNode>> m_collisionMap;
  std::vector<std::vector<MicroPatherNode*>> m_pathfindingMap;

  bool m_collisionAdded = false;

#ifdef _DEBUG
  // FOR DEBUGGING, SHOULD PROBABLY BE REMOVED LATER
  sf::VertexArray m_debugCollisionMap;
#endif
};

#endif
