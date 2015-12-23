#include <EntityFactory.hpp>

#include <components/RenderComponent.hpp>
#include <components/HealthComponent.hpp>
#include <components/UIComponent.hpp>
#include <components/PlayerComponent.hpp>
#include <components/AnimationComponent.hpp>

#include <Thor/Animations/FrameAnimation.hpp>
#include <components/DirectionComponent.hpp>
#include <components/VelocityComponent.hpp>
#include <components/LifetimeComponent.hpp>
#include <Game.hpp>
#include <components/PhysicsComponent.hpp>

#include <BehaviorTree.h>
#include <components/BehaviorTreeComponent.hpp>
#include <ai/MoveTowardNode.hpp>
#include <collisions/CollisionData.hpp>

ECS::Entity* EntityFactory::makeDrawable(sf::Drawable& p_drawable, int p_depth)
{
  auto& engine = Game::Get().getEngine();

  auto entity = engine.createEntity();

  auto cRender = engine.createComponent<RenderComponent>();
  entity->add(cRender);
  cRender->drawable = &p_drawable;
  cRender->depth = p_depth;

  return entity;
}

ECS::Entity* EntityFactory::makeUIContainer(sf::Sprite& p_sprite, CefRefPtr<CefBrowser> p_browser, UIValues& p_uiValues)
{
  auto& engine = Game::Get().getEngine();

  auto entity = engine.createEntity();

  auto cRender = engine.createComponent<RenderComponent>();
  entity->add(cRender);
  cRender->drawable = &p_sprite;
  cRender->depth = -999;

  auto cUI = engine.createComponent<UIComponent>();
  entity->add(cUI);
  cUI->uiBrowser = p_browser;
  cUI->uiValues = &p_uiValues;

  return entity;
}

ECS::Entity* EntityFactory::makePlayer(ResourceManager& p_resources, sf::Vector2f p_position)
{
  auto& engine = Game::Get().getEngine();

  auto entity = engine.createEntity();

  auto cRender = engine.createComponent<RenderComponent>();
  entity->add(cRender);
  auto sprite = new sf::Sprite(p_resources.getTexture("deer"));
  sprite->setTextureRect(sf::IntRect(32, 0, 32, 34));
  sprite->setOrigin(sprite->getTextureRect().width / 2.0f, sprite->getTextureRect().height / 2.0f);
  sprite->setPosition(p_position);
  cRender->drawable = sprite;

  /* Begin Animation Setup */

  auto animator = new thor::Animator<sf::Sprite, std::string>();
  
  thor::FrameAnimation walkDownAnim;
  walkDownAnim.addFrame(1.0f, sf::IntRect(0, 0, 32, 34));
  walkDownAnim.addFrame(1.0f, sf::IntRect(32, 0, 32, 34));
  walkDownAnim.addFrame(1.0f, sf::IntRect(64, 0, 32, 34));
  walkDownAnim.addFrame(1.0f, sf::IntRect(32, 0, 32, 34));
  animator->addAnimation("walkDown", walkDownAnim, sf::seconds(1.0f));

  thor::FrameAnimation walkLeftAnim;
  walkLeftAnim.addFrame(1.0f, sf::IntRect(0, 34, 32, 34));
  walkLeftAnim.addFrame(1.0f, sf::IntRect(32, 34, 32, 34));
  walkLeftAnim.addFrame(1.0f, sf::IntRect(64, 34, 32, 34));
  walkLeftAnim.addFrame(1.0f, sf::IntRect(32, 34, 32, 34));
  animator->addAnimation("walkLeft", walkLeftAnim, sf::seconds(1.0f));

  thor::FrameAnimation walkRightAnim;
  walkRightAnim.addFrame(1.0f, sf::IntRect(0, 68, 32, 34));
  walkRightAnim.addFrame(1.0f, sf::IntRect(32, 68, 32, 34));
  walkRightAnim.addFrame(1.0f, sf::IntRect(64, 68, 32, 34));
  walkRightAnim.addFrame(1.0f, sf::IntRect(32, 68, 32, 34));
  animator->addAnimation("walkRight", walkRightAnim, sf::seconds(1.0f));

  thor::FrameAnimation walkUpAnim;
  walkUpAnim.addFrame(1.0f, sf::IntRect(0, 102, 32, 34));
  walkUpAnim.addFrame(1.0f, sf::IntRect(32, 102, 32, 34));
  walkUpAnim.addFrame(1.0f, sf::IntRect(64, 102, 32, 34));
  walkUpAnim.addFrame(1.0f, sf::IntRect(32, 102, 32, 34));
  animator->addAnimation("walkUp", walkUpAnim, sf::seconds(1.0f));

  thor::FrameAnimation idleDownAnim;
  idleDownAnim.addFrame(1.0f, sf::IntRect(32, 0, 32, 34));
  animator->addAnimation("idleDown", idleDownAnim, sf::seconds(1.0f));

  thor::FrameAnimation idleLeftAnim;
  idleLeftAnim.addFrame(1.0f, sf::IntRect(32, 34, 32, 34));
  animator->addAnimation("idleLeft", idleLeftAnim, sf::seconds(1.0f));

  thor::FrameAnimation idleRightAnim;
  idleRightAnim.addFrame(1.0f, sf::IntRect(32, 68, 32, 34));
  animator->addAnimation("idleRight", idleRightAnim, sf::seconds(1.0f));

  thor::FrameAnimation idleUpAnim;
  idleUpAnim.addFrame(1.0f, sf::IntRect(32, 102, 32, 34));
  animator->addAnimation("idleUp", idleUpAnim, sf::seconds(1.0f));
  
  auto cAnimation = engine.createComponent<AnimationComponent>();
  entity->add(cAnimation);
  cAnimation->sprite = sprite;
  cAnimation->animator = animator;

  /* End Animation Setup */

  auto cDirection = engine.createComponent<DirectionComponent>();
  entity->add(cDirection);

  auto cHealth = engine.createComponent<HealthComponent>();
  entity->add(cHealth);
  cHealth->currentHealth = cHealth->maxHealth = 10;

  /* Physics Begin */

  auto& world = Game::Get().getWorld();

  auto cPhysics = engine.createComponent<PhysicsComponent>();
  entity->add(cPhysics);

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.fixedRotation = true;
  bodyDef.position.Set(p_position.x / Game::PIXELS_PER_METER, p_position.y / Game::PIXELS_PER_METER);

  auto body = world.CreateBody(&bodyDef);

  auto collisionData = new CollisionData();
  collisionData->type = Entity::Player;
  collisionData->entity = entity;
  body->SetUserData(collisionData);
  
  b2CircleShape dynamicCircle;
  dynamicCircle.m_radius = 0.25f;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  fixtureDef.restitution = 0.15f;

  fixtureDef.filter.categoryBits = Collision::Player;
  fixtureDef.filter.maskBits = Collision::Obstacle | Collision::Enemy | Collision::EnemyBullet;

  body->CreateFixture(&fixtureDef);

  cPhysics->body = body;

  /* Physics End */

  auto cPlayer = engine.createComponent<PlayerComponent>();
  entity->add(cPlayer);

  return entity;
}

