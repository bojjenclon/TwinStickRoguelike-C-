#ifndef PLAYER_STATS_SYNC_SYSTEM_HPP
#define PLAYER_STATS_SYNC_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>
#include <UIValues.hpp>

class PlayerStatsSyncSystem : public ECS::IteratingSystem<PlayerStatsSyncSystem>
{
public:
  PlayerStatsSyncSystem(UIValues& p_uiValues);

  void processEntity(ECS::Entity* p_entity, float p_dt) override;

private:
  UIValues& m_uiValues;
};

#endif

