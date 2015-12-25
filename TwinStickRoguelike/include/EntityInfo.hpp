#ifndef ENTITY_INFO_HPP
#define ENTITY_INFO_HPP

struct EntityInfo
{
  enum Type
  {
    Obstacle,
    Player,
    Enemy,
    Bullet
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
    EnemyBullet = 1 << 4
  };
};

#endif
