#include <systems/AnimationSystem.hpp>
#include <components/AnimationComponent.hpp>

AnimationSystem::AnimationSystem() : IteratingSystem(ECS::Family::all<AnimationComponent>().get())
{
}

void AnimationSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cAnimation = p_entity->get<AnimationComponent>();

  auto sprite = cAnimation->sprite;
  auto animator = cAnimation->animator;

  animator->update(sf::seconds(p_dt));
  animator->animate(*sprite);
}