#ifndef BULLET_SYSTEM_HPP
#define BULLET_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class BulletSystem : public ECS::IteratingSystem<BulletSystem>
{
public:
  BulletSystem();

  void processEntity(ECS::Entity* p_entity, float p_dt) override;
};

#endif

