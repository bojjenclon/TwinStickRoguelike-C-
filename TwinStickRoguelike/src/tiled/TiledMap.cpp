#include <tiled/TiledMap.hpp>
#include <json/json.hpp>

#include <fstream>
#include <sstream>

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

TiledMap TiledMap::loadFromJson(std::string p_path)
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
    auto layerWidth = parsedJson["layers"][i]["width"].get<int>();
    auto layerHeight = parsedJson["layers"][i]["width"].get<int>();
    auto layerData = parsedJson["layers"][i]["data"].get<std::string>();

    auto layer = TiledTileLayer::createFromData(map, layerData, layerWidth, layerHeight);

    map.addTileLayer(layer);
  }
  
  return map;
}
