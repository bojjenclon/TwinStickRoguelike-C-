#include <fsm/State.hpp>

State::State(std::string p_name) : m_name(p_name)
{
  
}

std::string State::getName() const
{
  return m_name;
}
