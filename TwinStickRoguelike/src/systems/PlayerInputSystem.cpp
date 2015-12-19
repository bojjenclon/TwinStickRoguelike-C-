#include <systems/PlayerInputSystem.hpp>
#include <components/PlayerComponent.hpp>
#include <components/AnimationComponent.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <components/DirectionComponent.hpp>

PlayerInputSystem::PlayerInputSystem(sf::Window& p_window) : IteratingSystem(ECS::Family::all<PlayerComponent, AnimationComponent, DirectionComponent>().get()), m_window(p_window)
{
  m_keyMaps["moveLeft"] = thor::Action(sf::Keyboard::Left, thor::Action::Hold) || thor::Action(sf::Keyboard::A, thor::Action::Hold);
  m_keyMaps["moveRight"] = thor::Action(sf::Keyboard::Right, thor::Action::Hold) || thor::Action(sf::Keyboard::D, thor::Action::Hold);
  m_keyMaps["moveUp"] = thor::Action(sf::Keyboard::Up, thor::Action::Hold) || thor::Action(sf::Keyboard::W, thor::Action::Hold);
  m_keyMaps["moveDown"] = thor::Action(sf::Keyboard::Down, thor::Action::Hold) || thor::Action(sf::Keyboard::S, thor::Action::Hold);
}

void PlayerInputSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  m_keyMaps.update(m_window);

  auto cDirection = p_entity->get<DirectionComponent>();
  auto cAnimation = p_entity->get<AnimationComponent>();

  auto sprite = cAnimation->sprite;
  auto animator = cAnimation->animator;

  if (m_keyMaps.isActive("moveLeft"))
  {
    sprite->move(-60 * p_dt, 0);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkLeft")
    {
      animator->playAnimation("walkLeft", true);
    }

    cDirection->direction = Left;
  }
  else if (m_keyMaps.isActive("moveRight"))
  {
    sprite->move(60 * p_dt, 0);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkRight")
    {
      animator->playAnimation("walkRight", true);
    }

    cDirection->direction = Right;
  }
  else if (m_keyMaps.isActive("moveUp"))
  {
    sprite->move(0, -60 * p_dt);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkUp")
    {
      animator->playAnimation("walkUp", true);
    }

    cDirection->direction = Up;
  }
  else if (m_keyMaps.isActive("moveDown"))
  {
    sprite->move(0, 60 * p_dt);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkDown")
    {
      animator->playAnimation("walkDown", true);
    }

    cDirection->direction = Down;
  }
  else
  {
    if (cDirection->direction == Left && animator->isPlayingAnimation())
    {
      animator->playAnimation("idleLeft");
    }
    else if (cDirection->direction == Right && animator->isPlayingAnimation())
    {
      animator->playAnimation("idleRight");
    }
    else if (cDirection->direction == Up && animator->isPlayingAnimation())
    {
      animator->playAnimation("idleUp");
    }
    else if (cDirection->direction == Down && animator->isPlayingAnimation())
    {
      animator->playAnimation("idleDown");
    }
  }
}