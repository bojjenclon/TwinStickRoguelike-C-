#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Constants
{
  static const int SCREEN_WIDTH = 1024;
  static const int SCREEN_HEIGHT = 768;
  static const int SCREEN_FPS = 60;
  static const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

  static const float PIXELS_PER_METER = 100.0f;

  static const auto COLLISION_TILE_WIDTH = 8;
  static const auto COLLISION_TILE_HEIGHT = 8;
  static const auto COLLISION_TILE_SIZE = COLLISION_TILE_WIDTH * COLLISION_TILE_HEIGHT;
  static const auto COLLISION_MAJORITY_FILLED = COLLISION_TILE_SIZE / 2.0f;
}

#endif
