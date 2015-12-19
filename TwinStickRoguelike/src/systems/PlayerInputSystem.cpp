#include <systems/PlayerInputSystem.hpp>
#include <components/PlayerComponent.hpp>
#include <components/AnimationComponent.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <components/DirectionComponent.hpp>

PlayerInputSystem::PlayerInputSystem() : IteratingSystem(ECS::Family::all<PlayerComponent, AnimationComponent, DirectionComponent>().get())
{
}

void PlayerInputSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cDirection = p_entity->get<DirectionComponent>();
  auto cAnimation = p_entity->get<AnimationComponent>();

  auto sprite = cAnimation->sprite;
  auto animator = cAnimation->animator;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
  {
    sprite->move(-60 * p_dt, 0);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkLeft")
    {
      animator->playAnimation("walkLeft", true);
    }

    cDirection->direction = Left;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
  {
    sprite->move(60 * p_dt, 0);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkRight")
    {
      animator->playAnimation("walkRight", true);
    }

    cDirection->direction = Right;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
  {
    sprite->move(0, -60 * p_dt);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkUp")
    {
      animator->playAnimation("walkUp", true);
    }

    cDirection->direction = Up;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
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