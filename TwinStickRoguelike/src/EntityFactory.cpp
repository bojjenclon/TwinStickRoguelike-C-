#include <EntityFactory.hpp>

#include <components/RenderComponent.hpp>
#include <components/HealthComponent.hpp>
#include <components/UIComponent.hpp>
#include <components/PlayerComponent.hpp>
#include <components/AnimationComponent.hpp>

#include <Thor/Animations/FrameAnimation.hpp>
#include <components/DirectionComponent.hpp>

ECS::Entity* EntityFactory::makeDrawable(ECS::Engine* p_engine, sf::Drawable& p_drawable, int p_depth = 1)
{
  auto entity = p_engine->createEntity();

  auto cRender = p_engine->createComponent<RenderComponent>();
  entity->add(cRender);
  cRender->drawable = &p_drawable;
  cRender->depth = p_depth;

  return entity;
}

ECS::Entity* EntityFactory::makeUIContainer(ECS::Engine* p_engine, sf::Sprite& p_sprite, CefRefPtr<CefBrowser> p_browser, UIValues& p_uiValues)
{
  auto entity = p_engine->createEntity();

  auto cRender = p_engine->createComponent<RenderComponent>();
  entity->add(cRender);
  cRender->drawable = &p_sprite;
  cRender->depth = -999;

  auto cUI = p_engine->createComponent<UIComponent>();
  entity->add(cUI);
  cUI->uiBrowser = p_browser;
  cUI->uiValues = &p_uiValues;

  return entity;
}

ECS::Entity* EntityFactory::makePlayer(ECS::Engine* p_engine, ResourceManager* p_resources)
{
  auto entity = p_engine->createEntity();

  auto cRender = p_engine->createComponent<RenderComponent>();
  entity->add(cRender);
  if (!p_resources->isTextureLoaded("deer.png"))
  {
    p_resources->loadTexture("deer.png");
  }
  auto sprite = new sf::Sprite(*p_resources->getTexture("deer.png"));
  sprite->setTextureRect(sf::IntRect(32, 0, 32, 34));
  cRender->drawable = sprite;

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
  
  auto cAnimation = p_engine->createComponent<AnimationComponent>();
  entity->add(cAnimation);
  cAnimation->sprite = sprite;
  cAnimation->animator = animator;

  auto cDirection = p_engine->createComponent<DirectionComponent>();
  entity->add(cDirection);

  auto cHealth = p_engine->createComponent<HealthComponent>();
  entity->add(cHealth);
  cHealth->currentHealth = cHealth->maxHealth = 10;

  auto cPlayer = p_engine->createComponent<PlayerComponent>();
  entity->add(cPlayer);

  return entity;
}

