#include <tiled/TiledMap.hpp>

#include <fstream>
#include <sstream>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <components/PhysicsComponent.hpp>
#include <physics/b2Separator.h>
#include <Thor/Math.hpp>
#include <components/TiledCollisionShapeComponent.hpp>
#include <Constants.hpp>
#include <pathfinding/MicropatherNode.hpp>

TiledMap::TiledMap()
{
}

int TiledMap::getWidth() const
{
  return m_width;
}

int TiledMap::getHeight() const
{
  return m_height;
}

void TiledMap::addTileLayer(TiledTileLayer p_layer)
{
  m_tileLayers.push_back(p_layer);
}

TiledTileLayer TiledMap::getTileLayer(int p_index) const
{
  return m_tileLayers[p_index];
}

void TiledMap::addTileset(TiledTileset p_tileset)
{
  m_tilesets.push_back(p_tileset);
}

TiledTileset TiledMap::getTileset(int p_index) const
{
  return m_tilesets[p_index];
}

TiledTileset TiledMap::findTilesetFromGid(int p_gid) const
{
  auto id = 0;

  for (auto i = m_tilesets.size() - 1; i > 0; --i)
  {
    auto tileset = m_tilesets[i];
    auto firstGid = tileset.getFirstGid();

    if (firstGid <= p_gid)
    {
      return tileset;
    }
  }

  return m_tilesets[0];
}

int TiledMap::getTileId(int p_layer, int p_x, int p_y) const
{
  return m_tileLayers[p_layer].getTileId(p_x, p_y);
}

void TiledMap::addCollisionShape(CollisionShape p_shape)
{
  m_collisionShapes.push_back(p_shape);
}

bool TiledMap::isCollisionAdded() const
{
  return m_collisionAdded;
}

bool TiledMap::addCollision(std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine, bool p_generateCollisionMap)
{
  if (m_collisionAdded)
  {
    return false;
  }

  for (auto it = m_collisionShapes.begin(); it != m_collisionShapes.end(); ++it)
  {
    auto shape = *it;

    if (shape.type == "concave_polygon")
    {
      auto body = p_world->CreateBody(&shape.bodyDef);
      it->body = body;

      auto fixtureDef = new b2FixtureDef();
      fixtureDef->density = 1.0f;

      auto sep = new b2Separator();

      sep->Separate(body, fixtureDef, static_cast<std::vector<b2Vec2>*>(shape.data), static_cast<int>(Constants::PIXELS_PER_METER));

      auto entity = p_engine->createEntity();

      auto cPhysics = p_engine->createComponent<PhysicsComponent>();
      entity->add(cPhysics);
      cPhysics->body = body;

      auto cTiledCollisionShape = p_engine->createComponent<TiledCollisionShapeComponent>();
      entity->add(cTiledCollisionShape);

      p_engine->addEntity(entity);

      delete sep;
    }
    else
    {
      auto body = p_world->CreateBody(&shape.bodyDef);
      it->body = body;

      body->CreateFixture(static_cast<b2Shape*>(shape.data), 1);

      auto entity = p_engine->createEntity();

      auto cPhysics = p_engine->createComponent<PhysicsComponent>();
      entity->add(cPhysics);
      cPhysics->body = body;

      auto cTiledCollisionShape = p_engine->createComponent<TiledCollisionShapeComponent>();
      entity->add(cTiledCollisionShape);

      p_engine->addEntity(entity);
    }
  }

  if (p_generateCollisionMap)
  {
    calculateCollisionMap();
  }

  m_collisionAdded = true;

  return true;
}

bool TiledMap::removeCollision(std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine)
{
  if (!m_collisionAdded)
  {
    return false;
  }

  auto entities = p_engine->getEntitiesFor(ECS::Family::all<TiledCollisionShapeComponent>().get());

  while (!entities->empty())
  {
    auto entity = entities->front();

    auto cPhysics = entity->get<PhysicsComponent>();
    p_world->DestroyBody(cPhysics->body);

    entity->removeAll();
    p_engine->removeEntity(entity);
  }

  m_collisionAdded = false;

  return true;
}

