#ifndef BEHAVIOR_TREE_COMPONENT_HPP
#define BEHAVIOR_TREE_COMPONENT_HPP

#include <ecstasy/core/Component.h>
#include <BehaviorTree.h>

struct BehaviorTreeComponent : public ECS::Component<BehaviorTreeComponent>
{
  BehaviorTree::BehaviorTreeInternalNode* rootNode = nullptr;

  void reset() override
  {
    rootNode = nullptr;
  }
};

#endif

