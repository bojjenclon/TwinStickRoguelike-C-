#include <systems/BehaviorTreeSystem.hpp>
#include <components/BehaviorTreeComponent.hpp>

BehaviorTreeSystem::BehaviorTreeSystem() : IteratingSystem(ECS::Family::all<BehaviorTreeComponent>().get())
{
}

void BehaviorTreeSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cBehaviorTree = p_entity->get<BehaviorTreeComponent>();

  auto rootNode = cBehaviorTree->rootNode;

  rootNode->execute(p_entity);
}