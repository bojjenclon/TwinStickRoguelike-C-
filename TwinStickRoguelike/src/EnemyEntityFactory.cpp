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
#include <Constants.hpp>
#include <ai/PathfindingNode.hpp>
#include <components/MicroPatherComponent.hpp>

ECS::Entity* EnemyEntityFactory::makeBasicEnemy(ResourceManager& p_resources, TiledMap* p_map, sf::Vector2f p_position)
{
  auto& game = Game::Get();
  auto& engine = game.getEngine();

  auto entity = engine.createEntity();

  auto cRender = engine.createComponent<RenderComponent>();
  entity->add(cRender);
  /*auto sprite = new sf::Sprite(p_resources.getTexture("iffrit"));
  sprite->setTextureRect(sf::IntRect(78, 0, 70, 78));*/
  auto sprite = new sf::Sprite(p_resources.getTexture("deer"));
  sprite->setTextureRect(sf::IntRect(32, 0, 32, 34));
  sprite->setOrigin(sprite->getTextureRect().width / 2.0f, sprite->getTextureRect().height / 2.0f);
  sprite->setPosition(p_position);
  sprite->setColor(sf::Color::Red);
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
  bodyDef.position.Set(p_position.x / Constants::PIXELS_PER_METER, p_position.y / Constants::PIXELS_PER_METER);

  auto body = world.CreateBody(&bodyDef);

  auto collisionData = new CollisionData();
  collisionData->type = EntityInfo::Enemy;
  collisionData->entity = entity;
  body->SetUserData(collisionData);

  b2CircleShape dynamicCircle;
  auto radius = 1.0f * (sprite->getTextureRect().width > sprite->getTextureRect().height ? sprite->getTextureRect().width : sprite->getTextureRect().height);
  radius /= 2;
  radius /= Constants::PIXELS_PER_METER;
  dynamicCircle.m_radius = radius;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  fixtureDef.restitution = 0.15f;

  fixtureDef.filter.categoryBits = Collision::Enemy;
  fixtureDef.filter.maskBits = Collision::Obstacle | Collision::Player | Collision::PlayerBullet;

  body->CreateFixture(&fixtureDef);

  cPhysics->body = body;
  cPhysics->previousPosition.x = body->GetPosition().x;
  cPhysics->previousPosition.y = body->GetPosition().y;

  /* Physics End */

  auto cMicroPather = engine.createComponent<MicroPatherComponent>();
  entity->add(cMicroPather);

  /* AI Begin */

  auto cBehaviorTree = engine.createComponent<BehaviorTreeComponent>();
  entity->add(cBehaviorTree);

  auto rootNode = new BehaviorTree::ParallelNode();
  
  /*rootNode
    ->addChild(new MoveTowardNode(game.getPlayer(), 0.005f, 150.0f));*/
  rootNode
    ->addChild(new PathfindingNode(game.getPlayer(), p_map, 0.005f));

  cBehaviorTree->rootNode = rootNode;

  /* AI End */

  return entity;
}
