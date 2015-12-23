#ifndef TARGET_STATS_SYNC_SYSTEM_HPP
#define TARGET_STATS_SYNC_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>
#include <UIValues.hpp>

class TargetStatsSyncSystem : public ECS::IteratingSystem<TargetStatsSyncSystem>
{
public:
  TargetStatsSyncSystem(UIValues& p_uiValues);

  void processEntity(ECS::Entity* p_entity, float p_dt) override;

private:
  UIValues& m_uiValues;
};

#endif

