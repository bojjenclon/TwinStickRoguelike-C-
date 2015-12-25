#include <BulletEntityFactory.hpp>
#include <components/LifetimeComponent.hpp>
#include <components/HitOnceComponent.hpp>
#include <components/BulletComponent.hpp>
#include <BulletBehavior.hpp>
#include <collisions/BulletCollisionData.hpp>
#include <Game.hpp>
#include <components/RenderComponent.hpp>
#include <components/PhysicsComponent.hpp>

ECS::Entity* BulletEntityFactory::makeBasicBullet(ResourceManager& p_resources, Bullet::Options p_options)
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

  auto cLifetime = engine.createComponent<LifetimeComponent>();
  entity->add(cLifetime);

  auto cHitOnce = engine.createComponent<HitOnceComponent>();
  entity->add(cHitOnce);

  auto cBullet = engine.createComponent<BulletComponent>();
  entity->add(cBullet);
  cBullet->collisionCallback = BulletBehavior::basicBulletCollision;

  /* Physics Begin */

  auto& world = game.getWorld();

  auto cPhysics = engine.createComponent<PhysicsComponent>();
  entity->add(cPhysics);
  cPhysics->hasMaxSpeed = false;

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(p_options.position.x / Game::PIXELS_PER_METER, p_options.position.y / Game::PIXELS_PER_METER);

  auto body = world.CreateBody(&bodyDef);

  auto collisionData = new BulletCollisionData();
  collisionData->type = EntityInfo::Bullet;
  collisionData->entity = entity;
  collisionData->owner = p_options.owner;
  body->SetUserData(collisionData);

  b2CircleShape dynamicCircle;
  dynamicCircle.m_radius = 0.05f;

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  fixtureDef.restitution = 0.5f;

  if (p_options.owner == EntityInfo::Player)
  {
    fixtureDef.filter.categoryBits = Collision::PlayerBullet;
    fixtureDef.filter.maskBits = Collision::Obstacle | Collision::Enemy | Collision::PlayerBullet | Collision::EnemyBullet;
  }
  else if (p_options.owner == EntityInfo::Enemy)
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
