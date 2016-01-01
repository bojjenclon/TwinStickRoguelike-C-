#include <systems/UIUpdateSystem.hpp>
#include <components/UIComponent.hpp>
#include <Awesomium/STLHelpers.h>

UIUpdateSystem::UIUpdateSystem() : IteratingSystem(ECS::Family::all<UIComponent>().get())
{
}

void UIUpdateSystem::processEntity(ECS::Entity* entity, float deltaTime)
{
  auto cUI = entity->get<UIComponent>();

  auto webView = cUI->uiWebView;
  auto uiValues = cUI->uiValues;

  if (uiValues->enemy.displayChanged)
  {
    if (uiValues->enemy.display)
    {
      std::string js = "HUD.showHud('enemy');";
      webView->ExecuteJavascript(WSLit(js.c_str()), WSLit(""));
    }
    else
    {
      std::string js = "HUD.hideHud('enemy');";
      webView->ExecuteJavascript(WSLit(js.c_str()), WSLit(""));
    }

    uiValues->enemy.displayChanged = false;
  }
  
  if (uiValues->player.healthChanged)
  {
    std::string js = "HUD.setHP(" + std::to_string(static_cast<int>(1.0f * uiValues->player.currentHealth / uiValues->player.maxHealth * 100)) + ", 'player');";
    webView->ExecuteJavascript(WSLit(js.c_str()), WSLit(""));
    uiValues->player.healthChanged = false;
  }
  else if (uiValues->enemy.display && uiValues->enemy.healthChanged)
  {
    std::string js = "HUD.setHP(" + std::to_string(static_cast<int>(1.0f * uiValues->enemy.currentHealth / uiValues->enemy.maxHealth * 100)) + ", 'enemy');";
    webView->ExecuteJavascript(WSLit(js.c_str()), WSLit(""));
    uiValues->enemy.healthChanged = false;
  }
}
