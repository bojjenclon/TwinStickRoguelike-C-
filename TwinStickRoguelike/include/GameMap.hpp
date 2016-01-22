#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include <tiled/TiledMap.hpp>
#include <tiled/TiledTileLayerDrawable.hpp>

struct GameMap
{
  TiledMap* tiledMap = nullptr;
  std::vector<TiledTileLayerDrawable*> mapLayerDrawables;
  std::vector<ECS::Entity*> mapLayerEntities;

  GameMap(TiledMap* p_tiledMap);
};

#endif
