#ifndef NODE_WATCH_SYSTEM_HPP
#define NODE_WATCH_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class NodeWatchSystem : public ECS::IteratingSystem<NodeWatchSystem>
{
public:
  NodeWatchSystem();

  void processEntity(ECS::Entity* p_entity, float p_dt) override;
};

#endif

