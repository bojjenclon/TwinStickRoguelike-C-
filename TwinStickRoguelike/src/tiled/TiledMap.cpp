#include <tiled/TiledMap.hpp>
#include <json/json.hpp>

#include <fstream>
#include <sstream>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Game.hpp>
#include <components/PhysicsComponent.hpp>
#include <physics/b2Separator.h>
#include <Thor/Math.hpp>

using namespace nlohmann;

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

TiledMap TiledMap::loadFromJson(std::string p_path, std::unique_ptr<b2World>& p_world, std::unique_ptr<ECS::Engine>& p_engine)
{
  TiledMap map;

  std::ifstream file(p_path);
  std::stringstream strStream;
  strStream << file.rdbuf();
  auto mapJsonStr = strStream.str();
  file.close();

  auto parsedJson = nlohmann::json::parse(mapJsonStr);

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
            box->SetAsBox(width / Game::PIXELS_PER_METER, height / Game::PIXELS_PER_METER);

            auto x = curObject["x"].get<int>() + width;
            auto y = curObject["y"].get<int>() + height;

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Game::PIXELS_PER_METER, y / Game::PIXELS_PER_METER);

            auto body = p_world->CreateBody(&bodyDef);

            body->CreateFixture(box, 1);

            auto entity = p_engine->createEntity();

            auto cPhysics = p_engine->createComponent<PhysicsComponent>();
            entity->add(cPhysics);
            cPhysics->body = body;

            p_engine->addEntity(entity);

            delete box;
          }
          else if (objectType == "circle")
          {
            auto width = curObject["width"].get<int>() / 2;
            auto height = curObject["height"].get<int>() / 2;
            auto radius = width > height ? width : height;

            auto circle = new b2CircleShape();
            circle->m_radius = radius / Game::PIXELS_PER_METER;

            auto x = curObject["x"].get<int>() + width;
            auto y = curObject["y"].get<int>() + height;

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Game::PIXELS_PER_METER, y / Game::PIXELS_PER_METER);

            auto body = p_world->CreateBody(&bodyDef);

            body->CreateFixture(circle, 1);

            auto entity = p_engine->createEntity();

            auto cPhysics = p_engine->createComponent<PhysicsComponent>();
            entity->add(cPhysics);
            cPhysics->body = body;

            p_engine->addEntity(entity);

            delete circle;
          }
          // currently broken
          /*else if (objectType == "ellipse")
          {
            auto width = curObject["width"].get<int>() / 2;
            auto height = curObject["height"].get<int>() / 2;

            auto numPoints = 32;
            auto step = 360.0f / numPoints;

            std::vector<b2Vec2> points;

            for (auto k = 0.0f; k < 360.0f; k += step)
            {
              auto px = width * cos(k * thor::Pi / 180.0f);
              auto py = height * sin(k * thor::Pi / 180.0f);

              points.push_back(b2Vec2(px / Game::PIXELS_PER_METER, py / Game::PIXELS_PER_METER));
            }

            auto polygon = new b2PolygonShape();
            polygon->Set(&points[0], points.size());

            auto x = curObject["x"].get<int>();
            auto y = curObject["y"].get<int>();

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Game::PIXELS_PER_METER, y / Game::PIXELS_PER_METER);

            auto body = p_world->CreateBody(&bodyDef);

            body->CreateFixture(polygon, 1);

            auto entity = p_engine->createEntity();

            auto cPhysics = p_engine->createComponent<PhysicsComponent>();
            entity->add(cPhysics);
            cPhysics->body = body;

            p_engine->addEntity(entity);
          }*/
          else if (objectType == "convex_polygon")
          {
            auto numPoints = curObject["polygon"].size();
            auto points = new b2Vec2[numPoints];

            for (unsigned int k = 0; k < numPoints; ++k)
            {
              auto curPoint = curObject["polygon"][k];

              points[k] = b2Vec2(curPoint["x"].get<int>() / Game::PIXELS_PER_METER, curPoint["y"].get<int>() / Game::PIXELS_PER_METER);
            }

            auto polygon = new b2PolygonShape();
            polygon->Set(points, numPoints);

            delete[] points;

            auto x = curObject["x"].get<int>();
            auto y = curObject["y"].get<int>();

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Game::PIXELS_PER_METER, y / Game::PIXELS_PER_METER);

            auto body = p_world->CreateBody(&bodyDef);

            body->CreateFixture(polygon, 1);

            auto entity = p_engine->createEntity();

            auto cPhysics = p_engine->createComponent<PhysicsComponent>();
            entity->add(cPhysics);
            cPhysics->body = body;

            p_engine->addEntity(entity);
          }
          else if (objectType == "concave_polygon")
          {
            auto numPoints = curObject["polygon"].size();
            auto points = new std::vector<b2Vec2>();

            for (unsigned int k = 0; k < numPoints; ++k)
            {
              auto curPoint = curObject["polygon"][k];

              points->push_back(b2Vec2(curPoint["x"].get<int>() / Game::PIXELS_PER_METER, curPoint["y"].get<int>() / Game::PIXELS_PER_METER));
            }

            auto x = curObject["x"].get<int>();
            auto y = curObject["y"].get<int>();

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.fixedRotation = true;
            bodyDef.position.Set(x / Game::PIXELS_PER_METER, y / Game::PIXELS_PER_METER);

            auto body = p_world->CreateBody(&bodyDef);

            auto fixtureDef = new b2FixtureDef();
            fixtureDef->density = 1.0f;

            auto sep = new b2Separator();

            sep->Separate(body, fixtureDef, points, Game::PIXELS_PER_METER);

            auto entity = p_engine->createEntity();

            auto cPhysics = p_engine->createComponent<PhysicsComponent>();
            entity->add(cPhysics);
            cPhysics->body = body;

            p_engine->addEntity(entity);

            delete sep;
          }
        }
      }
    }
  }
  
  return map;
}
