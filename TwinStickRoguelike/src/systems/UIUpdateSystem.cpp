#include <systems/UIUpdateSystem.hpp>
#include <components/UIComponent.hpp>

UIUpdateSystem::UIUpdateSystem() : IteratingSystem(ECS::Family::all<UIComponent>().get())
{
  
}

void UIUpdateSystem::processEntity(ECS::Entity* entity, float deltaTime)
{
  auto cUI = entity->get<UIComponent>();

  auto frame = cUI->uiBrowser->GetMainFrame();
  auto uiValues = cUI->uiValues;
  
  if (uiValues->healthChanged)
  {
    frame->ExecuteJavaScript("setHP(" + std::to_string(static_cast<int>(1.0f * uiValues->currentHealth / uiValues->maxHealth * 100)) + ");", frame->GetURL(), 0);
    uiValues->healthChanged = false;
  }
}