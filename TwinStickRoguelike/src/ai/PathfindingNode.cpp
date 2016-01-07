#include <ai/PathfindingNode.hpp>
#include <ecstasy/core/Entity.h>
#include <components/PhysicsComponent.hpp>
#include <components/MicroPatherComponent.hpp>
#include <components/NodeWatchComponent.hpp>
#include <pathfinding/MicropatherNode.hpp>
#include <Constants.hpp>
#include <components/RenderComponent.hpp>

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
    parentCMicroPather->path.clear();
    parentCMicroPather->smoothedPath.clear();

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
      std::vector<sf::Vector2f> smoothedPath;

      auto halfSpriteSize = sf::Vector2f(
        parentSprite->getTextureRect().width / 2.0f,
        parentSprite->getTextureRect().height / 2.0f);

      auto slope = 0.0f;
      sf::Vector2f startPoint;
      sf::Vector2f lastPoint;

      for (unsigned int i = 0; i < parentCMicroPather->path.size(); ++i)
      {
        auto currentNode = reinterpret_cast<MicroPatherNode*>(parentCMicroPather->path[i]);

        if (i == 0)
        {
          startPoint.x = static_cast<float>(currentNode->x);
          startPoint.y = static_cast<float>(currentNode->y);
        }

        if (currentNode->x == lastPoint.x)
        {
          if (currentNode->x != startPoint.x)
          {
            sf::Vector2f pathPoint;
            pathPoint.x = startPoint.x * Constants::COLLISION_TILE_WIDTH + halfSpriteSize.x;
            pathPoint.y = startPoint.y * Constants::COLLISION_TILE_WIDTH + halfSpriteSize.x;

            smoothedPath.push_back(pathPoint);

            startPoint.x = static_cast<float>(currentNode->x);
            startPoint.y = static_cast<float>(currentNode->y);
          }
        }
        else if (currentNode->y == lastPoint.y)
        {
          if (currentNode->y != startPoint.y)
          {
            sf::Vector2f pathPoint;
            pathPoint.x = startPoint.x * Constants::COLLISION_TILE_WIDTH + halfSpriteSize.x;
            pathPoint.y = startPoint.y * Constants::COLLISION_TILE_WIDTH + halfSpriteSize.x;

            smoothedPath.push_back(pathPoint);

            startPoint.x = static_cast<float>(currentNode->x);
            startPoint.y = static_cast<float>(currentNode->y);
          }
        }
        else
        {
          auto currentSlope = (lastPoint.y - currentNode->y) / (lastPoint.x - currentNode->x);
          
          if (currentSlope != slope)
          {
            sf::Vector2f pathPoint;
            pathPoint.x = startPoint.x * Constants::COLLISION_TILE_WIDTH + halfSpriteSize.x;
            pathPoint.y = startPoint.y * Constants::COLLISION_TILE_WIDTH + halfSpriteSize.x;

            smoothedPath.push_back(pathPoint);

            startPoint.x = static_cast<float>(currentNode->x);
            startPoint.y = static_cast<float>(currentNode->y);

            slope = currentSlope;
          }
        }

        lastPoint.x = static_cast<float>(currentNode->x);
        lastPoint.y = static_cast<float>(currentNode->y);
      }

      smoothedPath.push_back(sf::Vector2f(
        lastPoint.x * Constants::COLLISION_TILE_WIDTH + halfSpriteSize.x,
        lastPoint.y * Constants::COLLISION_TILE_HEIGHT + halfSpriteSize.y));

      parentCMicroPather->smoothedPath = smoothedPath;

#ifdef _DEBUG
      auto debugDraw = &parentCMicroPather->debugDraw;
      //debugDraw->setPrimitiveType(sf::Points);
      debugDraw->setPrimitiveType(sf::LinesStrip);
      debugDraw->resize(parentCMicroPather->smoothedPath.size());

      for (unsigned int i = 0; i < parentCMicroPather->smoothedPath.size(); ++i)
      {
        auto node = parentCMicroPather->smoothedPath[i];

        debugDraw->operator[](i).position = node;
        debugDraw->operator[](i).color = sf::Color::Red;
      }
#endif
    }
    else
    {
      parentBody->SetLinearVelocity(b2Vec2(0, 0));

#ifdef _DEBUG
      auto debugDraw = &parentCMicroPather->debugDraw;
      debugDraw->clear();
#endif
    }

    parentCMicroPather->positionInPath = 0;
  }

  auto moved = false;

  if (parentCMicroPather->smoothedPath.size() > 0 && parentCMicroPather->positionInPath < parentCMicroPather->smoothedPath.size())
  {
    auto node = parentCMicroPather->smoothedPath[parentCMicroPather->positionInPath];

    auto dx = node.x - parentBody->GetPosition().x * Constants::PIXELS_PER_METER;
    auto dy = node.y - parentBody->GetPosition().y * Constants::PIXELS_PER_METER;

    auto thresholdX = Constants::COLLISION_TILE_WIDTH;
    auto thresholdY = Constants::COLLISION_TILE_HEIGHT;

    if (abs(dx) < thresholdX && abs(dy) < thresholdY)
    {
      parentCMicroPather->positionInPath++;
    }
    else
    {
      b2Vec2 force(dx, dy);
      force.Normalize();
      
      parentBody->SetLinearVelocity(b2Vec2(parentBody->GetLinearVelocity().x * 0.9f, parentBody->GetLinearVelocity().y * 0.9f));
      parentBody->ApplyLinearImpulse(force * m_speed, parentBody->GetWorldCenter(), true);
    }

    moved = true;
  }
  else if (parentCMicroPather->smoothedPath.size() == 0 || parentCMicroPather->positionInPath >= parentCMicroPather->smoothedPath.size())
  {
    parentBody->SetLinearVelocity(b2Vec2(0, 0));
  }
  
  return moved ? BehaviorTree::BT_RUNNING : BehaviorTree::BT_SUCCESS;
}
