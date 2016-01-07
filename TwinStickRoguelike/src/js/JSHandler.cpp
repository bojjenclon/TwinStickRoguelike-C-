#include <js/JSHandler.hpp>

JSHandler::JSHandler(JSValue& p_jsObject, bool p_blockInput) : m_jsObject(p_jsObject), m_blockInput(p_blockInput)
{
}

bool JSHandler::doesBlockInput() const
{
  return m_blockInput;
}
