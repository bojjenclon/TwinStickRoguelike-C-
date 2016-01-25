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
#include <map/Exit.hpp>

using json = nlohmann::json;
using namespace micropather;

class TiledMap : public Graph
{
private:
  struct CollisionNode
  {
    int type = 0;
    sf::Vector2i horizontalClearance = sf::Vector2i(1, 1);
    sf::Vector2i verticalClearance = sf::Vector2i(1, 1);
  };

public:
  enum ExitDirection
  {
    North,
    South,
    East,
    West
  };

public:
  TiledMap();

  int getWidth() const;
  int getHeight() const;

  void addTileLayer(TiledTileLayer p_layer);
  TiledTileLayer getTileLayer(int p_index) const;
  unsigned int getTileLayerCount() const;

  void addTileset(TiledTileset p_tileset);
  TiledTileset getTileset(int p_index) const;
  TiledTileset findTilesetFromGid(int p_gid) const;

  int getTileId(int p_layer, int p_x, int p_y) const;

  bool isCollisionAdded() const;
  bool addCollision(std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine, bool p_generateCollisionMap = true, bool p_isCollisionEnabled = true);
  bool removeCollision(std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine);
  bool enableCollision() const;
  bool disableCollision() const;

  void addExit(const ExitDirection& p_direction, const Exit& p_exit);
  Exit& getExit(const ExitDirection& p_direction);
  bool hasExit(ExitDirection p_direction) const;

  MicroPather* getPather() const;
  MicroPatherNode* getPatherNode(unsigned int p_x, unsigned int p_y, int p_width = 1, int p_height = 1);

  void addEntity(ECS::Entity* p_entity);
  void enableEntities();
  void disableEntities();

#ifdef _DEBUG
  // FOR DEBUGGING, SHOULD PROBABLY BE REMOVED LATER
  CollisionNode getCollisionTile(int p_x, int p_y) const;
  void drawCollisionMap(sf::RenderTarget& p_renderTarget) const;
#endif

  static TiledMap* loadFromJson(std::string p_path);

  // Micropather Functions
public:
  int Passable(int nx, int ny, int width = 1, int height = 1) const;

  virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd) override;
  virtual void AdjacentCost(void* node, MPVector< StateCost > *neighbors) override;
  virtual void PrintStateInfo(void* node) override;

private:
  struct CollisionShape
  {
    b2BodyDef bodyDef;
    std::string type;
    bool isSensor = false;
    void* data = nullptr;
    b2Body* body = nullptr;

    CollisionShape(b2BodyDef p_bodyDef, std::string p_type, bool p_isSensor = false, void* p_data = nullptr, b2Body* p_body = nullptr)
      : bodyDef(p_bodyDef), type(p_type), isSensor(p_isSensor), data(p_data), body(p_body)
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
  
  std::map<ExitDirection, bool> m_exitDirections;
  std::map<ExitDirection, Exit> m_exits;

  std::vector<CollisionShape> m_collisionShapes;
  std::vector<TiledTileLayer> m_tileLayers;
  std::vector<TiledTileset> m_tilesets;
  
  std::vector<std::vector<CollisionNode>> m_collisionMap;
  std::vector<std::vector<MicroPatherNode*>> m_pathfindingMap;

  MicroPather* m_pather = nullptr;

  bool m_collisionAdded = false;

  std::vector<ECS::Entity*> m_entities;

#ifdef _DEBUG
  // FOR DEBUGGING, SHOULD PROBABLY BE REMOVED LATER
  sf::VertexArray m_debugCollisionMap;
#endif
};

#endif
