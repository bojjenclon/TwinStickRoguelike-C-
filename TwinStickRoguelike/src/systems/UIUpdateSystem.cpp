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

  if (uiValues->enemy.displayChanged)
  {
    if (uiValues->enemy.display)
    {
      frame->ExecuteJavaScript("HUD.showHud('enemy');", frame->GetURL(), 0);
    }
    else
    {
      frame->ExecuteJavaScript("HUD.hideHud('enemy');", frame->GetURL(), 0);
    }

    uiValues->enemy.displayChanged = false;
  }
  
  if (uiValues->player.healthChanged)
  {
    frame->ExecuteJavaScript("HUD.setHP(" + std::to_string(static_cast<int>(1.0f * uiValues->player.currentHealth / uiValues->player.maxHealth * 100)) + ", 'player');", frame->GetURL(), 0);
    uiValues->player.healthChanged = false;
  }
  else if (uiValues->enemy.display && uiValues->enemy.healthChanged)
  {
    frame->ExecuteJavaScript("HUD.setHP(" + std::to_string(static_cast<int>(1.0f * uiValues->enemy.currentHealth / uiValues->enemy.maxHealth * 100)) + ", 'enemy');", frame->GetURL(), 0);
    uiValues->enemy.healthChanged = false;
  }
}
