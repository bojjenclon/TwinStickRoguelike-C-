#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class MovementSystem : public ECS::IteratingSystem<MovementSystem>
{
public:
  MovementSystem();

  void processEntity(ECS::Entity* p_entity, float p_dt) override;
};

#endif
