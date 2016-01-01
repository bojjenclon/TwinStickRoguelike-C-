#ifndef UI_COMPONENT_HPP
#define UI_COMPONENT_HPP

#include <ecstasy/core/Component.h>
#include <Awesomium/WebCore.h>
#include <UIValues.hpp>

using namespace Awesomium;

struct UIComponent : public ECS::Component<UIComponent>
{
  WebView* uiWebView = nullptr;
  UIValues* uiValues = nullptr;

  void reset() override
  {
    uiWebView = nullptr;
    uiValues = nullptr;
  }
};

#endif

