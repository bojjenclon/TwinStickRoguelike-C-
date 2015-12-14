#ifndef UI_V8_HANDLER_HPP
#define UI_V8_HANDLER_HPP

#include <include/cef_v8.h>
#include "UIValues.hpp"

class UIV8Handler : public CefV8Handler
{
public:
  UIV8Handler(CefRefPtr<CefBrowser> p_browser);

  void releaseBrowser();

  virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) override;

private:
  CefRefPtr<CefBrowser> m_browser;

  IMPLEMENT_REFCOUNTING(UIV8Handler);
};

#endif
