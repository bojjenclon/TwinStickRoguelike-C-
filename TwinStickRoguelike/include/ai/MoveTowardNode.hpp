#ifndef MOVE_TOWARD_NODE_HPP
#define MOVE_TOWARD_NODE_HPP

#include <BehaviorTree.h>
#include <ecstasy/core/Entity.h>

class MoveTowardNode : public BehaviorTree::BehaviorTreeNode
{
public:
  MoveTowardNode(ECS::Entity* p_target, float p_speed = 0.005f);
  virtual ~MoveTowardNode();

  void init(void* p_agent) override;
  BehaviorTree::BEHAVIOR_STATUS execute(void* p_agent) override;
  
private:
  ECS::Entity* m_target;
  float m_speed;
};

#endif