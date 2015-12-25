#include <EnemyEntityFactory.hpp>
#include <Game.hpp>
#include <components/RenderComponent.hpp>
#include <components/DirectionComponent.hpp>
#include <components/HealthComponent.hpp>
#include <components/EnemyComponent.hpp>
#include <components/PhysicsComponent.hpp>
#include <collisions/CollisionData.hpp>
#include <components/BehaviorTreeComponent.hpp>
#include <ai/MoveTowardNode.hpp>

ECS::Entity* EnemyEntityFactory::makeBasicEnemy(ResourceManager& p_resources, sf::Vector2f p_position)
{
  auto& game = Game::Get();
  auto& engine = game.getEngine();

  auto entity = engine.createEntity();

  auto cRender = engine.createComponent<RenderComponent>();
  entity->add(cRender);
  auto sprite = new sf::Sprite(p_resources.getTexture("iffrit"));
  sprite->setTextureRect(sf::IntRect(78, 0, 70, 78));
  sprite->setOrigin(sprite->getTextureRect().width / 2.0f, sprite->getTextureRect().height / 2.0f);
  sprite->setPosition(p_position);
  cRender->drawable = sprite;

  auto cDirection = engine.createComponent<DirectionComponent>();
  entity->add(cDirection);

  auto cHealth = engine.createComponent<HealthComponent>();
  entity->add(cHealth);
  cHealth->currentHealth = cHealth->maxHealth = 5;

  auto cEnemy = engine.createComponent<EnemyComponent>();
  entity->add(cEnemy);

  /* Physics Begin */

  auto& world = game.getWorld();

  auto cPhysics = engine.createComponent<PhysicsComponent>();
  entity->add(cPhysics);

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.fixedRotation = true;
  bodyDef.position.Set(p_position.x / Game::PIXELS_PER_METER, p_position.y / Game::PIXELS_PER_METER);

  auto body = world.CreateBody(&bodyDef);

  auto collisionData = new CollisionData();
  collisionData->type = EntityInfo::Enemy;
  collisionData->entity = entity;
  body->SetUserData(collisionData);

  b2FixtureDef fixtureDef;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  fixtureDef.restitution = 0.15f;
  fixtureDef.filter.categoryBits = Collision::Enemy;
  fixtureDef.filter.maskBits = Collision::Obstacle | Collision::Player | Collision::PlayerBullet;

  b2CircleShape topCircle;
  topCircle.m_radius = 0.28f;
  topCircle.m_p.y = -0.18f;

  fixtureDef.shape = &topCircle;
  body->CreateFixture(&fixtureDef);

  b2PolygonShape middleRect;
  middleRect.SetAsBox(0.28f, 0.2f);

  fixtureDef.shape = &middleRect;
  body->CreateFixture(&fixtureDef);

  b2CircleShape bottomCircle;
  bottomCircle.m_radius = 0.28f;
  bottomCircle.m_p.y = 0.18f;

  fixtureDef.shape = &bottomCircle;
  body->CreateFixture(&fixtureDef);

  cPhysics->body = body;

  /* Physics End */

  /* AI Begin */

  auto cBehaviorTree = engine.createComponent<BehaviorTreeComponent>();
  entity->add(cBehaviorTree);

  auto rootNode = new BehaviorTree::ParallelNode();

  rootNode
    ->addChild(new MoveTowardNode(game.getPlayer(), 0.005f, 150.0f));

  cBehaviorTree->rootNode = rootNode;

  /* AI End */

  return entity;
}
