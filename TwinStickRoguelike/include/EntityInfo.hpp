#ifndef ENTITY_INFO_HPP
#define ENTITY_INFO_HPP

struct EntityInfo
{
  enum Type
  {
    Obstacle,
    Exit,
    Player,
    Enemy,
    Bullet,
    Item
  };
};

struct Collision
{
  enum Type
  {
    Obstacle = 1 << 0,
    Player = 1 << 1,
    Enemy = 1 << 2,
    PlayerBullet = 1 << 3,
    EnemyBullet = 1 << 4,
    Item = 1 << 5
  };
};

#endif