ECS::Entity* EntityFactory::makeBullet(ResourceManager& p_resources, Bullet::Options p_options)
{
  auto& game = Game::Get();
  auto& engine = game.getEngine();

  auto entity = engine.createEntity();

  auto cRender = engine.createComponent<RenderComponent>();
  entity->add(cRender);
  auto sprite = new sf::Sprite(p_resources.getTexture("pinkBullet"));
  sprite->setOrigin(sprite->getTextureRect().width / 2.0f, sprite->getTextureRect().height / 2.0f);
  sprite->setPosition(p_options.position);
  cRender->drawable = sprite;

  auto cVelocity = engine.createComponent<VelocityComponent>();
  entity->add(cVelocity);
  cVelocity->vx = p_options.velocity.x;
  cVelocity->vy = p_options.velocity.y;

  auto cLifetime = engine.createComponent<LifetimeComponent>();
  entity->add(cLifetime);

  /* Physics Begin */

  auto& world = game.getWorld();

  auto cPhysics = engine.createComponent<PhysicsComponent>();
  entity->add(cPhysics);
  cPhysics->hasMaxSpeed = false;

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(p_options.position.x / Game::PIXELS_PER_METER, p_options.position.y / Game::PIXELS_PER_METER);
  auto body = world.CreateBody(&bodyDef);
  
  b2CircleShape dynamicCircle;
  dynamicCircle.m_radius = 0.05f;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  fixtureDef.restitution = 0.5f;

  if (p_options.owner == Entity::Player)
  {
    fixtureDef.filter.categoryBits = Collision::PlayerBullet;
    fixtureDef.filter.maskBits = Collision::Obstacle | Collision::Enemy | Collision::PlayerBullet | Collision::EnemyBullet;
  }
  else if (p_options.owner == Entity::Enemy)
  {
    fixtureDef.filter.categoryBits = Collision::EnemyBullet;
    fixtureDef.filter.maskBits = Collision::Obstacle | Collision::Player | Collision::PlayerBullet | Collision::EnemyBullet;
  }
  
  body->CreateFixture(&fixtureDef);
  body->SetBullet(true);

  body->ApplyLinearImpulse(b2Vec2(p_options.velocity.x, p_options.velocity.y), body->GetWorldCenter(), true);

  cPhysics->body = body;

  /* Physics End */

  return entity;
}

ECS::Entity* EntityFactory::makeEnemy(ResourceManager& p_resources, sf::Vector2f p_position)
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
  cHealth->currentHealth = cHealth->maxHealth = 10;

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
  collisionData->type = Entity::Enemy;
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
    ->addChild(new MoveTowardNode(game.getPlayer(), 0.005f, 250.0f));

  cBehaviorTree->rootNode = rootNode;

  /* AI End */

  return entity;
}