#ifndef UI_COMPONENT_HPP
#define UI_COMPONENT_HPP

#include <ecstasy/core/Component.h>

#include <include/cef_browser.h>

#include <UIValues.hpp>

struct UIComponent : public ECS::Component<UIComponent>
{
  CefRefPtr<CefBrowser> uiBrowser = nullptr;
  UIValues* uiValues = nullptr;

  void reset() override
  {
    uiBrowser = nullptr;
    uiValues = nullptr;
  }
};

#endif

