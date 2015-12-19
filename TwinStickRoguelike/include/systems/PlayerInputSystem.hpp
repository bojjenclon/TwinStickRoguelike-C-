#ifndef PLAYER_INPUT_SYSTEM_HPP
#define PLAYER_INPUT_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class PlayerInputSystem : public ECS::IteratingSystem<PlayerInputSystem>
{
public:
  PlayerInputSystem();

  void processEntity(ECS::Entity* p_entity, float p_dt) override;
};

#endif

