#ifndef NODE_WATCH_COMPONENT_HPP
#define NODE_WATCH_COMPONENT_HPP

#include <ecstasy/core/Component.h>

struct NodeWatchComponent : public ECS::Component<NodeWatchComponent>
{
  int previousX = 0;
  int previousY = 0;
  bool nodeChanged = true;

  void reset() override
  {
    previousX = previousY = -1;
    nodeChanged = true;
  }
};

#endif

