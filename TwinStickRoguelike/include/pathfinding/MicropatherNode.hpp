#ifndef MICRO_PATHER_NODE_HPP
#define MICRO_PATHER_NODE_HPP

struct MicroPatherNode
{
  int x = 0;
  int y = 0;

  int width = 1;
  int height = 1;

  MicroPatherNode(int p_x, int p_y, int p_width = 1, int p_height = 1)
    : x(p_x), y(p_y), width(p_width), height(p_height)
  {
    
  }

  bool operator==(const MicroPatherNode& rhs) const
  {
    return x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.width;
  }
};

#endif
