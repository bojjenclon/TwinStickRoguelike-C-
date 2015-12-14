#include "cef/UIV8Handler.hpp"

UIV8Handler::UIV8Handler(CefRefPtr<CefBrowser> p_browser) : m_browser(p_browser)
{
  
}

void UIV8Handler::releaseBrowser()
{
  m_browser = nullptr;
}

// Map native functionality to Javascript calls
bool UIV8Handler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
  /*if (name == "getHP")
  {
    //retval = CefV8Value::CreateInt(m_uiValues->currentHealth);
    
    return true;
  }*/
  if (name == "setHP")
  {
    if (arguments.size() == 0 || !arguments[0]->IsInt())
    {
      printf("changeHP requires an integer as its first argument.\n");

      return false;
    }

    auto newHealth = arguments[0]->GetIntValue();

    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("healthChanged");

    CefRefPtr<CefListValue> args = msg->GetArgumentList();
    args->SetInt(0, newHealth);

    m_browser->SendProcessMessage(PID_BROWSER, msg);
    
    return true;
  }

  return false;
}

