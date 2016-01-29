#ifndef ITEM_COMPONENT_HPP
#define ACTIVE_COMPONENT_HPP

#include <ecstasy/core/Component.h>

struct ItemComponent : public ECS::Component<ItemComponent>
{
  std::string name = "None";
  std::function<bool(ECS::Entity* p_entity)> callback = nullptr;

  void reset() override
  {
    name = "None";
    callback = nullptr;
  }
};

#endif

