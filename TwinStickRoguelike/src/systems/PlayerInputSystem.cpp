#include <systems/PlayerInputSystem.hpp>
#include <components/PlayerComponent.hpp>
#include <components/AnimationComponent.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <components/DirectionComponent.hpp>
#include <components/PhysicsComponent.hpp>

PlayerInputSystem::PlayerInputSystem(sf::Window& p_window) : IteratingSystem(ECS::Family::all<PlayerComponent, AnimationComponent, DirectionComponent, PhysicsComponent>().get()), m_window(p_window)
{
  m_keyMaps["moveLeft"] = thor::Action(sf::Keyboard::Left, thor::Action::Hold) || thor::Action(sf::Keyboard::A, thor::Action::Hold);
  m_keyMaps["moveRight"] = thor::Action(sf::Keyboard::Right, thor::Action::Hold) || thor::Action(sf::Keyboard::D, thor::Action::Hold);
  m_keyMaps["moveUp"] = thor::Action(sf::Keyboard::Up, thor::Action::Hold) || thor::Action(sf::Keyboard::W, thor::Action::Hold);
  m_keyMaps["moveDown"] = thor::Action(sf::Keyboard::Down, thor::Action::Hold) || thor::Action(sf::Keyboard::S, thor::Action::Hold);
}

void PlayerInputSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  static const float MOVE_SPEED = 0.01f;

  m_keyMaps.update(m_window);

  auto cDirection = p_entity->get<DirectionComponent>();
  auto cAnimation = p_entity->get<AnimationComponent>();
  auto cPhysics = p_entity->get<PhysicsComponent>();

  auto animator = cAnimation->animator;

  auto isMoving = false;

  if (m_keyMaps.isActive("moveLeft"))
  {
    cPhysics->body->ApplyLinearImpulse(b2Vec2(-MOVE_SPEED, 0), cPhysics->body->GetWorldCenter(), true);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkLeft")
    {
      animator->playAnimation("walkLeft", true);
    }

    cDirection->direction = Left;

    isMoving = true;
  }
  else if (m_keyMaps.isActive("moveRight"))
  {
    cPhysics->body->ApplyLinearImpulse(b2Vec2(MOVE_SPEED, 0), cPhysics->body->GetWorldCenter(), true);

    if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkRight")
    {
      animator->playAnimation("walkRight", true);
    }

    cDirection->direction = Right;

    isMoving = true;
  }
  else
  {
    cPhysics->body->SetLinearVelocity(b2Vec2(cPhysics->body->GetLinearVelocity().x * 0.95f, cPhysics->body->GetLinearVelocity().y));
  }
  
  if (m_keyMaps.isActive("moveUp"))
  {
    cPhysics->body->ApplyLinearImpulse(b2Vec2(0, -MOVE_SPEED), cPhysics->body->GetWorldCenter(), true);

    if (!isMoving)
    {
      if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkUp")
      {
        animator->playAnimation("walkUp", true);
      }

      cDirection->direction = Up;
    }

    isMoving = true;
  }
  else if (m_keyMaps.isActive("moveDown"))
  {
    cPhysics->body->ApplyLinearImpulse(b2Vec2(0, MOVE_SPEED), cPhysics->body->GetWorldCenter(), true);

    if (!isMoving)
    {
      if (!animator->isPlayingAnimation() || animator->getPlayingAnimation() != "walkDown")
      {
        animator->playAnimation("walkDown", true);
      }

      cDirection->direction = Down;
    }

    isMoving = true;
  }
  else
  {
    cPhysics->body->SetLinearVelocity(b2Vec2(cPhysics->body->GetLinearVelocity().x, cPhysics->body->GetLinearVelocity().y * 0.95f));
  }
  
  if (!isMoving)
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