void TiledMap::calculateCollisionMap()
{
  // calculate a collision map whose tiles are 8x8

  auto wDivisions = m_tileWidth / Constants::COLLISION_TILE_WIDTH;
  auto hDivisions = m_tileHeight / Constants::COLLISION_TILE_HEIGHT;

  m_collisionMapWidth = m_width * wDivisions;
  m_collisionMapHeight = m_height * hDivisions;

  m_collisionMap = std::vector<std::vector<CollisionNode>>(m_collisionMapWidth, std::vector<CollisionNode>(m_collisionMapHeight));
  m_pathfindingMap = std::vector<std::vector<MicroPatherNode*>>(m_collisionMapWidth, std::vector<MicroPatherNode*>(m_collisionMapHeight));

#ifdef _DEBUG
  // REMOVE LATER, DEBUGGING ONLY
  m_debugCollisionMap.setPrimitiveType(sf::Quads);
  m_debugCollisionMap.resize(m_collisionMapWidth * m_collisionMapHeight * 4);
#endif

  for (auto x = 0; x < m_collisionMapWidth; ++x)
  {
    for (auto y = 0; y < m_collisionMapHeight; ++y)
    {
      m_pathfindingMap[x][y] = new MicroPatherNode(x, y);

      for (auto it = m_collisionShapes.begin(); it != m_collisionShapes.end(); ++it)
      {
        if (m_collisionMap[x][y].type > 0)
        {
          continue;
        }

        auto collisionShape = *it;

        auto body = collisionShape.body;
        auto transform = body->GetTransform();

        for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
          auto shape = fixture->GetShape();

          auto mx = x * Constants::COLLISION_TILE_WIDTH;
          auto my = y * Constants::COLLISION_TILE_HEIGHT;
          auto spaceOccupied = 0;

          for (auto i = 0; i < Constants::COLLISION_TILE_WIDTH; ++i)
          {
            for (auto j = 0; j < Constants::COLLISION_TILE_HEIGHT; ++j)
            {
              if (shape->TestPoint(transform, b2Vec2((mx + i) / Constants::PIXELS_PER_METER, (my + j) / Constants::PIXELS_PER_METER)))
              {
                spaceOccupied++;
              }
            }
          }

#ifdef _DEBUG
          // FOR DEBUGGING ONLY, REMOVE ALL INSTANCES OF m_debugCollisionMap DURING PROD
          auto quad = &m_debugCollisionMap[(x + y * m_collisionMapWidth) * 4];

          quad[0].position = sf::Vector2f(1.0f * x * Constants::COLLISION_TILE_WIDTH, 1.0f * y * Constants::COLLISION_TILE_HEIGHT);
          quad[1].position = sf::Vector2f((x + 1.0f) * Constants::COLLISION_TILE_WIDTH, 1.0f * y * Constants::COLLISION_TILE_HEIGHT);
          quad[2].position = sf::Vector2f((x + 1.0f) * Constants::COLLISION_TILE_WIDTH, (y + 1.0f) * Constants::COLLISION_TILE_HEIGHT);
          quad[3].position = sf::Vector2f(1.0f * x * Constants::COLLISION_TILE_WIDTH, (y + 1.0f) * Constants::COLLISION_TILE_HEIGHT);

          auto color = (x % 2 || y % 2) ? 255 : 195;
          auto alpha = 85;
          if (spaceOccupied >= Constants::COLLISION_MAJORITY_FILLED)
          {
            m_collisionMap[x][y].type = 1;

            quad[0].color = sf::Color(color, 0, 0, alpha);
            quad[1].color = sf::Color(color, 0, 0, alpha);
            quad[2].color = sf::Color(color, 0, 0, alpha);
            quad[3].color = sf::Color(color, 0, 0, alpha);
          }
          else if (m_collisionMap[x][y].type != 1)
          {
            quad[0].color = sf::Color(0, 0, color, alpha);
            quad[1].color = sf::Color(0, 0, color, alpha);;
            quad[2].color = sf::Color(0, 0, color, alpha);;
            quad[3].color = sf::Color(0, 0, color, alpha);;
          }
#else
          if (spaceOccupied >= Constants::COLLISION_MAJORITY_FILLED)
          {
            m_collisionMap[x][y].type = 1;
          }
#endif
        }
      }
    }
  }

  // calculate node clearance
  // note: clearance assumes entities have origin at the top-left corner (rather than the center)
  for (auto x = 0; x < m_collisionMapWidth; ++x)
  {
    for (auto y = 0; y < m_collisionMapHeight; ++y)
    {

      if (m_collisionMap[x][y].type == 1)
      {
        m_collisionMap[x][y].horizontalClearance = sf::Vector2i(0, 0);
        m_collisionMap[x][y].verticalClearance = sf::Vector2i(0, 0);

        continue;
      }

      // calculate horizontal priority clearance

      auto mainClearance = 1;
      auto minorClearance = 1;

      for (auto xi = x; xi < m_collisionMapWidth; ++xi)
      {
        if (m_collisionMap[xi][y].type > 0)
        {
          break;
        }

        mainClearance++;
      }

      if (mainClearance >= m_collisionMapWidth)
      {
        mainClearance = m_collisionMapWidth;
      }

      for (auto yi = y; yi < m_collisionMapHeight; ++yi)
      {
        if (m_collisionMap[mainClearance - 1][yi].type > 0)
        {
          break;
        }

        minorClearance++;
      }

      if (minorClearance >= m_collisionMapHeight)
      {
        minorClearance = m_collisionMapHeight;
      }

      m_collisionMap[x][y].horizontalClearance.x = mainClearance;
      m_collisionMap[x][y].horizontalClearance.y = minorClearance;

      // calculate vertical priority clearance

      mainClearance = 1;
      minorClearance = 1;

      for (auto yi = y; yi < m_collisionMapHeight; ++yi)
      {
        if (m_collisionMap[x][yi].type > 0)
        {
          break;
        }

        mainClearance++;
      }

      if (mainClearance >= m_collisionMapHeight)
      {
        mainClearance = m_collisionMapHeight;
      }

      for (auto xi = x; xi < m_collisionMapWidth; ++xi)
      {
        if (m_collisionMap[xi][mainClearance - 1].type > 0)
        {
          break;
        }

        minorClearance++;
      }

      if (minorClearance >= m_collisionMapWidth)
      {
        minorClearance = m_collisionMapWidth;
      }

      m_collisionMap[x][y].verticalClearance.x = mainClearance;
      m_collisionMap[x][y].verticalClearance.y = minorClearance;

    }
  }

}

