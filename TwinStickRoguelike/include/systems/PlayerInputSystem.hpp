#ifndef PLAYER_INPUT_SYSTEM_HPP
#define PLAYER_INPUT_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>
#include <Thor/Input/ActionMap.hpp>

class PlayerInputSystem : public ECS::IteratingSystem<PlayerInputSystem>
{
public:
  PlayerInputSystem(sf::Window& p_window);

  void processEntity(ECS::Entity* p_entity, float p_dt) override;

private:
  sf::Window& m_window;
  thor::ActionMap<std::string> m_keyMaps;
};

#endif

