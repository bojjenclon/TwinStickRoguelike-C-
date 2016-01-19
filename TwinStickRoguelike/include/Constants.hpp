#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Constants
{
  static const int SCREEN_WIDTH = 1280;
  static const int SCREEN_HEIGHT = 960;

  static const int MAP_OFFSET = 96;

  static const int VIEW_WIDTH = 640;
  static const int VIEW_HEIGHT = 480;
  static const int MAP_HEIGHT = VIEW_HEIGHT - MAP_OFFSET;

  static const int FPS = 60;
  static const int TICKS_PER_FRAME = 1000 / FPS;

  static const float PIXELS_PER_METER = 100.0f;

  static const int MAP_TILE_WIDTH = 16;
  static const int MAP_TILE_HEIGHT = 16;

  static const auto COLLISION_TILE_WIDTH = 8;
  static const auto COLLISION_TILE_HEIGHT = 8;
  static const auto COLLISION_TILE_SIZE = COLLISION_TILE_WIDTH * COLLISION_TILE_HEIGHT;
  static const auto COLLISION_MAJORITY_FILLED = COLLISION_TILE_SIZE / 2.0f;
}

#endif
