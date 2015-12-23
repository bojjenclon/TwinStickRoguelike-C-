#ifndef BEHAVIOR_TREE_SYSTEM_HPP
#define BEHAVIOR_TREE_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class BehaviorTreeSystem : public ECS::IteratingSystem<BehaviorTreeSystem>
{
public:
  BehaviorTreeSystem();

  void processEntity(ECS::Entity* p_entity, float p_dt) override;
};

#endif

