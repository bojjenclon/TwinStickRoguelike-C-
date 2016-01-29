#include <factories/ItemEntityFactory.hpp>
#include <Game.hpp>
#include <components/ActiveComponent.hpp>
#include <components/RenderComponent.hpp>
#include <Constants.hpp>
#include <components/PhysicsComponent.hpp>
#include <collisions/ItemCollisionData.hpp>

ECS::Entity* ItemEntityFactory::makeItem(const ItemOptions& p_options)
{
  auto& game = Game::Get();

  auto& engine = game.getEngine();

  auto entity = engine.createEntity();

  auto cActive = engine.createComponent<ActiveComponent>();
  entity->add(cActive);
  cActive->isActive = p_options.isActive;

  auto cRender = engine.createComponent<RenderComponent>();
  entity->add(cRender);
  cRender->drawable = static_cast<sf::Drawable*>(&p_options.sprite);
  cRender->depth = 2;

  /* Physics Begin */

  auto& world = game.getWorld();

  auto cPhysics = engine.createComponent<PhysicsComponent>();
  entity->add(cPhysics);
  cPhysics->hasMaxSpeed = false;

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(p_options.position.x / Constants::PIXELS_PER_METER, p_options.position.y / Constants::PIXELS_PER_METER);

  auto body = world.CreateBody(&bodyDef);

  auto collisionData = new ItemCollisionData();
  collisionData->type = EntityInfo::Item;
  collisionData->entity = entity;
  collisionData->name = p_options.name;
  body->SetUserData(collisionData);
  
  b2CircleShape dynamicCircle;
  auto radius = static_cast<float>(p_options.sprite.getTextureRect().width > p_options.sprite.getTextureRect().height ? p_options.sprite.getTextureRect().width : p_options.sprite.getTextureRect().height);
  radius /= 2;
  radius /= Constants::PIXELS_PER_METER;
  dynamicCircle.m_radius = radius;

  /* Collideable Fixture */

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  fixtureDef.restitution = 0.5f;

  fixtureDef.filter.categoryBits = Collision::Item;
  fixtureDef.filter.maskBits = Collision::Obstacle;

  body->CreateFixture(&fixtureDef);

  /* Player Pickup Fixture */

  dynamicCircle.m_radius *= 0.75f;

  fixtureDef.shape = &dynamicCircle;
  fixtureDef.isSensor = true;

  fixtureDef.filter.categoryBits = Collision::Item;
  fixtureDef.filter.maskBits = Collision::Player;

  body->CreateFixture(&fixtureDef);
  
  cPhysics->body = body;
  cPhysics->previousPosition.x = body->GetPosition().x;
  cPhysics->previousPosition.y = body->GetPosition().y;

  /* Physics End */

  return entity;
}
