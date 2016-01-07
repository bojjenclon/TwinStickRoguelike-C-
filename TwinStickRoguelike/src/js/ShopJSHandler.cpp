#include <js/ShopJSHandler.hpp>
#include <Awesomium/STLHelpers.h>

ShopJSHandler::ShopJSHandler(JSValue& p_jsObject) : JSHandler(p_jsObject, true)
{
  auto& appObject = m_jsObject.ToObject();

  appObject.SetCustomMethod(WSLit("elementClicked"), false);
  JsCallerKey elementClickedKey(appObject.remote_id(), WSLit("elementClicked"));
  m_jsFunctions[elementClickedKey] = std::bind(&ShopJSHandler::ElementClicked, this, std::placeholders::_1);

  appObject.SetCustomMethod(WSLit("itemClicked"), false);
  JsCallerKey itemClickedKey(appObject.remote_id(), WSLit("itemClicked"));
  m_jsFunctions[itemClickedKey] = std::bind(&ShopJSHandler::ItemClicked, this, std::placeholders::_1, std::placeholders::_2);
}

void ShopJSHandler::OnMethodCall(WebView* caller, unsigned remoteObjectId, WebString const& methodName, JSArray const& args)
{
  JsCallerKey key(remoteObjectId, methodName);
  auto itor = m_jsFunctions.find(key);
  
  if (itor != m_jsFunctions.end())
  {
    itor->second(caller, args);
  }
}

JSValue ShopJSHandler::OnMethodCallWithReturnValue(WebView* caller, unsigned remoteObjectId, WebString const& methodName, JSArray const& args)
{
  JsCallerKey key(remoteObjectId, methodName);
  auto itor = m_jsFunctionsWithRetValue.find(key);

  if (itor != m_jsFunctionsWithRetValue.end())
  {
    return itor->second(caller, args);
  }

  return JSValue::Undefined();
}

JSValue ShopJSHandler::ElementClicked(WebView* view) const
{
  printf("Test\n");

  return JSValue::Undefined();
}

JSValue ShopJSHandler::ItemClicked(WebView* view, JSArray const& args) const
{
  if (args.size() == 0)
    return JSValue();

  JSValue const& arg = args[0];
  if (!arg.IsInteger())
    return JSValue();

  auto index = arg.ToInteger();

  printf("Item index #%d\n", index);

  return JSValue();
}
