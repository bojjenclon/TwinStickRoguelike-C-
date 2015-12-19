#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class AnimationSystem : public ECS::IteratingSystem<AnimationSystem>
{
public:
  AnimationSystem();

  void processEntity(ECS::Entity* p_entity, float p_dt) override;
};

#endif

