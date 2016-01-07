#ifndef PATHFINDING_NODE_HPP
#define PATHFINDING_NODE_HPP

#include <BehaviorTree.h>
#include <ecstasy/core/Entity.h>
#include <tiled/TiledMap.hpp>

class PathfindingNode : public BehaviorTree::BehaviorTreeNode
{
public:
  PathfindingNode(ECS::Entity* p_target, TiledMap* p_map,  float p_speed = 0.05f);
  virtual ~PathfindingNode();

  void init(void* p_agent) override;
  BehaviorTree::BEHAVIOR_STATUS execute(void* p_agent) override;

private:
  ECS::Entity* m_target = nullptr;
  TiledMap* m_map = nullptr;
  float m_speed;
};

#endif