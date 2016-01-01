#include <tiled/TiledMap.hpp>

#include <fstream>
#include <sstream>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <components/PhysicsComponent.hpp>
#include <physics/b2Separator.h>
#include <Thor/Math.hpp>
#include <components/TiledCollisionShapeComponent.hpp>
#include <Constants.hpp>

TiledMap::TiledMap()
{
  
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

bool TiledMap::addCollision(std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine)
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

      auto fixtureDef = new b2FixtureDef();
      fixtureDef->density = 1.0f;

      auto sep = new b2Separator();

      sep->Separate(body, fixtureDef, static_cast<std::vector<b2Vec2>*>(shape.data), Constants::PIXELS_PER_METER);

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

TiledMap TiledMap::loadFromJson(std::string p_path)
{
  TiledMap map;

  std::ifstream file(p_path);
  std::stringstream strStream;
  strStream << file.rdbuf();
  auto mapJsonStr = strStream.str();
  file.close();

  auto parsedJson = json::parse(mapJsonStr);
  
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

    TiledTileset tileset(TiledTileset::TilesetOptions { firstGid, imagePath, name, tileWidth, tileHeight, tileCount, imageWidth, imageHeight, spacing, margin });

    map.addTileset(tileset);
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

      map.addTileLayer(layer);
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

            map.addCollisionShape(CollisionShape{ bodyDef, objectType, box });
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

            map.addCollisionShape(CollisionShape{ bodyDef, objectType, circle });
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

            map.addCollisionShape(CollisionShape{ bodyDef, objectType, polygon });
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

            map.addCollisionShape(CollisionShape{ bodyDef, objectType, polygon });
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

            map.addCollisionShape(CollisionShape{ bodyDef, objectType, points });
          }
        }
      }
    }
  }
  
  return map;
}
