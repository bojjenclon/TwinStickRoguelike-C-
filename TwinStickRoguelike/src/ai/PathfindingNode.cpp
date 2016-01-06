#include <ai/PathfindingNode.hpp>
#include <ecstasy/core/Entity.h>
#include <components/PhysicsComponent.hpp>
#include <components/MicroPatherComponent.hpp>
#include <components/NodeWatchComponent.hpp>
#include <pathfinding/MicropatherNode.hpp>
#include <Constants.hpp>
#include <components/RenderComponent.hpp>

template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

PathfindingNode::PathfindingNode(ECS::Entity* p_target, TiledMap* p_map, float p_speed) : m_target(p_target), m_map(p_map), m_speed(p_speed)
{
}

PathfindingNode::~PathfindingNode()
{
  m_target = nullptr;
}

void PathfindingNode::init(void* p_agent)
{

}

BehaviorTree::BEHAVIOR_STATUS PathfindingNode::execute(void* p_agent)
{
  if (m_target == nullptr || m_target->getId() == 0)
  {
    return BehaviorTree::BT_FAILURE;
  }

  auto entity = static_cast<ECS::Entity*>(p_agent);
  auto parentCMicroPather = entity->get<MicroPatherComponent>();
  auto parentCPhysics = entity->get<PhysicsComponent>();
  auto parentCRender = entity->get<RenderComponent>();

  auto parentBody = parentCPhysics->body;
  auto parentSprite = static_cast<sf::Sprite*>(parentCRender->drawable);

  auto targetCNodeWatch = m_target->get<NodeWatchComponent>();
  
  if (targetCNodeWatch->nodeChanged)
  {
    auto targetCRender = m_target->get<RenderComponent>();
    auto targetSprite = static_cast<sf::Sprite*>(targetCRender->drawable);

    auto targetCPhysics = m_target->get<PhysicsComponent>();
    auto targetBody = targetCPhysics->body;

    auto pather = m_map->getPather();

    // calculate start node
    auto parentWidth = static_cast<int>(ceil(parentSprite->getTextureRect().width / Constants::COLLISION_TILE_WIDTH));
    auto parentHeight = static_cast<int>(ceil(parentSprite->getTextureRect().height / Constants::COLLISION_TILE_HEIGHT));

    auto startX = static_cast<int>(parentBody->GetPosition().x * Constants::PIXELS_PER_METER / Constants::COLLISION_TILE_WIDTH) - (parentWidth / 2);
    auto startY = static_cast<int>(parentBody->GetPosition().y * Constants::PIXELS_PER_METER / Constants::COLLISION_TILE_HEIGHT) - (parentHeight / 2);

    auto startNode = m_map->getPatherNode(startX, startY, parentWidth, parentHeight);

    // calculate end node
    auto targetWidth = static_cast<int>(ceil(targetSprite->getTextureRect().width / Constants::COLLISION_TILE_WIDTH / 2));
    auto targetHeight = static_cast<int>(ceil(targetSprite->getTextureRect().height / Constants::COLLISION_TILE_HEIGHT / 2));

    auto endX = static_cast<int>(targetBody->GetPosition().x * Constants::PIXELS_PER_METER / Constants::COLLISION_TILE_WIDTH) - (targetWidth / 2);
    auto endY = static_cast<int>(targetBody->GetPosition().y * Constants::PIXELS_PER_METER / Constants::COLLISION_TILE_HEIGHT) - (targetHeight / 2);

    auto endNode = m_map->getPatherNode(endX, endY, targetWidth, targetHeight);

    float totalCost = 0;
    auto result = pather->Solve(startNode, endNode, &parentCMicroPather->path, &totalCost);

    if (result == MicroPather::SOLVED)
    {
#ifdef _DEBUG
      auto debugDraw = &parentCMicroPather->debugDraw;
      debugDraw->setPrimitiveType(sf::LinesStrip);
      debugDraw->resize(parentCMicroPather->path.size());

      auto halfTileWidth = Constants::COLLISION_TILE_WIDTH / 2.0f;
      auto halfTileHeight = Constants::COLLISION_TILE_HEIGHT / 2.0f;

      auto widthAdjust = parentWidth / 2.0f * Constants::COLLISION_TILE_WIDTH;
      auto heightAdjust = parentHeight / 2.0f * Constants::COLLISION_TILE_HEIGHT;

      for (unsigned int i = 0; i < parentCMicroPather->path.size(); ++i)
      {
        if (parentCMicroPather->path[i] == nullptr)
        {
          continue;
        }

        auto node = reinterpret_cast<MicroPatherNode*>(parentCMicroPather->path[i]);
        debugDraw->operator[](i).position = sf::Vector2f(
          node->x * Constants::COLLISION_TILE_WIDTH + halfTileWidth + widthAdjust,
          node->y * Constants::COLLISION_TILE_HEIGHT + halfTileHeight + heightAdjust
          );
        debugDraw->operator[](i).color = sf::Color::Red;
      }
#endif
    }
    else
    {
#ifdef _DEBUG
      auto debugDraw = &parentCMicroPather->debugDraw;
      debugDraw->clear();
#endif

      parentBody->SetLinearVelocity(b2Vec2(0, 0));
    }

    parentCMicroPather->positionInPath = 0;
  }

  auto moved = false;

  if (parentCMicroPather->path.size() > 0 && parentCMicroPather->positionInPath < parentCMicroPather->path.size())
  {
    auto node = reinterpret_cast<MicroPatherNode*>(parentCMicroPather->path[parentCMicroPather->positionInPath]);

    if (node == nullptr)
    {
      return BehaviorTree::BT_FAILURE;
    }

    auto nodeX = node->x * Constants::COLLISION_TILE_WIDTH + parentSprite->getTextureRect().width / 2.0f;
    auto nodeY = node->y * Constants::COLLISION_TILE_HEIGHT + parentSprite->getTextureRect().height / 2.0f;

    auto dx = nodeX - parentBody->GetPosition().x * Constants::PIXELS_PER_METER;
    auto dy = nodeY - parentBody->GetPosition().y * Constants::PIXELS_PER_METER;
    
    if (abs(dx) < 5.0f && abs(dy) < 5.0f)
    {
      parentCMicroPather->positionInPath++;
    }
    else
    {
      if (abs(dx) >= 5.0f)
      {
        parentBody->ApplyLinearImpulse(b2Vec2(m_speed * sgn(dx), 0), parentBody->GetWorldCenter(), true);
      }
      else
      {
        parentBody->SetLinearVelocity(b2Vec2(parentBody->GetLinearVelocity().x * 0.9f, parentBody->GetLinearVelocity().y));
      }

      if (abs(dy) >= 5.0f)
      {
        parentBody->ApplyLinearImpulse(b2Vec2(0, m_speed * sgn(dy)), parentBody->GetWorldCenter(), true);
      }
      else
      {
        parentBody->SetLinearVelocity(b2Vec2(parentBody->GetLinearVelocity().x, parentBody->GetLinearVelocity().y * 0.9f));
      }
    }

    moved = true;
  }

  return moved ? BehaviorTree::BT_RUNNING : BehaviorTree::BT_SUCCESS;
}
