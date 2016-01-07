#ifndef SHOP_JS_HANDLER_HPP
#define SHOP_JS_HANDLER_HPP

#include <js/JSHandler.hpp>

using namespace Awesomium;

class ShopJSHandler : public JSHandler
{
public:
  explicit ShopJSHandler(JSValue& p_jsObject);

  // Awesomium functions
  void OnMethodCall(WebView * caller, unsigned remoteObjectId, WebString const & methodName, JSArray const & args) override;
  JSValue OnMethodCallWithReturnValue(WebView * caller, unsigned remoteObjectId, WebString const & methodName, JSArray const & args) override;

  JSValue ElementClicked(WebView* view) const;
  JSValue ItemClicked(WebView* view, JSArray const& args) const;
};

#endif