MicroPatherNode* TiledMap::getPatherNode(unsigned int p_x, unsigned int p_y, int p_width, int p_height)
{
  if (p_x < 0 || p_x >= m_pathfindingMap.size() || p_y < 0 || p_y >= m_pathfindingMap[p_x].size())
  {
    return nullptr;
  }

  auto node = m_pathfindingMap[p_x][p_y];

  node->width = p_width;
  node->height = p_height;

  return node;
}

MicroPather* TiledMap::getPather() const
{
  return m_pather;
}

int TiledMap::Passable(int nx, int ny, int width, int height) const
{
  if (nx >= 0 && nx < m_collisionMapWidth
    && ny >= 0 && ny < m_collisionMapHeight)
  {
    auto node = m_collisionMap[nx][ny];

    auto canPass = height >= width ?
      width <= node.verticalClearance.x && height <= node.verticalClearance.y :
      width <= node.horizontalClearance.x && height <= node.horizontalClearance.y;

    if (canPass)
    {
      return m_collisionMap[nx][ny].type;
    }
  }

  return 1;
}

float TiledMap::LeastCostEstimate(void* nodeStart, void* nodeEnd)
{
  if (nodeStart == nullptr || nodeEnd == nullptr)
  {
    return 0;
  }

  auto pathStart = reinterpret_cast<MicroPatherNode*>(nodeStart);
  auto pathEnd = reinterpret_cast<MicroPatherNode*>(nodeEnd);

  auto dx = pathStart->x - pathEnd->x;
  auto dy = pathStart->y - pathEnd->y;
  return static_cast<float>(sqrt(static_cast<double>(dx * dx) + static_cast<double>(dy * dy)));
}

void TiledMap::AdjacentCost(void* node, MPVector<StateCost>* neighbors)
{
  if (node == nullptr)
  {
    return;
  }

  auto pathNode = reinterpret_cast<MicroPatherNode*>(node);

  const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
  const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

  const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

  //auto clearance = pathNode->width > pathNode->height ? pathNode->width : pathNode->height;
  
  for (auto i = 0; i < 8; ++i)
  {
    auto nx = pathNode->x + dx[i];
    auto ny = pathNode->y + dy[i];

    //auto pass = Passable(nx, ny, clearance);
    auto pass = Passable(nx, ny, pathNode->width, pathNode->height);
    if (pass == 0)
    {
      // diagonals
      /*if (i % 2 != 0)
      {
        if (Passable(nx, pathNode->y, clearance) && Passable(pathNode->x, ny, clearance))
        {
          StateCost nodeCost = { getPatherNode(nx, ny, pathNode->width, pathNode->height), cost[i] };
          neighbors->push_back(nodeCost);
        }
      }
      else
      {
        StateCost nodeCost = { getPatherNode(nx, ny, pathNode->width, pathNode->height), cost[i] };
        neighbors->push_back(nodeCost);
      }*/
      StateCost nodeCost = { getPatherNode(nx, ny, pathNode->width, pathNode->height), cost[i] };
      neighbors->push_back(nodeCost);
    }
  }
}

