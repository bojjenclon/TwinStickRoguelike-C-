#ifndef JS_HANDLER_HPP
#define JS_HANDLER_HPP

#include <Awesomium/JSObject.h>
#include <Awesomium/JSValue.h>
#include <functional>
#include <map>

using namespace Awesomium;

class JSHandler : public JSMethodHandler
{
public:
  explicit JSHandler(JSValue& p_jsObject, bool p_blockInput = false);

  bool doesBlockInput() const;

  // Awesomium functions
  virtual void OnMethodCall(WebView * caller, unsigned remoteObjectId, WebString const & methodName, JSArray const & args) override = 0;
  virtual JSValue OnMethodCallWithReturnValue(WebView * caller, unsigned remoteObjectId, WebString const & methodName, JSArray const & args) override = 0;

protected:
  typedef std::pair<unsigned, WebString> JsCallerKey;
  typedef std::function<JSValue(WebView *, JSArray const &)> JsFunction;

  JSValue& m_jsObject;

  std::map<JsCallerKey, JsFunction> m_jsFunctions;
  std::map<JsCallerKey, JsFunction> m_jsFunctionsWithRetValue;

  bool m_blockInput;
};

#endif
