#include <ai/MoveTowardNode.hpp>
#include <ecstasy/core/Entity.h>
#include <components/RenderComponent.hpp>
#include <components/PhysicsComponent.hpp>

MoveTowardNode::MoveTowardNode(ECS::Entity* p_target, float p_speed, float p_leashDistance) : m_target(p_target), m_speed(p_speed), m_leashDistance(p_leashDistance)
{
}

MoveTowardNode::~MoveTowardNode()
{
  m_target = nullptr;
}

void MoveTowardNode::init(void* p_agent)
{
  
}

BehaviorTree::BEHAVIOR_STATUS MoveTowardNode::execute(void* p_agent)
{
  if (m_target == nullptr || m_target->getId() == 0)
  {
    return BehaviorTree::BT_FAILURE;
  }

  auto entity = static_cast<ECS::Entity*>(p_agent);
  auto parentCRender = entity->get<RenderComponent>();
  auto parentSprite = dynamic_cast<sf::Sprite*>(parentCRender->drawable);

  auto targetCRender = m_target->get<RenderComponent>();
  auto targetSprite = dynamic_cast<sf::Sprite*>(targetCRender->drawable);

  auto dx = targetSprite->getPosition().x - parentSprite->getPosition().x;
  auto dy = targetSprite->getPosition().y - parentSprite->getPosition().y;

  auto parentCPhysics = entity->get<PhysicsComponent>();
  auto parentBody = parentCPhysics->body;

  auto moved = false;

  if (abs(dx) > m_leashDistance)
  {
    auto sign = dx < 0 ? -1 : 1;
    parentBody->ApplyLinearImpulse(b2Vec2(m_speed * sign, 0), parentBody->GetWorldCenter(), true);

    moved = true;
  }

  if (abs(dy) > m_leashDistance)
  {
    auto sign = dy < 0 ? -1 : 1;
    parentBody->ApplyLinearImpulse(b2Vec2(0, m_speed * sign), parentBody->GetWorldCenter(), true);

    moved = true;
  }

  return moved ? BehaviorTree::BT_RUNNING : BehaviorTree::BT_SUCCESS;
}