void TiledMap::PrintStateInfo(void* node)
{
  auto pathNode = reinterpret_cast<MicroPatherNode*>(node);
  printf("(%d,%d)", pathNode->x, pathNode->y);
}

#ifdef _DEBUG
TiledMap::CollisionNode TiledMap::getCollisionTile(int p_x, int p_y) const
{
  return m_collisionMap[p_x][p_y];
}

void TiledMap::drawCollisionMap(sf::RenderTarget& p_renderTarget) const
{
  p_renderTarget.draw(m_debugCollisionMap);

  /*static sf::Font font;
  font.loadFromFile("../assets/Adventure Subtitles.ttf");

  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(6);
  for (auto x = 0; x < m_collisionMapWidth; ++x)
  {
    for (auto y = 0; y < m_collisionMapHeight; ++y)
    {
      text.setString(m_collisionMap[x][y].clearance < 10 ? std::to_string(m_collisionMap[x][y].clearance) : "*");
      text.setPosition(x * Constants::COLLISION_TILE_WIDTH, y * Constants::COLLISION_TILE_HEIGHT);
      p_renderTarget.draw(text);
    }
  }*/
}
#endif

TiledMap* TiledMap::loadFromJson(std::string p_path)
{
  auto map = new TiledMap();

  std::ifstream file(p_path);
  std::stringstream strStream;
  strStream << file.rdbuf();
  auto mapJsonStr = strStream.str();
  file.close();

  auto parsedJson = json::parse(mapJsonStr);

  auto mapWidth = parsedJson["width"].get<int>();
  auto mapHeight = parsedJson["height"].get<int>();

  map->m_width = mapWidth;
  map->m_height = mapHeight;

  auto mapTileWidth = parsedJson["tilewidth"].get<int>();
  auto mapTileHeight = parsedJson["tileheight"].get<int>();

  map->m_tileWidth = mapTileWidth;
  map->m_tileHeight = mapTileHeight;

  auto tileDifX = mapTileWidth / Constants::COLLISION_TILE_WIDTH;
  auto tileDifY = mapTileHeight / Constants::COLLISION_TILE_HEIGHT;

  map->m_pather = new MicroPather(map, mapWidth * tileDifX * mapHeight * tileDifY, 8, true);

  // tilesets MUST be loaded before layers
  for (unsigned int i = 0; i < parsedJson["tilesets"].size(); ++i)
  {
    auto firstGid = parsedJson["tilesets"][i]["firstgid"].get<int>();
    auto imagePath = parsedJson["tilesets"][i]["image"].get<std::string>();
    auto name = parsedJson["tilesets"][i]["name"].get<std::string>();
    auto tileWidth = parsedJson["tilesets"][i]["tilewidth"].get<int>();
    auto tileHeight = parsedJson["tilesets"][i]["tileheight"].get<int>();
    auto tileCount = parsedJson["tilesets"][i]["tilecount"].get<int>();
    auto imageWidth = parsedJson["tilesets"][i]["imagewidth"].get<int>();
    auto imageHeight = parsedJson["tilesets"][i]["imageheight"].get<int>();
    auto spacing = parsedJson["tilesets"][i]["spacing"].get<int>();
    auto margin = parsedJson["tilesets"][i]["margin"].get<int>();

    TiledTileset tileset(TiledTileset::TilesetOptions{ firstGid, imagePath, name, tileWidth, tileHeight, tileCount, imageWidth, imageHeight, spacing, margin });

    map->addTileset(tileset);
  }

  for (unsigned int i = 0; i < parsedJson["layers"].size(); ++i)
  {
    auto curLayer = parsedJson["layers"][i];

    if (!curLayer["visible"].get<bool>())
    {
      continue;
    }

    auto layerType = curLayer["type"].get<std::string>();
    auto layerName = curLayer["name"].get<std::string>();

    if (layerType == "tilelayer")
    {
      auto layerWidth = curLayer["width"].get<int>();
      auto layerHeight = curLayer["width"].get<int>();
      auto layerData = curLayer["data"].get<std::string>();

      auto layer = TiledTileLayer::createFromData(map, layerData, layerWidth, layerHeight);

      if (!curLayer["properties"]["depth"].is_null())
      {
        auto layerDepth = atoi(curLayer["properties"]["depth"].get<std::string>().c_str());
        layer.setDepth(layerDepth);
      }

      map->addTileLayer(layer);
    }
    else if (layerType == "objectgroup")
    {
      if (layerName == "collision_shapes")
      {
        for (unsigned int j = 0; j < curLayer["objects"].size(); ++j)
        {
          auto curObject = curLayer["objects"][j];

          if (!curObject["visible"].get<bool>())
          {
            continue;
          }

          auto objectType = curObject["type"].get<std::string>();

          if (objectType == "rectangle")
          {
            auto width = curObject["width"].get<int>() / 2;
            auto height = curObject["height"].get<int>() / 2;

            auto box = new b2PolygonShape();
            box->SetAsBox(width / Constants::PIXELS_PER_METER, height / Constants::PIXELS_PER_METER);

            auto x = curObject["x"].get<int>() + width;
            auto y = curObject["y"].get<int>() + height;

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Constants::PIXELS_PER_METER, y / Constants::PIXELS_PER_METER);

            map->addCollisionShape(CollisionShape{ bodyDef, objectType, box });
          }
          else if (objectType == "circle")
          {
            auto width = curObject["width"].get<int>() / 2;
            auto height = curObject["height"].get<int>() / 2;
            auto radius = width > height ? width : height;

            auto circle = new b2CircleShape();
            circle->m_radius = radius / Constants::PIXELS_PER_METER;

            auto x = curObject["x"].get<int>() + width;
            auto y = curObject["y"].get<int>() + height;

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Constants::PIXELS_PER_METER, y / Constants::PIXELS_PER_METER);

            map->addCollisionShape(CollisionShape{ bodyDef, objectType, circle });
          }
          // currently broken
          else if (objectType == "ellipse")
          {
            auto width = curObject["width"].get<int>() / 2 / Constants::PIXELS_PER_METER;
            auto height = curObject["height"].get<int>() / 2 / Constants::PIXELS_PER_METER;

            auto segments = 32;
            auto segmentLength = 2 * thor::Pi / segments;

            auto points = new b2Vec2[segments];
            for (auto k = 0; k < segments; k++)
            {
              auto px = width * cos(segmentLength * k);
              auto py = height * sin(segmentLength * k);

              points[k] = b2Vec2(px, py);
            }

            auto polygon = new b2PolygonShape();
            polygon->Set(points, segments);

            delete[] points;

            auto x = curObject["x"].get<int>();
            auto y = curObject["y"].get<int>();

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Constants::PIXELS_PER_METER, y / Constants::PIXELS_PER_METER);

            map->addCollisionShape(CollisionShape{ bodyDef, objectType, polygon });
          }
          else if (objectType == "convex_polygon")
          {
            auto numPoints = curObject["polygon"].size();
            auto points = new b2Vec2[numPoints];

            for (unsigned int k = 0; k < numPoints; ++k)
            {
              auto curPoint = curObject["polygon"][k];

              points[k] = b2Vec2(curPoint["x"].get<int>() / Constants::PIXELS_PER_METER, curPoint["y"].get<int>() / Constants::PIXELS_PER_METER);
            }

            auto polygon = new b2PolygonShape();
            polygon->Set(points, numPoints);

            delete[] points;

            auto x = curObject["x"].get<int>();
            auto y = curObject["y"].get<int>();

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Constants::PIXELS_PER_METER, y / Constants::PIXELS_PER_METER);

            map->addCollisionShape(CollisionShape{ bodyDef, objectType, polygon });
          }
          else if (objectType == "concave_polygon")
          {
            auto numPoints = curObject["polygon"].size();
            auto points = new std::vector<b2Vec2>();

            for (unsigned int k = 0; k < numPoints; ++k)
            {
              auto curPoint = curObject["polygon"][k];

              points->push_back(b2Vec2(curPoint["x"].get<int>() / Constants::PIXELS_PER_METER, curPoint["y"].get<int>() / Constants::PIXELS_PER_METER));
            }

            auto x = curObject["x"].get<int>();
            auto y = curObject["y"].get<int>();

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Constants::PIXELS_PER_METER, y / Constants::PIXELS_PER_METER);

            map->addCollisionShape(CollisionShape{ bodyDef, objectType, points });
          }
        }
      }
    }
  }

  return map;
}
