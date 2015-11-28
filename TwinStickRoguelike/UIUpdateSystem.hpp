#ifndef UI_UPDATE_SYSTEM_HPP
#define UI_UPDATE_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class UIUpdateSystem : public ECS::IteratingSystem<UIUpdateSystem>
{
public:
  UIUpdateSystem();

  void processEntity(ECS::Entity* entity, float deltaTime) override;
};

#endif

