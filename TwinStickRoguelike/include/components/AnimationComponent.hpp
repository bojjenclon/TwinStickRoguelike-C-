#ifndef ANIMATION_COMPONENT_HPP
#define ANIMATION_COMPONENT_HPP

#include <ecstasy/core/Component.h>
#include <SFML/Graphics/Sprite.hpp>
#include <Thor/Animations/Animator.hpp>

struct AnimationComponent : public ECS::Component<AnimationComponent>
{
  sf::Sprite* sprite = nullptr;
  thor::Animator<sf::Sprite, std::string>* animator = nullptr;

  void reset() override
  {
    // animation components should always be paired with a render component, 
    // therefore deleting the sprite is not this component's responsibility
    sprite = nullptr;
    
    if (animator != nullptr)
    {
      delete animator;
      animator = nullptr;
    }
  }
};

#endif
